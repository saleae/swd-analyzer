#ifndef SWD_TYPES_H
#define SWD_TYPES_H

#include <deque>
#include <map>
#include <set>

#include <LogicPublicTypes.h>
#include <AnalyzerChannelData.h>

#include "SWDAnalyzerResults.h"

// Current Debug Protocol
enum class DebugProtocol
{
    DPROTOCOL_UNKNOWN,
    DPROTOCOL_DORMANT,
    DPROTOCOL_JTAG,
    DPROTOCOL_SWD
};

// the possible frame types
enum class SwdFrameTypes : U8
{
    SWD_FT_ERROR,
    SWD_FT_IGNORED,
    SWD_FT_BIT,

    SWD_FT_LINE_RESET,
    SWD_FT_JTAG_TO_SWD,
    SWD_FT_SWD_TO_JTAG,
    SWD_FT_IDLE_CYCLE,
    SWD_FT_JTAG_TLR,
    SWD_FT_JTAG_TO_DS,
    SWD_FT_SWD_TO_DS,
    SWD_FT_DS_SEL_ALERT_PREAMBLE,
    SWD_FT_DS_SEL_ALERT,
    SWD_FT_DS_ACTIVATION_CODE_PREAMBLE,
    SWD_FT_DS_ACTIVATION_CODE,

    SWD_FT_OPERATION,
    SWD_FT_REQUEST,
    SWD_FT_TURNAROUND,
    SWD_FT_ACK,
    SWD_FT_WDATA,
    SWD_FT_RDATA,
    SWD_FT_DATA_PARITY
};

class SWDFrame
{
    static const std::map<SwdFrameTypes, std::string> FRAME_NAMES;
    static const std::map<SwdFrameTypes, std::string> FRAMEV2_NAMES;

  public:
    static const std::string& GetSwdFrameName( SwdFrameTypes frame );
    static const std::string& GetSwdFrameV2Name( SwdFrameTypes frame );
};

// Version of the DP architecture implemented
enum class DPVersion
{
    DP_V0 = 0, // JTAG-DP conforms ADIv5.0
    DP_V1 = 1, // SW-DP conforms ADIv5.0
    DP_V2 = 2, // SW-DP version 2 conforms ADIv5.2
    DP_V3 = 3, // SW-DP version 3 conforms ADIv6
};

// the DebugPort and AccessPort registers as defined by SWD
enum class SWDRegisters : U16
{
    SWDR_UNDEFINED,

    // DPv1
    SWDR_DP_ABORT,
    SWDR_DP_DPIDR,
    SWDR_DP_CTRL_STAT,
    SWDR_DP_SELECT,
    SWDR_DP_RDBUFF,
    SWDR_DP_DLCR,
    SWDR_DP_RESEND,

    // DPv2
    SWDR_DP_TARGETID,
    SWDR_DP_DLPIDR,
    SWDR_DP_EVENTSTAT,
    SWDR_DP_TARGETSEL,

    // DPv3
    SWDR_DP_DPIDR1,
    SWDR_DP_BASEPTR0,
    SWDR_DP_BASEPTR1,
    SWDR_DP_SELECT1,

    // MEM-AP ADIv5
    SWDR_AP_CSW,
    SWDR_AP_TAR,
    SWDR_AP_TAR_MSW,
    SWDR_AP_DRW,
    SWDR_AP_BD0,
    SWDR_AP_BD1,
    SWDR_AP_BD2,
    SWDR_AP_BD3,
    SWDR_AP_MBT,
    SWDR_AP_BASE_MSW,
    SWDR_AP_CFG,
    SWDR_AP_BASE,
    SWDR_AP_IDR,
    SWDR_AP_RAZ_WI,

