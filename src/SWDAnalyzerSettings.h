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

    bool SetSettingsFromInterfaces() override;
    void LoadSettings( const char* settings ) override;
    const char* SaveSettings() override;

    void UpdateInterfacesFromSettings();

    Channel mSWDIO;
    Channel mSWCLK;

    DebugProtocol mDProtocol;
    SwdFrameTypes mLastFrame;
    DPVersion mDPVersion;
    U8 mNumTurnarounds;
    bool mOverrunDetection;
    U32 mSelectRegister;


  protected:
    AnalyzerSettingInterfaceChannel mSWDIOInterface;
    AnalyzerSettingInterfaceChannel mSWCLKInterface;
    AnalyzerSettingInterfaceNumberList mDProtocolInterface;
    AnalyzerSettingInterfaceNumberList mLastFrameInterface;
    AnalyzerSettingInterfaceNumberList mDPVersionInterface;
    AnalyzerSettingInterfaceNumberList mNumTurnaroundsInterface;
    AnalyzerSettingInterfaceBool mOverrunDetectionInterface;
    AnalyzerSettingInterfaceText mSelectRegisterInterface;
};

#endif // SWD_ANALYZER_SETTINGS_H
