#include <AnalyzerHelpers.h>

#include "SWDAnalyzerSettings.h"
#include "SWDAnalyzerResults.h"
#include "SWDTypes.h"
#include <sstream>

SWDAnalyzerSettings::SWDAnalyzerSettings()
    : mSWDIO( UNDEFINED_CHANNEL ),
      mSWCLK( UNDEFINED_CHANNEL ),
      mDProtocol( DebugProtocol::DPROTOCOL_UNKNOWN ),
      mLastFrame( SwdFrameTypes::SWD_FT_LINE_RESET ),
      mDPVersion( DPVersion::DP_V0 ),
      mNumTurnarounds( 1u ),
      mOverrunDetection( false ),
      mSelectRegister( 0x0u )
{
    // init the interface
    mSWDIOInterface.SetTitleAndTooltip( "SWDIO", "SWDIO" );
    mSWDIOInterface.SetChannel( mSWDIO );

    mSWCLKInterface.SetTitleAndTooltip( "SWCLK", "SWCLK" );
    mSWCLKInterface.SetChannel( mSWCLK );

    mDProtocolInterface.SetTitleAndTooltip( "Initial protocol", "Start parsing with the assumption that the current debug protocol is" );
    mDProtocolInterface.AddNumber( static_cast<double>( DebugProtocol::DPROTOCOL_UNKNOWN ), "Unknown", "" );
    mDProtocolInterface.AddNumber( static_cast<double>( DebugProtocol::DPROTOCOL_DORMANT ), "Dormant", "" );
    mDProtocolInterface.AddNumber( static_cast<double>( DebugProtocol::DPROTOCOL_JTAG ), "JTAG", "" );
    mDProtocolInterface.AddNumber( static_cast<double>( DebugProtocol::DPROTOCOL_SWD ), "SWD", "" );
    mDProtocolInterface.SetNumber( static_cast<double>( mDProtocol ) );

    mLastFrameInterface.SetTitleAndTooltip( "Assumed previous frame", "Start parsing with the assumption that the previous frame is" );
    for( U32 i = static_cast<U32>( SwdFrameTypes::SWD_FT_LINE_RESET ); i <= static_cast<U32>( SwdFrameTypes::SWD_FT_OPERATION ); ++i )
    {
        mLastFrameInterface.AddNumber( i, SWDFrame::GetSwdFrameName( static_cast<SwdFrameTypes>( i ) ).c_str(), "" );
    }
    mLastFrameInterface.SetNumber( static_cast<double>( mLastFrame ) );

    mDPVersionInterface.SetTitleAndTooltip( "Assumed DP version", "Start parsing with the assumption that the DP version is" );
    mDPVersionInterface.AddNumber( static_cast<double>( DPVersion::DP_V1 ), "v1", "ADIv5 ARM IHI 0031A" );
    mDPVersionInterface.AddNumber( static_cast<double>( DPVersion::DP_V2 ), "v2", "ADIv5.2 ARM IHI 0031C" );
    mDPVersionInterface.AddNumber( static_cast<double>( DPVersion::DP_V3 ), "v3", "ADIv6 ARM IHI 0074D" );
    mDPVersionInterface.SetNumber( static_cast<double>( mDPVersion ) );

    mNumTurnaroundsInterface.SetTitleAndTooltip( "Assumed DLCR.TURNROUND", "Start parsing with the assumption that the turnaround tristate period is" );
    mNumTurnaroundsInterface.AddNumber( 1, "1", "1 data period" );
    mNumTurnaroundsInterface.AddNumber( 2, "2", "2 data period" );
    mNumTurnaroundsInterface.AddNumber( 3, "3", "3 data period" );
    mNumTurnaroundsInterface.AddNumber( 4, "4", "4 data period" );
    mNumTurnaroundsInterface.SetNumber( mNumTurnarounds );

    mOverrunDetectionInterface.SetTitleAndTooltip( "Assumed CTRL/STAT.ORUNDETECT", "Start parsing with the assumption that the ORUNDETECT bit of DP CTRL/STAT register is" );
    mOverrunDetectionInterface.SetValue( mOverrunDetection );

    mSelectRegisterInterface.SetTitleAndTooltip( "Assumed SELECT", "Start parsing with the assumption that the SELECT register is" );
    mSelectRegisterInterface.SetTextType( AnalyzerSettingInterfaceText::NormalText );
    {
        std::stringstream ss;
        ss << "0x" << std::hex << mSelectRegister;
        mSelectRegisterInterface.SetText( ss.str().c_str() );
    }

    // add the interface
    AddInterface( &mSWDIOInterface );
    AddInterface( &mSWCLKInterface );
    AddInterface( &mDProtocolInterface );
    AddInterface( &mLastFrameInterface );
    AddInterface( &mDPVersionInterface );
    AddInterface( &mNumTurnaroundsInterface );
    AddInterface( &mOverrunDetectionInterface );
    AddInterface( &mSelectRegisterInterface );

    // describe export
    AddExportOption( 0, "Export as text file" );
    AddExportExtension( 0, "text", "txt" );

    ClearChannels();

    AddChannel( mSWDIO, "SWDIO", false );
    AddChannel( mSWCLK, "SWCLK", false );
}