    // MEM-APv2 ADIv6
    SWDR_AP_DAR0,
    SWDR_AP_DAR1,
    SWDR_AP_DAR2,
    SWDR_AP_DAR3,
    SWDR_AP_DAR4,
    SWDR_AP_DAR5,
    SWDR_AP_DAR6,
    SWDR_AP_DAR7,
    SWDR_AP_DAR8,
    SWDR_AP_DAR9,
    SWDR_AP_DAR10,
    SWDR_AP_DAR11,
    SWDR_AP_DAR12,
    SWDR_AP_DAR13,
    SWDR_AP_DAR14,
    SWDR_AP_DAR15,
    SWDR_AP_DAR16,
    SWDR_AP_DAR17,
    SWDR_AP_DAR18,
    SWDR_AP_DAR19,
    SWDR_AP_DAR20,
    SWDR_AP_DAR21,
    SWDR_AP_DAR22,
    SWDR_AP_DAR23,
    SWDR_AP_DAR24,
    SWDR_AP_DAR25,
    SWDR_AP_DAR26,
    SWDR_AP_DAR27,
    SWDR_AP_DAR28,
    SWDR_AP_DAR29,
    SWDR_AP_DAR30,
    SWDR_AP_DAR31,
    SWDR_AP_DAR32,
    SWDR_AP_DAR33,
    SWDR_AP_DAR34,
    SWDR_AP_DAR35,
    SWDR_AP_DAR36,
    SWDR_AP_DAR37,
    SWDR_AP_DAR38,
    SWDR_AP_DAR39,
    SWDR_AP_DAR40,
    SWDR_AP_DAR41,
    SWDR_AP_DAR42,
    SWDR_AP_DAR43,
    SWDR_AP_DAR44,
    SWDR_AP_DAR45,
    SWDR_AP_DAR46,
    SWDR_AP_DAR47,
    SWDR_AP_DAR48,
    SWDR_AP_DAR49,
    SWDR_AP_DAR50,
    SWDR_AP_DAR51,
    SWDR_AP_DAR52,
    SWDR_AP_DAR53,
    SWDR_AP_DAR54,
    SWDR_AP_DAR55,
    SWDR_AP_DAR56,
    SWDR_AP_DAR57,
    SWDR_AP_DAR58,
    SWDR_AP_DAR59,
    SWDR_AP_DAR60,
    SWDR_AP_DAR61,
    SWDR_AP_DAR62,
    SWDR_AP_DAR63,
    SWDR_AP_DAR64,
    SWDR_AP_DAR65,
    SWDR_AP_DAR66,
    SWDR_AP_DAR67,
    SWDR_AP_DAR68,
    SWDR_AP_DAR69,
    SWDR_AP_DAR70,
    SWDR_AP_DAR71,
    SWDR_AP_DAR72,
    SWDR_AP_DAR73,
    SWDR_AP_DAR74,
    SWDR_AP_DAR75,
    SWDR_AP_DAR76,
    SWDR_AP_DAR77,
    SWDR_AP_DAR78,
    SWDR_AP_DAR79,
    SWDR_AP_DAR80,
    SWDR_AP_DAR81,
    SWDR_AP_DAR82,
    SWDR_AP_DAR83,
    SWDR_AP_DAR84,
    SWDR_AP_DAR85,
    SWDR_AP_DAR86,
    SWDR_AP_DAR87,
    SWDR_AP_DAR88,
    SWDR_AP_DAR89,
    SWDR_AP_DAR90,
    SWDR_AP_DAR91,
    SWDR_AP_DAR92,
    SWDR_AP_DAR93,
    SWDR_AP_DAR94,
    SWDR_AP_DAR95,
    SWDR_AP_DAR96,
    SWDR_AP_DAR97,
    SWDR_AP_DAR98,
    SWDR_AP_DAR99,
    SWDR_AP_DAR100,
    SWDR_AP_DAR101,
    SWDR_AP_DAR102,
    SWDR_AP_DAR103,
    SWDR_AP_DAR104,
    SWDR_AP_DAR105,
    SWDR_AP_DAR106,
    SWDR_AP_DAR107,
    SWDR_AP_DAR108,
    SWDR_AP_DAR109,
    SWDR_AP_DAR110,
    SWDR_AP_DAR111,
    SWDR_AP_DAR112,
    SWDR_AP_DAR113,
    SWDR_AP_DAR114,
    SWDR_AP_DAR115,
    SWDR_AP_DAR116,
    SWDR_AP_DAR117,
    SWDR_AP_DAR118,
    SWDR_AP_DAR119,
    SWDR_AP_DAR120,
    SWDR_AP_DAR121,
    SWDR_AP_DAR122,
    SWDR_AP_DAR123,
    SWDR_AP_DAR124,
    SWDR_AP_DAR125,
    SWDR_AP_DAR126,
    SWDR_AP_DAR127,
    SWDR_AP_DAR128,
    SWDR_AP_DAR129,
    SWDR_AP_DAR130,
    SWDR_AP_DAR131,
    SWDR_AP_DAR132,
    SWDR_AP_DAR133,
    SWDR_AP_DAR134,
    SWDR_AP_DAR135,
    SWDR_AP_DAR136,
    SWDR_AP_DAR137,
    SWDR_AP_DAR138,
    SWDR_AP_DAR139,
    SWDR_AP_DAR140,
    SWDR_AP_DAR141,
    SWDR_AP_DAR142,
    SWDR_AP_DAR143,
    SWDR_AP_DAR144,
    SWDR_AP_DAR145,
    SWDR_AP_DAR146,
    SWDR_AP_DAR147,
    SWDR_AP_DAR148,
    SWDR_AP_DAR149,
    SWDR_AP_DAR150,
    SWDR_AP_DAR151,
    SWDR_AP_DAR152,
    SWDR_AP_DAR153,
    SWDR_AP_DAR154,
    SWDR_AP_DAR155,
    SWDR_AP_DAR156,
    SWDR_AP_DAR157,
    SWDR_AP_DAR158,
    SWDR_AP_DAR159,
    SWDR_AP_DAR160,
    SWDR_AP_DAR161,
    SWDR_AP_DAR162,
    SWDR_AP_DAR163,
    SWDR_AP_DAR164,
    SWDR_AP_DAR165,
    SWDR_AP_DAR166,
    SWDR_AP_DAR167,
    SWDR_AP_DAR168,
    SWDR_AP_DAR169,
    SWDR_AP_DAR170,
    SWDR_AP_DAR171,
    SWDR_AP_DAR172,
    SWDR_AP_DAR173,
    SWDR_AP_DAR174,
    SWDR_AP_DAR175,
    SWDR_AP_DAR176,
    SWDR_AP_DAR177,
    SWDR_AP_DAR178,
    SWDR_AP_DAR179,
    SWDR_AP_DAR180,
    SWDR_AP_DAR181,
    SWDR_AP_DAR182,
    SWDR_AP_DAR183,
    SWDR_AP_DAR184,
    SWDR_AP_DAR185,
    SWDR_AP_DAR186,
    SWDR_AP_DAR187,
    SWDR_AP_DAR188,
    SWDR_AP_DAR189,
    SWDR_AP_DAR190,
    SWDR_AP_DAR191,
    SWDR_AP_DAR192,
    SWDR_AP_DAR193,
    SWDR_AP_DAR194,
    SWDR_AP_DAR195,
    SWDR_AP_DAR196,
    SWDR_AP_DAR197,
    SWDR_AP_DAR198,
    SWDR_AP_DAR199,
    SWDR_AP_DAR200,
    SWDR_AP_DAR201,
    SWDR_AP_DAR202,
    SWDR_AP_DAR203,
    SWDR_AP_DAR204,
    SWDR_AP_DAR205,
    SWDR_AP_DAR206,
    SWDR_AP_DAR207,
    SWDR_AP_DAR208,
    SWDR_AP_DAR209,
    SWDR_AP_DAR210,
    SWDR_AP_DAR211,
    SWDR_AP_DAR212,
    SWDR_AP_DAR213,
    SWDR_AP_DAR214,
    SWDR_AP_DAR215,
    SWDR_AP_DAR216,
    SWDR_AP_DAR217,
    SWDR_AP_DAR218,
    SWDR_AP_DAR219,
    SWDR_AP_DAR220,
    SWDR_AP_DAR221,
    SWDR_AP_DAR222,
    SWDR_AP_DAR223,
    SWDR_AP_DAR224,
    SWDR_AP_DAR225,
    SWDR_AP_DAR226,
    SWDR_AP_DAR227,
    SWDR_AP_DAR228,
    SWDR_AP_DAR229,
    SWDR_AP_DAR230,
    SWDR_AP_DAR231,
    SWDR_AP_DAR232,
    SWDR_AP_DAR233,
    SWDR_AP_DAR234,
    SWDR_AP_DAR235,
    SWDR_AP_DAR236,
    SWDR_AP_DAR237,
    SWDR_AP_DAR238,
    SWDR_AP_DAR239,
    SWDR_AP_DAR240,
    SWDR_AP_DAR241,
    SWDR_AP_DAR242,
    SWDR_AP_DAR243,
    SWDR_AP_DAR244,
    SWDR_AP_DAR245,
    SWDR_AP_DAR246,
    SWDR_AP_DAR247,
    SWDR_AP_DAR248,
    SWDR_AP_DAR249,
    SWDR_AP_DAR250,
    SWDR_AP_DAR251,
    SWDR_AP_DAR252,
    SWDR_AP_DAR253,
    SWDR_AP_DAR254,
    SWDR_AP_DAR255,
    SWDR_AP_TRR,
    SWDR_AP_T0TR,
    SWDR_AP_CFG1,
    SWDR_AP_ITCTRL,
    SWDR_AP_CLAIMSET,
    SWDR_AP_CLAIMCLR,
    SWDR_AP_DEVAFF0,
    SWDR_AP_DEVAFF1,
    SWDR_AP_LAR,
    SWDR_AP_LSR,
    SWDR_AP_AUTHSTATUS,
    SWDR_AP_DEVARCH,
    SWDR_AP_DEVID2,
    SWDR_AP_DEVID1,
    SWDR_AP_DEVID,
    SWDR_AP_DEVTYPE,
    SWDR_AP_PIDR4,
    SWDR_AP_PIDR5,
    SWDR_AP_PIDR6,
    SWDR_AP_PIDR7,
    SWDR_AP_PIDR0,
    SWDR_AP_PIDR1,
    SWDR_AP_PIDR2,
    SWDR_AP_PIDR3,
    SWDR_AP_CIDR0,
    SWDR_AP_CIDR1,
    SWDR_AP_CIDR2,
    SWDR_AP_CIDR3
};

