#include <algorithm>

#include <AnalyzerChannelData.h>

#include "SWDAnalyzer.h"
#include "SWDAnalyzerSettings.h"
#include "SWDUtils.h"

SWDAnalyzer::SWDAnalyzer() : Analyzer2(), mSWDIO(), mSWCLK(), mSimulationInitilized( false )
{
    SetAnalyzerSettings( &mSettings );
}

SWDAnalyzer::~SWDAnalyzer()
{
    KillThread();
}

void SWDAnalyzer::SetupResults()
{
    // reset the results
    mResults.reset( new SWDAnalyzerResults( this, &mSettings ) );
    SetAnalyzerResults( mResults.get() );

    // set which channels will carry bubbles
    mResults->AddChannelBubblesWillAppearOn( mSettings.mSWDIO );
    mResults->AddChannelBubblesWillAppearOn( mSettings.mSWCLK );
}

void SWDAnalyzer::WorkerThread()
{
    // SetupResults();
    // get the channel pointers
    mSWDIO = GetAnalyzerChannelData( mSettings.mSWDIO );
    mSWCLK = GetAnalyzerChannelData( mSettings.mSWCLK );

    mSWDParser.Setup( mSWDIO, mSWCLK, this );

    mSWDParser.Clear();

    // Set parser initial values
    mSWDParser.SetCurrentProtocol( mSettings.mDProtocol );
    mSWDParser.SetLastFrameType( mSettings.mLastFrame );
    mSWDParser.SetDPVersion( mSettings.mDPVersion );
    mSWDParser.SetNumTurnarounds( mSettings.mNumTurnarounds );
    mSWDParser.SetOverrunDetection( mSettings.mOverrunDetection );
    mSWDParser.SetSelectRegister( mSettings.mSelectRegister );

    SWDErrorBits& errorBits = mSWDParser.GetErrorBits();
    SwdFrameTypes previousFrameType = mSWDParser.GetLastFrameType();
    DebugProtocol dProtocol = mSWDParser.GetCurrentProtocol();

    for( ;; )
    {
        if( mSWDParser.IsBufferEmpty() || ( bestFixedLengthCmpResult == SeqCmpResult::SEQ_MATCH_PARTIALLY ) ||
            ( bestVariableLengthCmpResult == SeqCmpResult::SEQ_MATCH_PARTIALLY ) || ( bestCompleteMatched <= bestPartialyMatched ) )
        {
            // Get the next bit from the SWDIO channel
            mSWDParser.AddBitToBuffer();
        }

        ClearBestMatchingResults();

        for( auto wrappedSeqRef : mSWDParser.GetSequences() )
        {
            SWDBaseSequence& sequence = wrappedSeqRef.get();
            if( sequence.IsDebugProtocolAllowed( dProtocol ) )
            {
                if( sequence.IsPreviousFrameTypeAllowed( previousFrameType ) )
                {
                    const SeqCmpResult sequenceMached = mSWDParser.Match( sequence );
                    const bool fixedLengthSequence = sequence.IsFixedLengthSequence();
                    if( fixedLengthSequence )
                    {
                        bestFixedLengthCmpResult = BestMach( bestFixedLengthCmpResult, sequenceMached );
                    }
                    else
                    {
                        bestVariableLengthCmpResult = BestMach( bestVariableLengthCmpResult, sequenceMached );
                        if( sequenceMached == SeqCmpResult::SEQ_MATCH_PARTIALLY )
                        {
                            const auto numberCheckedBits = sequence.GetNumberCheckedBits();
                            if( numberCheckedBits > bestPartialyMatched )
                            {
                                bestPartialyMatched = numberCheckedBits;
                            }
                        }
                        else if( sequenceMached == SeqCmpResult::SEQ_MATCH_COMPLETELY )
                        {
                            const auto numberCheckedBits = sequence.GetNumberCheckedBits();
                            if( numberCheckedBits > bestCompleteMatched )
                            {
                                bestCompleteMatched = numberCheckedBits;
                            }
                        }
                    }
                    if( ( sequenceMached == SeqCmpResult::SEQ_MATCH_COMPLETELY ) &&
                        ( fixedLengthSequence || ( ( bestFixedLengthCmpResult != SeqCmpResult::SEQ_MATCH_PARTIALLY ) &&
                                                   ( bestCompleteMatched > bestPartialyMatched ) ) ) )
                    {
                        if( !errorBits.Empty() )
                        {
                            errorBits.SetProtocol( dProtocol );
                            errorBits.UpdateBitInfo();
                            errorBits.AddFrames( mResults.get() );
                            mResults->CommitResults();
                            errorBits.Clear();
                        }

                        mSWDParser.CopyBits( sequence );

                        sequence.AddFrames( mResults.get() );
                        sequence.AddMarkers( mResults.get() );
                        mResults->CommitResults();

                        sequence.UpdateAdiState();

                        sequence.Clear();

                        previousFrameType = mSWDParser.GetLastFrameType();
                        dProtocol = mSWDParser.GetCurrentProtocol();
                        ClearBestMatchingResults();

                        break;
                    }
                }
            }
        }
        if( ( bestFixedLengthCmpResult == SeqCmpResult::SEQ_MISMATCH ) && ( bestVariableLengthCmpResult == SeqCmpResult::SEQ_MISMATCH ) )
        {
            // This is neither a valid transaction nor a valid sequence,
            // so collect these bits and go forward.
            errorBits.PushBackBit( mSWDParser.PopFrontBit() );
            errorBits.UpdateAdiState();
        }

        ReportProgress( mSWDIO->GetSampleNumber() );
    }
}

bool SWDAnalyzer::NeedsRerun()
{
    return false;
}

DPVersion SWDAnalyzer::GetDPVersion() const
{
    return mSWDParser.GetDPVersion();
}

void SWDAnalyzer::ClearBestMatchingResults()
{
    bestFixedLengthCmpResult = SeqCmpResult::SEQ_UNKNOWN;
	bestVariableLengthCmpResult = SeqCmpResult::SEQ_UNKNOWN;
	bestPartialyMatched = 0u;
	bestCompleteMatched = 0u;
}

U32 SWDAnalyzer::GenerateSimulationData( U64 minimumSampleIndex, U32 deviceSampleRate,
                                         SimulationChannelDescriptor** simulationChannels )
{
    if( !mSimulationInitilized )
    {
        mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), &mSettings );
        mSimulationInitilized = true;
    }

    return mSimulationDataGenerator.GenerateSimulationData( minimumSampleIndex, deviceSampleRate, simulationChannels );
}

U32 SWDAnalyzer::GetMinimumSampleRateHz()
{
    // this 1MHz limit is a little arbitrary, since the specs don't say much about the
    // valid frequency range a SWD stream should be in.
    return 1000000;
}

const char* SWDAnalyzer::GetAnalyzerName() const
{
    return ::GetAnalyzerName();
}

const char* GetAnalyzerName()
{
    return "SWD";
}

Analyzer* CreateAnalyzer()
{
    return new SWDAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
    delete analyzer;
}
