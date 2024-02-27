#include <iostream>
#include <fstream>
#include <algorithm>

#include <AnalyzerHelpers.h>

#include "SWDAnalyzerResults.h"
#include "SWDAnalyzer.h"
#include "SWDAnalyzerSettings.h"
#include "SWDUtils.h"

SWDAnalyzerResults::SWDAnalyzerResults( SWDAnalyzer* analyzer, SWDAnalyzerSettings* settings )
    : mSettings( settings ), mAnalyzer( analyzer )
{
}

SWDAnalyzerResults::~SWDAnalyzerResults()
{
}

double SWDAnalyzerResults::GetSampleTime( S64 sample ) const
{
    return ( sample - mAnalyzer->GetTriggerSample() ) / double( mAnalyzer->GetSampleRate() );
}

std::string SWDAnalyzerResults::GetSampleTimeStr( S64 sample ) const
{
    char timeStr[ 128 ];
    AnalyzerHelpers::GetTimeString( sample, mAnalyzer->GetTriggerSample(), mAnalyzer->GetSampleRate(), timeStr, sizeof( timeStr ) );

    // remove trailing zeros
    size_t l = strlen( timeStr );
    if( l > 7 )
        timeStr[ l - 7 ] = '\0';

    return timeStr;
}

SWDAnalyzerSettings* SWDAnalyzerResults::GetSettings() const
{
    return mSettings;
}