// some ACK values
enum class SWDAcks : U8
{
    ACK_OK = 1,
    ACK_WAIT = 2,
    ACK_FAULT = 4
};
bool operator==( const U8 a, const SWDAcks b );
bool operator!=( const U8 a, const SWDAcks b );

// Address auto-increment and packing mode field of CSW register
enum class CswAddrInc : U8
{
    CSW_ADDRINC_DISABLED = 0u,
    CSW_ADDRINC_SINGLE = 1u,
    CSW_ADDRINC_PACKED = 2u,
    CSW_ADDRINC_RESERVED
};

// The size of the data type that is used to access the MEM-AP encoded in size firld of CSW register 
enum class CswSize : U8
{
    CSW_SIZE_8_BIT = 0u,
    CSW_SIZE_16_BIT = 1u,
    CSW_SIZE_32_BIT = 2u,
    CSW_SIZE_64_BIT = 3u,
    CSW_SIZE_128_BIT = 4u,
    CSW_SIZE_256_BIT = 5u,
    CSW_SIZE_RESERVED
};

// Access and DP version bit field
enum class RegMask : U8
{
    REG_READ = 0b00001u,
    REG_WRITE = 0b00010u,
    REG_V1 = 0b00100u,
    REG_V2 = 0b01000u,
    REG_V3 = 0b10000u
};
U8 operator|( const RegMask a, const RegMask b );
U8 operator|( const U8 a, const RegMask b );

enum class SeqCmpResult : U8
{
    SEQ_UNKNOWN = 0u,
    SEQ_MISMATCH = 1u,
    SEQ_MATCH_PARTIALLY = 2u,
    SEQ_MATCH_COMPLETELY = 3u
};
SeqCmpResult BestMach( const SeqCmpResult a, const SeqCmpResult b );

enum class OpItemSize : U8
{
    OP_REQUEST_SIZE = 8u,
    OP_1_TURNAROUND_SIZE = 1u,
    OP_2_TURNAROUND_SIZE = 2u,
    OP_3_TURNAROUND_SIZE = 3u,
    OP_4_TURNAROUND_SIZE = 4u,
    OP_ACK_SIZE = 3u,
    OP_DATA_SIZE = 32u,
    OP_DATA_PARITY_SIZE = 1u
};

enum class OpBitRange : U8
{
    OP_UNDEFINED_RANGE,
    OP_REQUEST_RANGE,
    OP_TURNAROUND_RANGE,
    OP_ACK_RANGE,
    OP_DATA_RANGE,
    OP_DATA_PARITY_RANGE
};

// this is the basic token of the analyzer
// objects of this type are buffered in SWDOperation
struct SWDBit
{
    BitState stateRising;
    BitState stateFalling;

    S64 lowStart;
    S64 rising;
    S64 falling;
    S64 lowEnd;

    bool IsHigh( bool isRising = true ) const;

    S64 GetMinStartEnd() const;
    S64 GetStartSample() const;
    S64 GetEndSample() const;
};

// Function template to get the byte of any value
template <typename T, std::enable_if_t<std::is_unsigned<T>::value, bool> = true>
constexpr U8 GetByteOf( const T value, const std::size_t byteNr )
{
    return ( value >> ( 8u * byteNr ) ) & 0xFFu;
}

