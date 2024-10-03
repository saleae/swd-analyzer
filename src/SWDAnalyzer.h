#ifndef SWD_ANALYZER_H
#define SWD_ANALYZER_H

#include <set>

#include <Analyzer.h>

#include "SWDAnalyzerSettings.h"
#include "SWDAnalyzerResults.h"
#include "SWDSimulationDataGenerator.h"

#include "SWDTypes.h"

class SWDAnalyzer : public Analyzer2
{
  public:
    SWDAnalyzer();
    virtual ~SWDAnalyzer();
    virtual void SetupResults() override;
    virtual void WorkerThread() override;

    virtual U32 GenerateSimulationData( U64 newestSampleRequested, U32 sampleRate,
                                        SimulationChannelDescriptor** simulationChannels ) override;
    virtual U32 GetMinimumSampleRateHz() override;

    virtual const char* GetAnalyzerName() const override;
    virtual bool NeedsRerun() override;

    DPVersion GetDPVersion() const;

  protected: // vars
    SWDAnalyzerSettings mSettings;
    std::unique_ptr<SWDAnalyzerResults> mResults;

    AnalyzerChannelData* mSWDIO;
    AnalyzerChannelData* mSWCLK;

    SWDSimulationDataGenerator mSimulationDataGenerator;

    SWDParser mSWDParser;

    bool mSimulationInitilized;

    // Sequence matching results
    SeqCmpResult bestFixedLengthCmpResult = SeqCmpResult::SEQ_UNKNOWN;
    SeqCmpResult bestVariableLengthCmpResult = SeqCmpResult::SEQ_UNKNOWN;
    std::size_t bestPartialyMatched = 0u;
    std::size_t bestCompleteMatched = 0u;
    void ClearBestMatchingResults();
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif // SWD_ANALYZER_H
