#include <AnalyzerHelpers.h>

#include "SWDAnalyzerSettings.h"
#include "SWDAnalyzerResults.h"
#include "SWDTypes.h"

SWDAnalyzerSettings::SWDAnalyzerSettings()
:	mSWDIO(UNDEFINED_CHANNEL),
	mSWCLK(UNDEFINED_CHANNEL)
{
	// init the interface
	mSWDIOInterface.SetTitleAndTooltip("SWDIO", "SWDIO");
	mSWDIOInterface.SetChannel(mSWDIO);

	mSWCLKInterface.SetTitleAndTooltip("SWCLK", "SWCLK");
	mSWCLKInterface.SetChannel(mSWCLK);

	// add the interface
	AddInterface(&mSWDIOInterface);
	AddInterface(&mSWCLKInterface);

	// describe export
	AddExportOption(0, "Export as text file");
	AddExportExtension(0, "text", "txt");

	ClearChannels();

	AddChannel(mSWDIO, "SWDIO", false);
	AddChannel(mSWCLK, "SWCLK", false);
}

SWDAnalyzerSettings::~SWDAnalyzerSettings()
{}

bool SWDAnalyzerSettings::SetSettingsFromInterfaces()
{
	if (mSWDIOInterface.GetChannel() == UNDEFINED_CHANNEL)
	{
		SetErrorText("Please select an input for the channel 1.");
		return false;
	}

	if (mSWCLKInterface.GetChannel() == UNDEFINED_CHANNEL)
	{
		SetErrorText("Please select an input for the channel 2.");
		return false;
	}

	mSWDIO = mSWDIOInterface.GetChannel();
	mSWCLK = mSWCLKInterface.GetChannel();

	if (mSWDIO == mSWCLK)
	{
		SetErrorText("Please select different inputs for the channels.");
		return false;
	}

	ClearChannels();

	AddChannel(mSWDIO, "SWDIO", true);
	AddChannel(mSWCLK, "SWCLK", true);

	return true;
}

void SWDAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mSWDIOInterface.SetChannel(mSWDIO);
	mSWCLKInterface.SetChannel(mSWCLK);
}

void SWDAnalyzerSettings::LoadSettings(const char* settings)
{
	SimpleArchive text_archive;
	text_archive.SetString(settings);

	text_archive >> mSWDIO;
	text_archive >> mSWCLK;

	ClearChannels();

	AddChannel(mSWDIO, "SWDIO", true);
	AddChannel(mSWCLK, "SWCLK", true);

	UpdateInterfacesFromSettings();
}

const char* SWDAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mSWDIO;
	text_archive << mSWCLK;

	return SetReturnString(text_archive.GetString());
}