// Function template for little endian serialization to U8 vector
template <typename T, std::enable_if_t<std::is_unsigned<T>::value, bool> = true>
std::vector<U8> ToVectorU8( const T value )
{
    std::vector<U8> vector( sizeof( T ) );
    for( std::size_t i = 0u; i < sizeof( T ); ++i )
    {
        vector[ i ] = GetByteOf( value, i );
    }
    return vector;
}

class SWDRequestByte
{
    U8 byte;

    public:
    explicit SWDRequestByte( const U8 byteValue );
};

union SWDRegistersUnion
{
    U64 blob;
    // Encoding rules of reg
    // prev               | current            | usecase
    // -------------------------------------------
    // SWDR_UNDEFINED     | not SWDR_UNDEFINED | DP Read except READBUFF, DP Write, AP Write, First AP Read
    // not SWDR_UNDEFINED | not SWDR_UNDEFINED | Next AP Reads, DP Read of READBUFF
    struct
    {
        U32 memAddr;          // TAR value
        SWDRegisters prev;    // Previous SWD register
        SWDRegisters current; // Currenr SWD register
    } reg;
};

template <typename T, std::enable_if_t<std::is_unsigned<T>::value, bool> = true>
class UintSequence
{
    const std::vector<T>& sequence;
    const std::size_t bitCount;
    S64 startSample = 0;
    std::size_t numberCheckedBits = 0u;
    SeqCmpResult seqCheckResult = SeqCmpResult::SEQ_UNKNOWN;
    void Clear()
    {
        startSample = 0;
        numberCheckedBits = 0u;
        seqCheckResult = SeqCmpResult::SEQ_UNKNOWN;
    }

  public:

    explicit UintSequence( const std::vector<T>& seq )
        : sequence( seq ), bitCount( seq.size() * sizeof( T ) * 8u ), startSample( 0 ), numberCheckedBits( 0u ), seqCheckResult( SeqCmpResult::SEQ_UNKNOWN ) { }
    UintSequence( const std::vector<T>& seq, const std::size_t bitCount )
        : sequence( seq ), bitCount( bitCount ), startSample( 0 ), numberCheckedBits( 0u ), seqCheckResult( SeqCmpResult::SEQ_UNKNOWN ) { }
    SeqCmpResult Match( const std::deque<SWDBit>& bits, const std::size_t startOffset = 0u ) // startOffset is the offset from the start of the bits buffer
    {
        if( bits.size() <= startOffset )
        {
            // Nothing to compare
            seqCheckResult = SeqCmpResult::SEQ_UNKNOWN;
        }
        else
        {
            auto bitsStartSample = bits.at( startOffset ).GetStartSample();
            if( bitsStartSample != startSample )
            {
                // New secuence detected
                Clear(); // Clear the bits
                startSample = bitsStartSample;
            }
            if( ( numberCheckedBits != 0u ) &&
                ( ( seqCheckResult == SeqCmpResult::SEQ_MISMATCH ) || ( seqCheckResult == SeqCmpResult::SEQ_MATCH_COMPLETELY ) ) )
            {
                // The previously checked bits did not match or match cmpletely, so there is no point in comparing the next bits.
                return seqCheckResult;
            }
            for( ; ( ( startOffset + numberCheckedBits ) < bits.size() ) && ( numberCheckedBits < bitCount ) &&
                   ( seqCheckResult != SeqCmpResult::SEQ_MISMATCH );
                 ++numberCheckedBits )
            {
                std::size_t itemIdx = numberCheckedBits / ( sizeof( T ) * 8u );
                std::size_t bitShift = numberCheckedBits % ( sizeof( T ) * 8u );
                if( bits[ startOffset + numberCheckedBits ].IsHigh() == ( ( ( sequence[ itemIdx ] >> bitShift ) & 0b1u ) == 0b1u ) )
                {
                    if( numberCheckedBits == 0u )
                    {
                        // The first bit
                        seqCheckResult = SeqCmpResult::SEQ_MATCH_PARTIALLY;
                    }
                }
                else
                {
                    seqCheckResult = SeqCmpResult::SEQ_MISMATCH;
                }
            }
            if( ( numberCheckedBits == bitCount ) && ( seqCheckResult == SeqCmpResult::SEQ_MATCH_PARTIALLY ) )
            {
                seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
            }
        }
        return seqCheckResult;
    }
    std::size_t GetBitCount() const
	{
		return bitCount;
	}
    const T& GetItem( const std::size_t idx ) const
	{
		return sequence[ idx ];
	}
	const std::size_t GetNumberCheckedBits() const
	{
		return numberCheckedBits;
	}
};

// This class holds current ADI state
class ADIState
{
    bool mIgnoreAck = false;                                        // Whether to ignore the ACK bits in the SWD operation
    SWDRegisters mRegister = SWDRegisters::SWDR_UNDEFINED;          // DebugPort or AccessPort register that this operation is reading/writing
    size_t mNumApReads = 0u;                                        // Number of Continuous AP Reads
    SWDRegisters mLastReadRegister = SWDRegisters::SWDR_UNDEFINED;  // The register that was used in the previous AP read operation.
    CswAddrInc mCswAddrInc = CswAddrInc::CSW_ADDRINC_DISABLED;      // Address auto-increment and packing mode field of CSW register
    CswSize mCswDataSize = CswSize::CSW_SIZE_32_BIT;                // MEM-AP access size
    U32 mTar = 0u;  											    // The value of the AP.TAR register
    // Customizable parser initial values
    DebugProtocol mCurrentProtocol = DebugProtocol::DPROTOCOL_UNKNOWN; // The current Debug Protocol
    SwdFrameTypes mLastFrameType = SwdFrameTypes::SWD_FT_LINE_RESET;   // The last frame type
    DPVersion mDPVersion = DPVersion::DP_V0;                           // The version of the DP architecture implemented
    U8 mTurnaroundNumber = 1u;                                         // Number of turnaround tristate period. Allowed range 1..4
    bool mOrundetection = false;                                       // Overrun detection as per the CTRL/STAT.STICKYORUN flag 
    U32 mSelect = 0u;                                                  // The value of the DP.SELECT register 