SWDAnalyzerSettings::~SWDAnalyzerSettings()
{
}

bool SWDAnalyzerSettings::SetSettingsFromInterfaces()
{
    if( mSWDIOInterface.GetChannel() == UNDEFINED_CHANNEL )
    {
        SetErrorText( "Please select an input for the channel 1." );
        return false;
    }

    if( mSWCLKInterface.GetChannel() == UNDEFINED_CHANNEL )
    {
        SetErrorText( "Please select an input for the channel 2." );
        return false;
    }

    mSWDIO = mSWDIOInterface.GetChannel();
    mSWCLK = mSWCLKInterface.GetChannel();

    if( mSWDIO == mSWCLK )
    {
        SetErrorText( "Please select different inputs for the channels." );
        return false;
    }

    ClearChannels();

    AddChannel( mSWDIO, "SWDIO", true );
    AddChannel( mSWCLK, "SWCLK", true );

    mDProtocol = static_cast<DebugProtocol>( mDProtocolInterface.GetNumber() );
    mLastFrame = static_cast<SwdFrameTypes>( mLastFrameInterface.GetNumber() );
    mDPVersion = static_cast<DPVersion>( mDPVersionInterface.GetNumber() );
    mNumTurnarounds = static_cast<U8>( mNumTurnaroundsInterface.GetNumber() );
    mOverrunDetection = mOverrunDetectionInterface.GetValue();
    {
        std::stringstream ss;
        ss << std::hex << mSelectRegisterInterface.GetText();
        ss >> mSelectRegister;
    }

    return true;
}

void SWDAnalyzerSettings::UpdateInterfacesFromSettings()
{
    mSWDIOInterface.SetChannel( mSWDIO );
    mSWCLKInterface.SetChannel( mSWCLK );

    mDProtocolInterface.SetNumber( static_cast<double>( mDProtocol ) );
    mLastFrameInterface.SetNumber( static_cast<double>( mLastFrame ) );
    mDPVersionInterface.SetNumber( static_cast<double>( mDPVersion ) );
    mNumTurnaroundsInterface.SetNumber( mNumTurnarounds );
    mOverrunDetectionInterface.SetValue( mOverrunDetection );
    {
        std::stringstream ss;
        ss << "0x" << std::hex << mSelectRegister;
        mSelectRegisterInterface.SetText( ss.str().c_str() );
    }
}

void SWDAnalyzerSettings::LoadSettings( const char* settings )
{
    SimpleArchive textArchive;
    textArchive.SetString( settings );

    textArchive >> mSWDIO;
    textArchive >> mSWCLK;

    U32 tmp;
    textArchive >> tmp;
    mDProtocol = static_cast<DebugProtocol>( tmp );
    textArchive >> tmp;
    mLastFrame = static_cast<SwdFrameTypes>( tmp );
    textArchive >> tmp;
    mDPVersion = static_cast<DPVersion>( tmp );
    textArchive >> tmp;
    mNumTurnarounds = static_cast<U8>( tmp );
    textArchive >> mOverrunDetection;
    textArchive >> mSelectRegister;

    ClearChannels();

    AddChannel( mSWDIO, "SWDIO", true );
    AddChannel( mSWCLK, "SWCLK", true );

    UpdateInterfacesFromSettings();
}

const char* SWDAnalyzerSettings::SaveSettings()
{
    SimpleArchive textArchive;

    textArchive << mSWDIO;
    textArchive << mSWCLK;

    U32 tmp;
    tmp = static_cast<U32>( mDProtocol );
    textArchive << tmp;
    tmp = static_cast<U32>( mLastFrame );
    textArchive << tmp;
    tmp = static_cast<U32>( mDPVersion );
    textArchive << tmp;
    tmp = static_cast<U32>( mNumTurnarounds );
    textArchive << tmp;
    textArchive << mOverrunDetection;
    textArchive << mSelectRegister;

    return SetReturnString( textArchive.GetString() );
}