void SWDAnalyzerResults::GetBubbleText( const Frame& f, DisplayBase displayBase, std::vector<std::string>& results )
{
    results.clear();

    switch( f.mType )
    {
        case static_cast<U8>(SwdFrameTypes::SWD_FT_REQUEST):
            {
                SWDRequestFrame& req( ( SWDRequestFrame& )f );

                std::string regName( req.GetRegisterName() );

                results.push_back( std::string( "Request " ) + ( req.IsAccessPort() ? " AccessPort" : " DebugPort" ) +
                           ( req.IsRead() ? " Read" : " Write" ) + " " + regName );

                results.push_back( Int2StrSal( req.mData2, displayBase ) );
                results.push_back( "rq" );
                results.push_back( "req" );
                results.push_back( "request" );
                results.push_back( std::string( "request " ) + ( req.IsAccessPort() ? "AP" : "DP" ) + ( req.IsRead() ? " R" : " W" ) + " " +
                           regName );

                results.push_back( std::string( "Request " ) + ( req.IsAccessPort() ? "AccessPort" : "DebugPort" ) +
                           ( req.IsRead() ? " Read" : " Write" ) + " " + regName );
            }
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_LINE_RESET):
            results.push_back( "Line Reset " + Int2Str( f.mData1 ) + " bits" );
            results.push_back( "rst" );
            results.push_back( "reset" );
            results.push_back( "Line Reset" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_JTAG_TLR):
            results.push_back( "JTAG Test-Logic-Reset " + Int2Str( f.mData1 ) + " bits" );
            results.push_back( "jtlr" );
            results.push_back( "j tlr" );
            results.push_back( "JTAG TLR" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_JTAG_TO_DS):
            results.push_back( "JTAG-to-DS sequence " + Int2StrSal( f.mData1, displayBase, 31 ) );
            results.push_back( "JtDS" );
            results.push_back( "JTAGtoDS" );
            results.push_back( "JTAG-to-DS" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_SWD_TO_DS):
            results.push_back( "SWD-to-DS sequence " + Int2StrSal( f.mData1, displayBase, 16 ) );
            results.push_back( "StDS" );
            results.push_back( "SWDtoDS" );
            results.push_back( "SWD-to-DS" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_JTAG_TO_SWD):
            results.push_back( std::string( f.mFlags == 0 ? "" : "Deprecated " ) + "JTAG to SWD sequence " +
                           Int2StrSal( f.mData1, displayBase, 16 ) );
            results.push_back( std::string( f.mFlags == 0 ? "" : "D" ) + "JtS" );
            results.push_back( std::string( f.mFlags == 0 ? "" : "Depr" ) + "JTAGtoSWD" );
            results.push_back( std::string( f.mFlags == 0 ? "" : "Deprecated " ) + "JTAG to SWD " +
                            Int2StrSal( f.mData1, displayBase, 16 ) );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_SWD_TO_JTAG):
            results.push_back( std::string( f.mFlags == 0 ? "" : "Deprecated " ) + "SWD to JTAG sequence " +
                           Int2StrSal( f.mData1, displayBase, 16 ) );
            results.push_back( std::string( f.mFlags == 0 ? "" : "D" ) + "StJ" );
            results.push_back( std::string( f.mFlags == 0 ? "" : "Depr" ) + "SWDtoJTAG" );
            results.push_back( std::string( f.mFlags == 0 ? "" : "Deprecated " ) + "SWD to JTAG " + Int2StrSal( f.mData1, displayBase, 16 ) );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_TURNAROUND):
            results.push_back( "Turnaround" );
            results.push_back( "T" );
            results.push_back( "trn" );
            results.push_back( "turn" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_ACK):
            if( f.mData1 == static_cast<U64>( SWDAcks::ACK_OK ) )
            {
                results.push_back( "ACK OK" );
                results.push_back( "OK" );
            }
            else if( f.mData1 == static_cast<U64>( SWDAcks::ACK_WAIT ) )
            {
                results.push_back( "ACK WAIT" );
                results.push_back( "WAIT" );
            }
            else if( f.mData1 == static_cast<U64>( SWDAcks::ACK_FAULT ) )
            {
                results.push_back( "ACK FAULT" );
                results.push_back( "FAULT" );
            }
            else
            {
                results.push_back( "ACK <unknown> probably disconnected" );
                results.push_back( "ACK <unknown>" );
                results.push_back( "disc" );
            }
            results.push_back( "ACK" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_WDATA):
        case static_cast<U8>(SwdFrameTypes::SWD_FT_RDATA):
            {
                std::string dataStr( Int2StrSal( f.mData1, displayBase, 32 ) );
                std::string regName;
                std::string regValue;
                SWDRegistersUnion swdRegCouple; // Couple of previous and current SWD register
                swdRegCouple.blob = f.mData2;

                if( swdRegCouple.reg.prev == SWDRegisters::SWDR_UNDEFINED )
                {
                    // DP Read except READBUFF, DP Write, AP Write
                    regName = GetRegisterName( swdRegCouple.reg.current );
                    regValue = GetRegisterValueDesc( swdRegCouple.reg.current, static_cast<U32>( f.mData1 ), displayBase, mAnalyzer->GetDPVersion() );
                }
                else
                {
                    if( swdRegCouple.reg.current == SWDRegisters::SWDR_UNDEFINED )
                    {
                        // First AP Read
                        regName = GetRegisterName( swdRegCouple.reg.prev );
                        regValue = "Ignored AP first read data";
                    }
                    else
                    {
                        // Next AP Reads, DP Read of READBUFF
                        regName = GetRegisterName( swdRegCouple.reg.current );
                        regValue = GetRegisterValueDesc( swdRegCouple.reg.prev, static_cast<U32>( f.mData1 ), displayBase, mAnalyzer->GetDPVersion() );
                    }
                }

                std::string prefix = ( f.mType == static_cast<U8>(SwdFrameTypes::SWD_FT_WDATA ) ) ? "WData" : "RData";
                if( !regValue.empty() )
                {
                    results.push_back( prefix + " " + dataStr + " reg " + regName + " bits " + regValue );
                }
                results.push_back( prefix + " " + dataStr + " reg " + regName );
                results.push_back( prefix );
                results.push_back( prefix + " " + dataStr );
            }
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_DATA_PARITY):
            results.push_back( std::string( "Data parity" ) + ( f.mData2 ? "ok" : "NOT OK" ) );
            results.push_back( f.mData1 ? "1" : "0" );
            results.push_back( "prty" );
            results.push_back( "Parity" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_IDLE_CYCLE):
            results.push_back( Int2Str( f.mData1 ) + " Idle Cycles" );
            results.push_back( "idl" );
            results.push_back( "idle" );
            results.push_back( "Idle Cycles" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_ERROR):
            {
                std::string protocolName( "" );
                if( f.mData2 == static_cast<U64>( DebugProtocol::DPROTOCOL_SWD ) )
                {
                    protocolName += "SWD ";
                }
                results.push_back( "Erroneus " + protocolName + Int2Str( f.mData1 ) + " bits" );
                results.push_back( "e" );
                results.push_back( "err" );
                results.push_back( "Erroneus " + protocolName + "bits" );
            }
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_IGNORED):
            {
                std::string protocolName( "" );
                if( f.mData2 == static_cast<U64>( DebugProtocol::DPROTOCOL_DORMANT ) )
                {
                    protocolName += "Dormant ";
                }
                else
                {
                    if( f.mData2 == static_cast<U64>( DebugProtocol::DPROTOCOL_JTAG ) )
                    {
                        protocolName += "JTAG ";
                    }
                }
                results.push_back( "Ignored " + protocolName + Int2Str( f.mData1 ) + " bits" );
                results.push_back( "i" );
                results.push_back( "ign" );
                results.push_back( "Ignored " + protocolName + "bits" );
            }
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_DS_SEL_ALERT_PREAMBLE):
            results.push_back( "DS Selection Alert sequence preamble " + Int2Str( f.mData1 ) + " bits" );
            results.push_back( "dssap" );
            results.push_back( "ds sap" );
            results.push_back( "DS SA preamble" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_DS_SEL_ALERT):
            results.push_back( "DS Selection Alert sequence " + Int2StrSal( f.mData1, displayBase, 64 ) + " " +
                               Int2StrSal( f.mData2, displayBase, 31 ) );
            results.push_back( "dssa" );
            results.push_back( "ds sa" );
            results.push_back( "DS SA" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE):
            results.push_back( "DS Activation code sequence preamble " + Int2Str( f.mData1 ) + " bits" );
            results.push_back( "dsacp" );
            results.push_back( "ds acp" );
            results.push_back( "DS AC preamble" );
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE):
            {
                std::string protocolName;
                switch( f.mData1 )
                {
                    case 0x00u:
                    case 0x0Au:
                        protocolName = "JTAG";
                        break;
                    case 0x1Au:
                        protocolName = "SWD";
                        break;
                    default:
                        protocolName = "Unknown";
                        break;
                }
                results.push_back( "DS Activation code sequence " + Int2StrSal( f.mData1, displayBase, 8 ) + " (" + protocolName + ")" );
                results.push_back( "dsac" );
                results.push_back( "ds ac" );
                results.push_back( "DS AC" );
            }
            break;
        case static_cast<U8>(SwdFrameTypes::SWD_FT_BIT):
            {
                std::string msg;
                msg = "bit " + Int2Str( f.mData2 );
                results.push_back( msg );
            }
            break;
        default:
            break;
    }
}