    struct DPRegister
    {
        std::set<U8> Bank; // If empty, it covers all banks 0x0-0xF
        U8 Access;         // Register version and access bitmask
        SWDRegisters Register;
    };

    static const std::map<U8, std::vector<DPRegister>> DP_REGISTERS;
    static const std::map<U8, SWDRegisters> MEM_AP_ADI_V5;
    static const std::map<U16, SWDRegisters> MEM_AP_ADI_V6;

  public:

    void SetIgnoreAck( bool ignoreAck );
    bool GetIgnoreAck() const;
    void SetDPVersion( DPVersion version );
    DPVersion GetDPVersion() const;
    void SetRegister( SWDRegisters reg );
    SWDRegisters GetRegister() const;
    void SetTurnaroundNumber( U8 num );
    U8 GetTurnaroundNumber() const;
    void SetOverrunDetection( bool enable );
    bool GetOverrunDetection() const;
    void IncrementNumApReads();
    void ClearNumApReads();
    size_t GetNumApReads() const;
    void SetLastReadRegister( SWDRegisters reg );
    SWDRegisters GetLastReadRegister() const;
    void SetTar( U32 tar );
    U32 GetTar() const;
    void DetermineRegister( bool reqAPnDP, bool reqRnW, U8 reqA23Addr );
    void SetAddrInc( U8 num );
    void SetDataSize( U8 num );
    void IncrementTar();
    void SetSelect( U32 select );
    U32 GetSelect() const;
    void SetCurrentProtocol( DebugProtocol protocol );
    DebugProtocol GetCurrentProtocol() const;
    void SetLastFrameType( SwdFrameTypes frameType );
    SwdFrameTypes GetLastFrameType() const;
};

class SWDBaseSequence
{
  private:
    // Sequence conditions
    const std::set<DebugProtocol> allowedDebugProtocols;
    const std::set<SwdFrameTypes> allowedPreviousFrames; // If empty, it means all types of previous frames are allowed

  protected:
    // Internal variables
    const SwdFrameTypes seqFrameType; // The frame type of the sequence
    std::deque<SWDBit> bits;          // Sequence bits
    S64 startSample;
    S64 endSample;
    std::size_t numberCheckedBits;
    SeqCmpResult seqCheckResult;
    ADIState& adiState;

    virtual void UpdateBitInfo();
    bool BreakMatch( const std::deque<SWDBit>& bits );
    virtual void CaptureNewSequence( const std::deque<SWDBit>& bits );
    void AddFrame( SWDAnalyzerResults* pResults, const S64 startingSampleInclusive, const S64 endingSampleInclusive, const U64 data1,
                   const U64 data2, const SwdFrameTypes type, const U8 flags ) const;
    void AddFrameV2SimpleSequence( SWDAnalyzerResults* pResults, const S64 startingSampleInclusive, const S64 endingSampleInclusive,
                                   const SwdFrameTypes frameType, const std::size_t bitCount ) const;
    void AddFrameV2DataSequence( SWDAnalyzerResults* pResults, const S64 startingSampleInclusive, const S64 endingSampleInclusive,
                                   const SwdFrameTypes frameType, const std::size_t bitCount, const std::vector<U8>& bytes ) const;

  public:
    explicit SWDBaseSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                              const std::set<SwdFrameTypes>& previousFrames,
                             ADIState& adiStateRef );

    const std::size_t GetNumberCheckedBits() const;
    const bool Empty() const;
    virtual void Clear();
    virtual void AddFrames( SWDAnalyzerResults* pResults ) = 0;
    virtual void AddMarkers( SWDAnalyzerResults* pResults ) const;
    virtual SeqCmpResult Match( const std::deque<SWDBit>& bits ) = 0;
    virtual bool IsFixedLengthSequence() const = 0;
    const bool IsDebugProtocolAllowed( DebugProtocol protocol ) const;
    const bool IsPreviousFrameTypeAllowed( SwdFrameTypes frameType ) const;
    virtual void UpdateAdiState();
    void CopyBits( std::deque<SWDBit>& srcBits );
};

class SWDFixedLengthSequence : public SWDBaseSequence
{
  public:
    explicit SWDFixedLengthSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                                     const std::set<SwdFrameTypes>& previousFrames,
                                      ADIState& adiStateRef );
    bool IsFixedLengthSequence() const override { return true; };
};

template <typename T1, typename T2, std::enable_if_t<std::is_unsigned<T1>::value && std::is_unsigned<T2>::value, bool> = true>
class SwitchSequenceWithDeprecate : public SWDFixedLengthSequence
{
  private:
    UintSequence<T1>& sequence;
    UintSequence<T2>& sequenceDeprecated;

  protected:
    bool deprecated; // Deprecated flag

  public:
    // mFlag
    enum class SQ_FLAG : U8
    {
        IS_NONE = 0u,
        IS_DEPRECATED = 1u
    };
    explicit SwitchSequenceWithDeprecate( const SwdFrameTypes frameType, ADIState& adiStateRef, UintSequence<T1>& sequenceRef,
                                          UintSequence<T2>& sequenceDeprecatedRef )
        : SWDFixedLengthSequence( frameType,
                                  { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_SWD, DebugProtocol::DPROTOCOL_JTAG },
                                  { SwdFrameTypes::SWD_FT_LINE_RESET }, adiStateRef ),
          sequence( sequenceRef ),
          sequenceDeprecated( sequenceDeprecatedRef ),
          deprecated( false )
    {
    }
    void Clear() override
    {
        SWDFixedLengthSequence::Clear();
        deprecated = false;
    }
    SeqCmpResult Match( const std::deque<SWDBit>& bits ) override
    {
        if( BreakMatch( bits ) )
        {
            return seqCheckResult;
        }
        const SeqCmpResult cmpResult = sequence.Match( bits );
        const SeqCmpResult cmpResultDeprecated = sequenceDeprecated.Match( bits );
        seqCheckResult = BestMach( cmpResult, cmpResultDeprecated );
        numberCheckedBits = ( seqCheckResult == cmpResult ) ? sequence.GetNumberCheckedBits() : sequenceDeprecated.GetNumberCheckedBits();
        if( seqCheckResult == SeqCmpResult::SEQ_MATCH_COMPLETELY )
        {
            deprecated = ( seqCheckResult == cmpResultDeprecated );
        }
        return seqCheckResult;
    }
    typename std::common_type<T1, T2>::type GetData() const
	{
        return ( !deprecated ) ? sequence.GetItem( 0u ) : sequenceDeprecated.GetItem( 0u );
	}
    void AddFrames( SWDAnalyzerResults* pResults ) override
    {
        // Legacy Frame
        auto data = GetData();
        AddFrame( pResults, startSample, endSample, data, 0u, seqFrameType,
                  static_cast<U8>( deprecated ? SwitchSequenceWithDeprecate::SQ_FLAG::IS_DEPRECATED
                                              : SwitchSequenceWithDeprecate::SQ_FLAG::IS_NONE ) );

        // FrameV2
        std::vector<U8> bytes = ToVectorU8( data );
        AddFrameV2DataSequence( pResults, startSample, endSample, seqFrameType, numberCheckedBits, bytes );
    }
};


