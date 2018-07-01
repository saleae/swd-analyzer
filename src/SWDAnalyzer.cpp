#include <vector>
#include <algorithm>

#include <AnalyzerChannelData.h>

#include "SWDAnalyzer.h"
#include "SWDAnalyzerSettings.h"
#include "SWDUtils.h"

SWDAnalyzer::SWDAnalyzer()
	: mSimulationInitilized(false)
{
	SetAnalyzerSettings(&mSettings);
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
	//SetupResults();
	// get the channel pointers
	mSWDIO = GetAnalyzerChannelData( mSettings.mSWDIO );
	mSWCLK = GetAnalyzerChannelData( mSettings.mSWCLK );

	mSWDParser.Setup( mSWDIO, mSWCLK, this );

	// these are our three objects that SWDParser will fill with data
	// on calls to IsOperation or IsLineReset
	SWDOperation tran;
	SWDLineReset reset;
	SWDBit error_bit;

	mSWDParser.Clear();

	// For every new bit the parser extracts from the stream,
	// ask if this can be a valid operation or line reset.
	// A valid operation will have the constant part of the request correctly set,
	// and also the parity bits will be correct.
	// A valid line reset has at least 50 high bits in succession.
	for (;;)
	{
		if (mSWDParser.IsOperation(tran))
		{
			tran.AddFrames(mResults.get());
			tran.AddMarkers(mResults.get());

			mResults->CommitResults();

		} else if (mSWDParser.IsLineReset(reset)) {

			reset.AddFrames(mResults.get());

			mResults->CommitResults();

		} else {
			
			// This is neither a valid transaction nor a valid reset,
			// so remove the first bit and try again.
			// We're dropping the error bit into oblivion.
			error_bit = mSWDParser.PopFrontBit();
		}

		ReportProgress(mSWDIO->GetSampleNumber());
	}
}

bool SWDAnalyzer::NeedsRerun()
{
	return false;
}

U32 SWDAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels)
{
	if (!mSimulationInitilized)
	{
		mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), &mSettings);
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
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

void DestroyAnalyzer(Analyzer* analyzer)
{
	delete analyzer;
}
