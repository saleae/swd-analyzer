#include <algorithm>

#include <AnalyzerChannelData.h>

#include "SWDAnalyzer.h"
#include "SWDAnalyzerSettings.h"
#include "SWDUtils.h"

// Sequence condition matrix
const std::vector<SWDAnalyzer::SWDSequenceCondition> SWDAnalyzer::SEQUENCE_CONDITIONS = {
    // protocol, allowed previous frames, is swdSequence compare method, swdSequence object
    // 50,... - SWD line reset. At least 50 SWCLKTCK cycles with SWDIOTMS HIGH.
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD },
      {},
      &SWDParser::IsLineReset,
      &SWDParser::GetLineReset,
      &SWDParser::SetLineReset },
    // 16 - JTAG-to-SWD select sequence
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
      { SwdFrameTypes::SWD_FT_LINE_RESET },
      &SWDParser::IsJtagToSwd,
      &SWDParser::GetJtagToSwd,
      &SWDParser::SetJtagToSwd },
    // 16 - SWD-to-JTAG select sequence
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
      { SwdFrameTypes::SWD_FT_LINE_RESET },
      &SWDParser::IsSwdToJtag,
      &SWDParser::GetSwdToJtag,
      &SWDParser::SetSwdToJtag },
    // 16 - SWD-to-DS select sequence
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
      { SwdFrameTypes::SWD_FT_LINE_RESET },
      &SWDParser::IsSwdToDs,
      &SWDParser::GetSwdToDs,
      &SWDParser::SetSwdToDs },
    // 31 - JTAG-to-DS select sequence
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
      { SwdFrameTypes::SWD_FT_LINE_RESET, SwdFrameTypes::SWD_FT_JTAG_TLR },
      &SWDParser::IsJtagToDs,
      &SWDParser::GetJtagToDs,
      &SWDParser::SetJtagToDs },
    // 12,46 - SWD read/write operation
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD },
      { SwdFrameTypes::SWD_FT_LINE_RESET, SwdFrameTypes::SWD_FT_IDLE_CYCLE, SwdFrameTypes::SWD_FT_OPERATION,
        SwdFrameTypes::SWD_FT_ERROR },
      &SWDParser::IsOperation,
      &SWDParser::GetOperation,
      &SWDParser::SetOperation },
    // 1,... - SWD idle cycles. SWCLKTCK cycles with SWDIOTMS LOW
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD },
      { SwdFrameTypes::SWD_FT_LINE_RESET, SwdFrameTypes::SWD_FT_IDLE_CYCLE, SwdFrameTypes::SWD_FT_OPERATION },
      &SWDParser::IsIdleCycles,
      &SWDParser::GetIdleCycles,
      &SWDParser::SetIdleCycles },
    // 5,0 - Enters to JTAG Test-Logic-Reset state
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
      {},
      &SWDParser::IsJtagTlr,
      &SWDParser::GetJtagTlr,
      &SWDParser::SetJtagTlr },
    // 8,... - Selection Alert preamble. At least 8 SWCLKTCK cycles with SWDIOTMS HIGH.
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT },
      {},
      &SWDParser::IsDsSelectionAlertPreamble,
      &SWDParser::GetDsSelectionAlertPreamble,
      &SWDParser::SetDsSelectionAlertPreamble },
    // 128 - Selection Alert sequence
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT },
      {},
      &SWDParser::IsDsSelectionAlert,
      &SWDParser::GetDsSelectionAlert,
      &SWDParser::SetDsSelectionAlert },
    // 4 - Activation Code preamble. 4 SWCLKTCK cycles with SWDIOTMS LOW
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT },
      { SwdFrameTypes::SWD_FT_DS_SEL_ALERT },
      &SWDParser::IsDsActivationCodePreamble,
      &SWDParser::GetDsActivationCodePreamble,
      &SWDParser::SetDsActivationCodePreamble },
    // 8,12 - Activation Code sequence
    { { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT },
      { SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE },
      &SWDParser::IsDsActivationCode,
      &SWDParser::GetDsActivationCode,
      &SWDParser::SetDsActivationCode },
};


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
    for( ;; )
    {
        bool conditionMeet = false;
        SwdFrameTypes lastFrame = mSWDParser.GetLastFrameType();
        DebugProtocol dProtocol = mSWDParser.GetCurrentProtocol();

        for( const SWDSequenceCondition& cond : SEQUENCE_CONDITIONS )
        {
            if( cond.protocols.count( dProtocol ) != 0 )
            {
                if (cond.previousFrames.empty() || (std::find(cond.previousFrames.begin(), cond.previousFrames.end(), lastFrame) != cond.previousFrames.end()))
                {
                    bool sequenceMached = std::invoke( cond.CompareFn, mSWDParser );
                    if( sequenceMached )
                    {
                        conditionMeet = true;
                        if( !errorBits.bits.empty() )
                        {
                            errorBits.SetProtocol( dProtocol );
                            errorBits.AddFrames( mResults.get() );
                            mResults->CommitResults();
                            errorBits.Clear();
                        }
                        SWDBaseSequnce& sequence = std::invoke( cond.GetSequence, mSWDParser );
                        sequence.AddFrames( mResults.get() );
                        sequence.AddMarkers( mResults.get() );

                        mResults->CommitResults();

                        std::invoke( cond.UpdateStatus, mSWDParser );
                        break;
                    }
                }
            }
        }
        if( !conditionMeet )
        {
            // This is neither a valid transaction nor a valid reset,
            // so collect these bits and go forward.
            mSWDParser.BufferBits( 1 ); // Make sure at least one bit is placed on the buffer
            errorBits.bits.push_back( mSWDParser.PopFrontBit() );
            mSWDParser.SetErrorBits();
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