template <typename T1, typename T2, typename T3,
          std::enable_if_t<std::is_unsigned<T1>::value && std::is_unsigned<T2>::value && std::is_unsigned<T3>::value, bool> = true>
class SwitchSequenceActivationCode : public SWDFixedLengthSequence
{
  private:
    UintSequence<T1>& sequenceActivationCode1;
    UintSequence<T2>& sequenceActivationCode2;
    UintSequence<T3>& sequenceActivationCode3;

  protected:
    enum class ActivationCodeID : U8
    {
        ACTIVATION_CODE_1 = 0u,
        ACTIVATION_CODE_2 = 1u,
        ACTIVATION_CODE_3 = 2u
    };
    ActivationCodeID resultIdx;

  public:
    explicit SwitchSequenceActivationCode( const SwdFrameTypes frameType, ADIState& adiStateRef, UintSequence<T1>& activationCode1Ref,
                                           UintSequence<T2>& activationCode2Ref, UintSequence<T3>& activationCode3Ref )
        : SWDFixedLengthSequence( frameType, { DebugProtocol::DPROTOCOL_UNKNOWN, DebugProtocol::DPROTOCOL_DORMANT },
                                  { SwdFrameTypes::SWD_FT_DS_ACTIVATION_CODE_PREAMBLE }, adiStateRef ),
          sequenceActivationCode1( activationCode1Ref ),
          sequenceActivationCode2( activationCode2Ref ),
          sequenceActivationCode3( activationCode3Ref ),
          resultIdx( ActivationCodeID::ACTIVATION_CODE_1 )
    {
    }
    void Clear() override
	{
		SWDFixedLengthSequence::Clear();
		resultIdx = ActivationCodeID::ACTIVATION_CODE_1;
	}
    SeqCmpResult Match( const std::deque<SWDBit>& bits ) override
    {
        if( BreakMatch( bits ) )
        {
            return seqCheckResult;
        }
        const SeqCmpResult activationCode1Result = sequenceActivationCode1.Match( bits );
        const SeqCmpResult activationCode2Result = sequenceActivationCode2.Match( bits );
        const SeqCmpResult activationCode3Result = sequenceActivationCode3.Match( bits );
        seqCheckResult = BestMach( activationCode1Result, activationCode2Result );
        seqCheckResult = BestMach( seqCheckResult, activationCode3Result );
        if( seqCheckResult == activationCode1Result )
        {
            numberCheckedBits = sequenceActivationCode1.GetNumberCheckedBits();
            resultIdx = ActivationCodeID::ACTIVATION_CODE_1;
        }
        else if( seqCheckResult == activationCode2Result )
        {
            numberCheckedBits = sequenceActivationCode2.GetNumberCheckedBits();
            resultIdx = ActivationCodeID::ACTIVATION_CODE_2;
        }
        else
        {
            numberCheckedBits = sequenceActivationCode3.GetNumberCheckedBits();
            resultIdx = ActivationCodeID::ACTIVATION_CODE_3;
        }
        return seqCheckResult;
    }
    typename std::common_type<T1, T2, T3>::type GetData() const
    {
        if( resultIdx == ActivationCodeID::ACTIVATION_CODE_1 )
        {
            return sequenceActivationCode1.GetItem( 0u );
        }
        else if( resultIdx == ActivationCodeID::ACTIVATION_CODE_2 )
        {
            return sequenceActivationCode2.GetItem( 0u );
        }
        else
        {
            return sequenceActivationCode3.GetItem( 0u );
        }
    }
};

template <typename T, std::enable_if_t<std::is_unsigned<T>::value, bool> = true>
class SwitchSequence : public SWDFixedLengthSequence
{
  private:
    UintSequence<T>& sequence;

  public:
    explicit SwitchSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                             const std::set<SwdFrameTypes>& previousFrames, ADIState& adiStateRef, UintSequence<T>& sequenceRef )
        : SWDFixedLengthSequence( frameType, debugProtocols, previousFrames, adiStateRef ), sequence( sequenceRef )
    {
    }
	SeqCmpResult Match( const std::deque<SWDBit>& bits ) override
	{
		if( BreakMatch( bits ) )
		{
			return seqCheckResult;
		}
		seqCheckResult = sequence.Match( bits );
		numberCheckedBits = sequence.GetNumberCheckedBits();
		return seqCheckResult;
	}
    T GetData(std::size_t idx = 0u) const
	{
        return sequence.GetItem( idx );
	}
    void AddFrames( SWDAnalyzerResults* pResults ) override
    {
        // Legacy Frame
        auto data = GetData();
        AddFrame( pResults, startSample, endSample, data, 0u, seqFrameType, 0u );

        // FrameV2
        std::vector<U8> bytes = ToVectorU8( data );
        AddFrameV2DataSequence( pResults, startSample, endSample, seqFrameType, numberCheckedBits, bytes );
    }
};

