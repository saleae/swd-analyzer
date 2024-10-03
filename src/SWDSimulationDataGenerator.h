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

    void Initialize( U32 simulationSampleRate, SWDAnalyzerSettings* settings );
    U32 GenerateSimulationData( U64 newestSampleRequested, U32 sampleRate, SimulationChannelDescriptor** simulationChannels );

  protected:
    SWDAnalyzerSettings* mSettings;
    U32 mSimulationSampleRateHz;
    U32 mSimulCnt;

    void AdvanceAllBySec( double sec );

    // read and write in this context is a bit read or written from the perspective of the host
    void OutputWriteBit( BitState state );
    void OutputReadBit( BitState firstHalf, BitState secondHalf );

    void OutputTurnaround( BitState state );
    bool OutputRequest( U8 req, U8 ack, BitState firstDataBit );
    void OutputData( U32 data, bool isWrite );
    void OutputLineReset();

  protected:
    ClockGenerator mClockGenerator;

    SimulationChannelDescriptorGroup mSWDSimulationChannels;
    SimulationChannelDescriptor* mSWDIO;
    SimulationChannelDescriptor* mSWCLK;
};

#endif // SWD_SIMULATION_DATA_GENERATOR_H
