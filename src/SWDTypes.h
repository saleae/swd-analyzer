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

  public:
    static const std::string GetSwdFrameName( SwdFrameTypes frame );
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
enum class SWDRegisters
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
enum class SWDAcks
{
    ACK_OK = 1,
    ACK_WAIT = 2,
    ACK_FAULT = 4,
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

    Frame MakeFrame() const;
};

class SWDRequestByte
{
    U8 byte;

    public:
    explicit SWDRequestByte( const U8 byteValue );
    bool IsByteValid() const;
    bool GetAPnDP() const;
    bool GetRnW() const;
    U8 GetAddr() const; // A[2..3]
};

union SWDRegistersUnion
{
    U64 blob;
    // Encoding rules of reg
    // prev               | current            | usecase
    // -------------------------------------------
    // SWDR_UNDEFINED     | not SWDR_UNDEFINED | DP Read except READBUFF, DP Write, AP Write
    // not SWDR_UNDEFINED | SWDR_UNDEFINED     | First AP Read
    // not SWDR_UNDEFINED | not SWDR_UNDEFINED | Next AP Reads, DP Read of READBUFF
    struct
    {
        SWDRegisters prev;    // Previous SWD register
        SWDRegisters current; // Currenr SWD register
    } reg;
};

struct SWDBaseSequnce
{
    std::deque<SWDBit> bits;

    virtual void Clear();
    virtual void AddFrames( SWDAnalyzerResults* pResults ) const;
    virtual void AddMarkers( SWDAnalyzerResults* pResults ) const;
};

// this object contains data about one SWD operation as described in section 5.3
// of the ARM Debug Interface v5 Architecture Specification
struct SWDOperation : SWDBaseSequnce
{
    // request
    bool reqAPnDP = false;
    bool reqRnW = true;
    U8 addr = 0u; // A[2..3]
    U8 parityRead = 0u;
    U8 requestByte = 0u; // the entire request byte
    // acknowledge
    U8 reqAck = 0u;
    // data
    U32 data = 0u;
    U8 dataParity = 0u;
    bool dataParityOk = true;
    // DebugPort or AccessPort register that this operation is reading/writing
    DPVersion dpVer = DPVersion::DP_V0;
    SWDRegisters reg = SWDRegisters::SWDR_UNDEFINED;
    // Number of turnaround tristate period. Allowed range 1..4
    U8 turnaround = 1u;
    // Overrun detection
    bool orundetect = false;

    struct DPRegister
    {
        std::set<U8> Bank; // If empty, it covers all banks 0x0-0xF
        U8 Access;         // Register version and access bitmask
        SWDRegisters Register;
    };

    static const std::map<U8, std::vector<DPRegister>> DP_REGISTERS;
    static const std::map<U8, SWDRegisters> MEM_AP_ADI_V5;
    static const std::map<U16, SWDRegisters> MEM_AP_ADI_V6;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
    void AddMarkers( SWDAnalyzerResults* pResults ) const override;
    void SetRegister( U32 selectReg );
    void SetDPVer( DPVersion version );
    void SetTurnaroundNumber( U8 num );
    void SetOrunDetect( bool enable );

    bool IsRead() const;
    size_t GetTurnaroundNumber() const;
    bool GetOrunDetect() const;
};