class SWDVariableLengthSequence : public SWDBaseSequence
{
  protected:
    size_t bitCount;

  public:
    explicit SWDVariableLengthSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                                        const std::set<SwdFrameTypes>& previousFrames,
                                         ADIState& adiStateRef );
    void UpdateBitInfo() override;
    bool IsFixedLengthSequence() const override { return false; };
};

class PlainBitSequence : public SWDVariableLengthSequence
{
  private:
    const bool bitHigh;               // The bit value
    const size_t minimumBitCnt; // The minimum number of bits in the sequence

  public:
    explicit PlainBitSequence( const SwdFrameTypes frameType, const std::set<DebugProtocol>& debugProtocols,
                               const std::set<SwdFrameTypes>& previousFrames,
                               ADIState& adiStateRef, const bool bitInHigh, const std::size_t requiredBitCnt )
        : SWDVariableLengthSequence( frameType, debugProtocols, previousFrames, adiStateRef ), bitHigh( bitInHigh ), minimumBitCnt( requiredBitCnt )
    { }
	SeqCmpResult Match( const std::deque<SWDBit>& bits ) override
    {
        if( BreakMatch( bits ) )
        {
            return seqCheckResult;
        }
        // Loop through the unchecked bits.
        for( ; ( numberCheckedBits < bits.size() ) && ( seqCheckResult != SeqCmpResult::SEQ_MISMATCH ); ++numberCheckedBits )
        {
            if( bits[ numberCheckedBits ].IsHigh() == bitHigh )
            {
                if( numberCheckedBits == 0u )
                {
                    // The first bit
                    seqCheckResult = SeqCmpResult::SEQ_MATCH_PARTIALLY;
                }
            }
            else
            {
                if( numberCheckedBits >= minimumBitCnt )
                {
                    seqCheckResult = SeqCmpResult::SEQ_MATCH_COMPLETELY;
                    break;
                }
                else
                {
                    seqCheckResult = SeqCmpResult::SEQ_MISMATCH;
                }
            }
        }
        return seqCheckResult;
    }
};

// this object contains data about one SWD operation as described in section 5.3
// of the ARM Debug Interface v5 Architecture Specification
class SWDOperation : public SWDFixedLengthSequence
{
    // request
    bool reqAPnDP;  // APnDP
    bool reqRnW;    // RnW
    U8 reqA23Addr;  // A[2:3]
    U8 reqParity;   // Number of bits with a value of 0b1 in  bits APnDP, RnW and A[2:3]:
    U8 reqByte;     // the entire request byte
    // acknowledge
    U8 reqAck;	    // ACK
    // data
    U32 data;	    // RDATA or WDATA
    U8 calculatedDataParity; // The calculated parity of the data
    U8 dataParity;  // DATA parity
    bool dataParityOk; // Is the calculated parity matches DATA parity


    static const std::vector<std::vector<std::pair<OpBitRange, OpItemSize>>> OP_REQ_AND_ACK_SIZE;
    static const std::vector<std::vector<std::pair<OpBitRange, OpItemSize>>> OP_RDATA_SIZE;
    static const std::vector<std::vector<std::pair<OpBitRange, OpItemSize>>> OP_WDATA_SIZE;
    const std::vector<std::pair<OpBitRange, OpItemSize>>* opReqAndAckItemSize;
    const std::vector<std::pair<OpBitRange, OpItemSize>>* opRdataItemSize;
    const std::vector<std::pair<OpBitRange, OpItemSize>>* opWdataItemSize;
    std::size_t opReqAndAckSize;
    std::size_t opAckOffset;
    std::size_t opDataOffset;

    enum class OP_REQUEST_BIT : U8
    {
        OP_REQ_START = 0u,
        OP_REQ_AP_N_DP = 1u,
        OP_REQ_R_N_W = 2u,
        OP_REQ_A2 = 3u,
        OP_REQ_A3 = 4u,
        OP_REQ_PARITY = 5u,
        OP_REQ_STOP = 6u,
        OP_REQ_PARK = 7u
    };

    OpBitRange GetOpBitRange( const std::vector<std::pair<OpBitRange, OpItemSize>>* opItemSize, const std::size_t bitIdx ) const;
    std::size_t GetOpBitRangeFirstBitIdx( const std::size_t bitIdx ) const;
    void GetAndParseOperationRequest( const std::deque<SWDBit>& bits );
    void GetAndParseOperationTurnaround( const std::deque<SWDBit>& bits );
    void GetAndParseOperationAck( const std::deque<SWDBit>& bits );
    void GetAndParseOperationData( const std::deque<SWDBit>& bits );
    void GetAndParseOperationDataParity( const std::deque<SWDBit>& bits );
    void CapureApRegData( SWDRegisters reg );

  protected:
	void CaptureNewSequence( const std::deque<SWDBit>& bits ) override; 

  public:
    explicit SWDOperation( ADIState& adiStateRef );

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) override;
    void AddMarkers( SWDAnalyzerResults* pResults ) const override;
    SeqCmpResult Match( const std::deque<SWDBit>& bits ) override;
    void UpdateAdiState() override;
};

class SWDLineReset : public SWDVariableLengthSequence
{
    std::size_t firstLowBitOffset;

  public:
    explicit SWDLineReset( ADIState& adiStateRef );

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) override;
    SeqCmpResult Match( const std::deque<SWDBit>& bits ) override;
    void UpdateAdiState() override;
};

class JTAGToSWD : public SwitchSequenceWithDeprecate<U16, U16>
{
  public:
    explicit JTAGToSWD( ADIState& adiStateRef );

    void UpdateAdiState() override;
};

class SWDToJTAG : public SwitchSequenceWithDeprecate<U16, U16>
{
  public:
    explicit SWDToJTAG( ADIState& adiStateRef );

    void UpdateAdiState() override;
};