void SWDAnalyzerResults::GenerateBubbleText( U64 frameIndex, Channel& channel, DisplayBase displayBase )
{
    ClearResultStrings();
    Frame f = GetFrame( frameIndex );

    std::vector<std::string> results;
    GetBubbleText( f, displayBase, results );

    for( std::vector<std::string>::iterator ri( results.begin() ); ri != results.end(); ++ri )
        AddResultString( ri->c_str() );
}

#define EXP_RECORD_FIELDS 9

void SaveRecord( std::vector<std::string>& rec, std::ofstream& of )
{
    if( rec.empty() )
        return;

    while( rec.size() < EXP_RECORD_FIELDS )
        rec.push_back( "" );

    for( std::vector<std::string>::iterator ri( rec.begin() ); ri != rec.end(); ++ri )
    {
        if( ri != rec.begin() )
            of << "\t";

        of << *ri;
    }

    of << std::endl;

    rec.clear();
}

void SWDAnalyzerResults::GenerateExportFile( const char* file, DisplayBase displayBase, U32 exportTypeUserId )
{
    std::ofstream of( file, std::ios::out );

    of << "Time\tType\tR/W\tAP/DP\tRegister\tRequest byte\tACK\tData\tData details" << std::endl;

    Frame f;
    const U64 numFrames = GetNumFrames();
    std::vector<std::string> record;
    for( U64 fcnt = 0; fcnt < numFrames; fcnt++ )
    {
        // get the frame
        f = GetFrame( fcnt );

        switch( f.mType )
        {
            case static_cast<U8>( SwdFrameTypes::SWD_FT_ERROR ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "Erroneus " + Int2Str( f.mData1 ) + " bits" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_IGNORED ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "Ignored " + Int2Str( f.mData1 ) + " bits" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_BIT ):
                // Skip this type of frame
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_LINE_RESET ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "Line reset" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_JTAG_TO_SWD ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "JTAG to SWD Sequence" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_SWD_TO_JTAG ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "SWD to JTAG Sequence" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_IDLE_CYCLE ):
                // Skip this type of frame
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_JTAG_TLR ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "JTAG Test-Logic-Reset " + Int2Str( f.mData1 ) + " bits" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_JTAG_TO_DS ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "JTAG-to-DS sequence " + Int2StrSal( f.mData1, displayBase, 31 ) );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_SWD_TO_DS ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "SWD-to-DS sequence " + Int2StrSal( f.mData1, displayBase, 31 ) );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_DS_SEL_ALERT_PREAMBLE ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "DS Selection Alert sequence preamble " + Int2Str( f.mData1 ) + " bits" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_DS_SEL_ALERT ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "DS Selection Alert sequence " + Int2StrSal( f.mData1, displayBase, 64 ) + " " + Int2StrSal( f.mData2, displayBase, 31 ) );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "DS Activation code sequence preamble " + Int2Str( f.mData1 ) + " bits" );
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                {
                    std::string protocolName;
                    switch( f.mData1 )
                    {
                    case 0x00u:
                    case 0x0Au:
                        protocolName = "JTAG";
                        break;
                    case 0x1Au:
                        protocolName = "SWD";
                        break;
                    default:
                        protocolName = "Unknown";
                        break;
                    }
                    record.push_back( "DS Activation code sequence " + Int2StrSal( f.mData1, displayBase, 8 ) + " (" + protocolName + ")" );
                }
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_OPERATION ):
                // Skip this type of frame it will have parsed as subframes: SWD_FT_REQUEST, SWD_FT_ACK, SWD_FT_RDATA/SWD_FT_WDATA
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_REQUEST ):
                SaveRecord( record, of );
                record.push_back( GetSampleTimeStr( f.mStartingSampleInclusive ) );
                record.push_back( "Operation" );
                record.push_back( static_cast<SWDRequestFrame&>( f ).IsRead() ? "read" : "write" );
                record.push_back( static_cast<SWDRequestFrame&>( f ).IsAccessPort() ? "AccessPort" : "DebugPort" );
                record.push_back( static_cast<SWDRequestFrame&>( f ).GetRegisterName() );
                record.push_back( Int2StrSal( f.mData1, displayBase, 8 ) );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_TURNAROUND ):
                // Skip this type of frame
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_ACK ):
                if( f.mData1 == static_cast<U64>( SWDAcks::ACK_OK ) )
                    record.push_back( "OK" );
                else if( f.mData1 == static_cast<U64>( SWDAcks::ACK_WAIT ) )
                    record.push_back( "WAIT" );
                else if( f.mData1 == static_cast<U64>( SWDAcks::ACK_FAULT ) )
                    record.push_back( "FAULT" );
                else
                    record.push_back( "<disc>" );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_WDATA ):
            case static_cast<U8>( SwdFrameTypes::SWD_FT_RDATA ):
                record.push_back( Int2StrSal( f.mData1, displayBase, 32 ) );
                {
                    SWDRegistersUnion swdRegCouple; // Couple of previous and current SWD register
                    swdRegCouple.blob = f.mData2;
                    std::string regValue;

                    if( swdRegCouple.reg.prev == SWDRegisters::SWDR_UNDEFINED )
                    {
                        // DP Read except READBUFF, DP Write, AP Write
                        regValue = GetRegisterValueDesc( swdRegCouple.reg.current, static_cast<U32>( f.mData1 ), displayBase, mAnalyzer->GetDPVersion() );
                    }
                    else
                    {
                        if( swdRegCouple.reg.current == SWDRegisters::SWDR_UNDEFINED )
                        {
                            // First AP Read
                            regValue = "Ignored AP first read data";
                        }
                        else
                        {
                            // Next AP Reads, DP Read of READBUFF
                            regValue = GetRegisterValueDesc( swdRegCouple.reg.prev, static_cast<U32>( f.mData1 ), displayBase, mAnalyzer->GetDPVersion() );
                        }
                    }
                    record.push_back( regValue );
                }
                SaveRecord( record, of );
                break;
            case static_cast<U8>( SwdFrameTypes::SWD_FT_DATA_PARITY ):
                // Skip this type of frame
                break;

            default:
                break;
        }

        if( UpdateExportProgressAndCheckForCancel( fcnt, numFrames ) )
            return;
    }

    UpdateExportProgressAndCheckForCancel( numFrames, numFrames );
}

void SWDAnalyzerResults::GenerateFrameTabularText( U64 frameIndex, DisplayBase displayBase )
{
    ClearTabularText();

    std::vector<std::string> results;
    Frame f = GetFrame( frameIndex );
    GetBubbleText( f, displayBase, results );

    if( !results.empty() )
        AddTabularText( results.front().c_str() );
}

void SWDAnalyzerResults::GeneratePacketTabularText( U64 packetId, DisplayBase displayBase )
{
    ClearResultStrings();
    AddResultString( "not supported" );
}

void SWDAnalyzerResults::GenerateTransactionTabularText( U64 transactionId, DisplayBase displayBase )
{
    ClearResultStrings();
    AddResultString( "not supported" );
}
