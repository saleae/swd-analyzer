#ifndef SWD_SIMULATION_DATA_GENERATOR_H
#define SWD_SIMULATION_DATA_GENERATOR_H

#include <AnalyzerHelpers.h>

#include "SWDTypes.h"

class SWDAnalyzerSettings;

class SWDSimulationDataGenerator
{
public:
	SWDSimulationDataGenerator();
	~SWDSimulationDataGenerator();

	void Initialize(U32 simulation_sample_rate, SWDAnalyzerSettings* settings);
	U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels);

protected:
	SWDAnalyzerSettings*	mSettings;
	U32						mSimulationSampleRateHz;
	U32						mSimulCnt;

	void AdvanceAllBySec(double sec)
	{
		mSWDSimulationChannels.AdvanceAll(mClockGenerator.AdvanceByTimeS(sec));
	}

	// read and write in this context is a bit read or written from the perspective of the host
	void OutputWriteBit(BitState state);
	void OutputReadBit(BitState first_half, BitState second_half);

	void OutputTurnaround(BitState state);
	bool OutputRequest(U8 req, U8 ack, BitState first_data_bit);
	void OutputData(U32 data, bool is_write);
	void OutputLineReset();

protected:

	ClockGenerator mClockGenerator;

	SimulationChannelDescriptorGroup	mSWDSimulationChannels;
	SimulationChannelDescriptor*		mSWDIO;
	SimulationChannelDescriptor*		mSWCLK;
};

#endif	// SWD_SIMULATION_DATA_GENERATOR_H
