#ifndef SWD_ANALYZER_SETTINGS_H
#define SWD_ANALYZER_SETTINGS_H

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

#include "SWDTypes.h"

class SWDAnalyzerSettings : public AnalyzerSettings
{
public:
	SWDAnalyzerSettings();
	virtual ~SWDAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	virtual void LoadSettings(const char* settings);
	virtual const char* SaveSettings();

	void UpdateInterfacesFromSettings();

	Channel		mSWDIO;
	Channel		mSWCLK;

protected:
	AnalyzerSettingInterfaceChannel		mSWDIOInterface;
	AnalyzerSettingInterfaceChannel		mSWCLKInterface;
};

#endif	// SWD_ANALYZER_SETTINGS_H