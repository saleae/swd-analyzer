#ifndef SWD_ANALYZER_H
#define SWD_ANALYZER_H

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
	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels);
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected:	// vars

	SWDAnalyzerSettings					mSettings;
	std::auto_ptr<SWDAnalyzerResults>	mResults;

	AnalyzerChannelData*				mSWDIO;
	AnalyzerChannelData*				mSWCLK;

	SWDSimulationDataGenerator			mSimulationDataGenerator;

	SWDParser							mSWDParser;

	bool	mSimulationInitilized;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer* analyzer);

#endif	// SWD_ANALYZER_H