struct SWDLineReset : SWDBaseSequnce
{
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct JTAGToSWD : SWDBaseSequnce
{
    // Deprecated flag
    bool deprecated = false;
     // Detected sequence value
    U16 data = 0u;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct SWDToJTAG : SWDBaseSequnce
{
    // Deprecated flag
    bool deprecated = false;
    // Detected sequence value
    U16 data = 0u;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct SWDErrorBits : SWDBaseSequnce
{
    DebugProtocol protocol = DebugProtocol::DPROTOCOL_UNKNOWN;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;

    void SetProtocol( const DebugProtocol newProtocol );
};

struct SWDIdleCycles : SWDBaseSequnce
{
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct JTAGTlr : SWDBaseSequnce
{
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct JTAGToDS : SWDBaseSequnce
{
    // Detected sequence value
    U32 data = 0u;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct SWDToDS : SWDBaseSequnce
{
    // Detected sequence value
    U16 data = 0u;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct DSSelectionAlertPreamble : SWDBaseSequnce
{
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct DSSelectionAlert : SWDBaseSequnce
{
    U64 hi64BitData = 0u;
    U64 lo64BitData = 0u;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct DSActivationCodePreamble : SWDBaseSequnce
{
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct DSActivationCode : SWDBaseSequnce
{
    // Detected sequence value
    U16 data = 0u;

    void Clear() override;
    void AddFrames( SWDAnalyzerResults* pResults ) const override;
};

struct SWDRequestFrame : public Frame
{
    // mData1 contains addr, mData2 contains the register enum

    // mFlag
    enum
    {
        IS_READ = ( 1 << 0 ),
        IS_ACCESS_PORT = ( 1 << 1 ),
    };

    void SetRequestByte( U8 requestByte );
    U8 GetAddr() const;
    bool IsRead() const;
    bool IsAccessPort() const;
    bool IsDebugPort() const;
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

    SwdFrameTypes mLastFrameType;
    DebugProtocol mCurrentProtocol;
    DPVersion mDPVersion;

    // SWD sequence objects
    SWDOperation mTran;
    SWDLineReset mReset;
    JTAGToSWD mJtagToSwd;
    SWDToJTAG mSwdToJtag;
    SWDErrorBits mErrorBits;
    SWDIdleCycles mIdleCycles;
    JTAGTlr mJtagTlr;
    JTAGToDS mJtagToDs;
    SWDToDS mSwdToDs;
    DSSelectionAlertPreamble mDsSelectionAlertPreamble;
    DSSelectionAlert mDsSelectionAlert;
    DSActivationCodePreamble mDsActivationCodePreamble;
    DSActivationCode mDsActivationCode;

    U32 mSelectRegister;

    // consts
    static const U16 SEQUENCE_JTAG_SERIAL = 0x0000u; // 0b0000_0000_0000 transmitted LSB first, JTAG-Serial
    static const U8 SEQUENCE_SW_DP = 0x1Au;          // 0b0001_1010 transmitted LSB first, ARM CoreSight SW-DP
    static const U8 SEQUENCE_JTAG_DP = 0x0Au;        // 0b0000_1010 transmitted LSB first, ARM CoreSight JTAG-DP

    SWDBit ParseBit();
    void CopyBits( std::deque<SWDBit>& destination, const size_t numBits );

    // Comparison pattern primitives
    bool IsAtLeast( const size_t numBits, const BitState bit );
    size_t BitCount( const BitState bit, const size_t startingFromBit = 0 );
    bool IsU8Sequence( const U8 *sequence, const size_t bitCnt = sizeof( U8 ) * 8u );
    bool IsU16Sequence( const U16 *sequence, const size_t bitCnt = sizeof( U16 ) * 8u );
    bool IsU32Sequence( const U32 *sequence, const size_t bitCnt = sizeof( U32 ) * 8u );
    bool IsU64Sequence( const U64 *sequence, const size_t bitCnt = sizeof( U64 ) * 8u );

  public:
    SWDParser();

    void Setup( AnalyzerChannelData* pSWDIO, AnalyzerChannelData* pSWCLK, SWDAnalyzer* pAnalyzer );
    void Clear();
    void BufferBits( const size_t numBits );

    SwdFrameTypes GetLastFrameType() const;
    DebugProtocol GetCurrentProtocol() const;
    DPVersion GetDPVersion() const;

    void SetCurrentProtocol( DebugProtocol protocol );
    void SetLastFrameType( SwdFrameTypes frame );
    void SetDPVersion( DPVersion version );
    void SetNumTurnarounds( U8 num );
    void SetOverrunDetection( bool enabled );
    void SetSelectRegister( U32 value );

    // Compare methods
    bool IsOperation();
    bool IsLineReset();  // At least 50 SWCLKTCK cycles with SWDIOTMS HIGH.
    bool IsJtagToSwd();
    bool IsSwdToJtag();
    bool IsIdleCycles(); // SWCLKTCK cycles with SWDIOTMS LOW.
    bool IsJtagTlr();    // At least five SWCLKTCK cycles with SWDIOTMS HIGH.
    bool IsJtagToDs();
    bool IsSwdToDs();
    bool IsDsSelectionAlertPreamble(); // At least eight SWCLKTCK cycles with SWDIOTMS HIGH.
    bool IsDsSelectionAlert();         // Selection Alert sequence ( 128 cycles )
    bool IsDsActivationCodePreamble(); // 4 cycles with SWDIOTMS LOW
    bool IsDsActivationCode();

    // Sequence getter
    const SWDBaseSequnce& GetLineReset() const;
    const SWDBaseSequnce& GetJtagToSwd() const;
    const SWDBaseSequnce& GetSwdToJtag() const;
    const SWDBaseSequnce& GetOperation() const;
    const SWDBaseSequnce& GetIdleCycles() const;
    const SWDBaseSequnce& GetJtagTlr() const;
    const SWDBaseSequnce& GetJtagToDs() const;
    const SWDBaseSequnce& GetSwdToDs() const;
    const SWDBaseSequnce& GetDsSelectionAlertPreamble() const;
    const SWDBaseSequnce& GetDsSelectionAlert() const;
    const SWDBaseSequnce& GetDsActivationCodePreamble() const;
    const SWDBaseSequnce& GetDsActivationCode() const;
    SWDErrorBits& GetErrorBits();

    // Sequence status updaters
    void SetLineReset();
    void SetJtagToSwd();
    void SetSwdToJtag();
    void SetOperation();
    void SetIdleCycles();
    void SetJtagTlr();
    void SetJtagToDs();
    void SetSwdToDs();
    void SetDsSelectionAlertPreamble();
    void SetDsSelectionAlert();
    void SetDsActivationCodePreamble();
    void SetDsActivationCode();

    void SetErrorBits();

    SWDBit PopFrontBit();
};

#endif // SWD_TYPES_H