class SWDErrorBits : public SWDVariableLengthSequence
{
    DebugProtocol protocol;

  public:
	explicit SWDErrorBits( ADIState& adiStateRef );

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) override;
    SeqCmpResult Match( const std::deque<SWDBit>& bits ) override { return SeqCmpResult::SEQ_UNKNOWN; };
    void SetProtocol( const DebugProtocol newProtocol );
    void PushBackBit( SWDBit&& bit );
};

class SWDIdleCycles : public PlainBitSequence
{
  public:
    explicit SWDIdleCycles( ADIState& adiStateRef );

    void AddFrames( SWDAnalyzerResults* pResults ) override;
};

class JTAGTlr : public PlainBitSequence
{
  public:
    explicit JTAGTlr( ADIState& adiStateRef );

    void AddFrames( SWDAnalyzerResults* pResults ) override;
};

class JTAGToDS : public SwitchSequence<U32>
{
  public:
    explicit JTAGToDS( ADIState& adiStateRef );

    void UpdateAdiState() override;
};

class SWDToDS : public SwitchSequence<U16>
{
  public:
    explicit SWDToDS( ADIState& adiStateRef );

    void UpdateAdiState() override;
};

class DSSelectionAlertPreamble : public PlainBitSequence
{
  public:
    explicit DSSelectionAlertPreamble( ADIState& adiStateRef );

    void AddFrames( SWDAnalyzerResults* pResults ) override;
};

class DSSelectionAlert : public SwitchSequence<U64>
{
  public:
    explicit DSSelectionAlert( ADIState& adiStateRef );
        
    void AddFrames( SWDAnalyzerResults* pResults ) override;
};

class DSActivationCodePreamble : public SwitchSequence<U8>
{
  public:
    explicit DSActivationCodePreamble( ADIState& adiStateRef );

    void AddFrames( SWDAnalyzerResults* pResults ) override;
};

class DSActivationCode : public SwitchSequenceActivationCode<U16, U8, U8>
{
  public:
    explicit DSActivationCode( ADIState& adiStateRef );

    void AddFrames( SWDAnalyzerResults* pResults ) override;
    void UpdateAdiState() override;
};

struct SWDRequestFrame : public Frame
{
    // mData1 contains addr, mData2 contains the register enum

    // mFlag
    enum class RQ_FLAG : U8
    {
        IS_NONE = 0u,
        IS_READ = ( 1u << 0u ),
        IS_ACCESS_PORT = ( 1u << 1u ),
    };

    bool IsRead() const;
    bool IsAccessPort() const;
    void SetRegister( SWDRegisters reg );
    SWDRegisters GetRegister() const;
    std::string GetRegisterName() const;
};

class SWDAnalyzer;

// This object parses and buffers the bits of the SWD stream.
// IsOperation and IsLineReset return true if the subsequent bits in
// the stream are a valid operation or line reset.
class SWDParser
{
  private:
    AnalyzerChannelData* mSWDIO;
    AnalyzerChannelData* mSWCLK;

    SWDAnalyzer* mAnalyzer;

    std::deque<SWDBit> mBitsBuffer;

    ADIState mAdiState;

    // SWD sequence objects
    SWDOperation mTran; // bit count >= 12 <= 46. SWD read/write transaction
    SWDLineReset mReset;  // bit count > 50. SWD line reset. At least 50 SWCLKTCK cycles with SWDIOTMS HIGH.
    JTAGToSWD mJtagToSwd; // bit count == 16. JTAG-to-SWD select sequence
    SWDToJTAG mSwdToJtag; // bit count == 16. SWD-to-JTAG select sequence
    SWDErrorBits mErrorBits; // bit count > 0. Error bits.
    SWDIdleCycles mIdleCycles; // bit count > 1. SWD idle cycles. SWCLKTCK cycles with SWDIOTMS LOW
    JTAGTlr mJtagTlr;   // bit count > 5. Enters to JTAG Test-Logic-Reset state
    JTAGToDS mJtagToDs; // bit count == 31. JTAG-to-DS select sequence
    SWDToDS mSwdToDs; // bit count == 16. SWD-to-DS select sequence
    DSSelectionAlertPreamble mDsSelectionAlertPreamble; // bit count > 8. Selection Alert preamble. At least 8 SWCLKTCK cycles with SWDIOTMS HIGH.
    DSSelectionAlert mDsSelectionAlert; // bit count == 128. Selection Alert sequence
    DSActivationCodePreamble mDsActivationCodePreamble; // bit count == 4. Activation Code preamble. 4 SWCLKTCK cycles with SWDIOTMS LOW
    DSActivationCode mDsActivationCode; // bit count >= 8 <=12. Activation Code sequence

    std::vector <std::reference_wrapper<SWDBaseSequence>> mSequences;

    SWDBit ParseBit();

  public:
    SWDParser();

    void Setup( AnalyzerChannelData* pSWDIO, AnalyzerChannelData* pSWCLK, SWDAnalyzer* pAnalyzer );
    void Clear();
    bool IsBufferEmpty() const;
    void AddBitToBuffer();
    void CopyBits( SWDBaseSequence& sequence ); // Move bits from mBitsBuffer to sequence object

    SwdFrameTypes GetLastFrameType() const;
    DebugProtocol GetCurrentProtocol() const;
    DPVersion GetDPVersion() const;

    // Initial state setters
    void SetCurrentProtocol( DebugProtocol protocol );
    void SetLastFrameType( SwdFrameTypes frame );
    void SetDPVersion( DPVersion version );
    void SetNumTurnarounds( U8 num );
    void SetOverrunDetection( bool enabled );
    void SetSelectRegister( U32 value );

    // Comparison method (bit by bit speculative partial comparison)
    SeqCmpResult Match( SWDBaseSequence& sequence );

    // Sequence getter
    std::vector<std::reference_wrapper<SWDBaseSequence>>& GetSequences();
    SWDErrorBits& GetErrorBits();

    SWDBit PopFrontBit();
};

#endif // SWD_TYPES_H


