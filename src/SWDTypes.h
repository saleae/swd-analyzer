#ifndef SWD_TYPES_H
#define SWD_TYPES_H

#include <LogicPublicTypes.h>

#include "SWDAnalyzerResults.h"

// the possible frame types
enum SWDFrameTypes
{
	SWDFT_Error,
	SWDFT_Bit,

	SWDFT_LineReset,

	SWDFT_Request,
	SWDFT_Turnaround,
	SWDFT_ACK,
	SWDFT_WData,
	SWDFT_DataParity,
	SWDFT_TrailingBits,
};

// the DebugPort and AccessPort registers as defined by SWD
enum SWDRegisters
{
	SWDR_undefined,

	// DP
	SWDR_DP_IDCODE,
	SWDR_DP_ABORT,
	SWDR_DP_CTRL_STAT,
	SWDR_DP_WCR,
	SWDR_DP_RESEND,
	SWDR_DP_SELECT,
	SWDR_DP_RDBUFF,
	SWDR_DP_ROUTESEL,

	// AP
	SWDR_AP_CSW,
	SWDR_AP_TAR,
	SWDR_AP_DRW,
	SWDR_AP_BD0,
	SWDR_AP_BD1,
	SWDR_AP_BD2,
	SWDR_AP_BD3,
	SWDR_AP_CFG,
	SWDR_AP_BASE,
	SWDR_AP_RAZ_WI,
	SWDR_AP_IDR,
};

// some ACK values
enum SWDACK
{
	ACK_OK		= 1,
	ACK_WAIT	= 2,
	ACK_FAULT	= 4,
};

// this is the basic token of the analyzer
// objects of this type are buffered in SWDOperation
struct SWDBit
{
	BitState	state_rising;
	BitState	state_falling;

	S64			low_start;
	S64			rising;
	S64			falling;
	S64			low_end;

	bool IsHigh(bool is_rising = true) const
	{
		return (is_rising ? state_rising : state_falling) == BIT_HIGH;
	}

	S64 GetMinStartEnd() const;
	S64 GetStartSample() const;
	S64 GetEndSample() const;

	Frame MakeFrame();
};

// this object contains data about one SWD operation as described in section 5.3
// of the ARM Debug Interface v5 Architecture Specification
struct SWDOperation
{
	// request
	bool		APnDP;
	bool		RnW;
	U8			addr;		// A[2..3]

	U8			parity_read;

	U8			request_byte;	// the entire request byte

	// acknowledge
	U8			ACK;

	// data
	U32			data;
	U8			data_parity;
	bool		data_parity_ok;

	std::vector<SWDBit>		bits;

	// DebugPort or AccessPort register that this operation is reading/writing
	SWDRegisters			reg;

	void Clear();
	void AddFrames(SWDAnalyzerResults* pResults);
	void AddMarkers(SWDAnalyzerResults* pResults);
	void SetRegister(U32 select_reg);

	bool IsRead()
	{
		return RnW;
	}
};

struct SWDLineReset
{
	std::vector<SWDBit>		bits;

	void Clear()
	{
		bits.clear();
	}

	void AddFrames(AnalyzerResults* pResults);
};

struct SWDRequestFrame: public Frame
{
	// mData1 contains addr, mData2 contains the register enum

	// mFlag
	enum {
		IS_READ			= (1 << 0),
		IS_ACCESS_PORT	= (1 << 1),
	};

	void SetRequestByte(U8 request_byte)
	{
		mData1 = request_byte;
	}

	U8 GetAddr() const			{ return (U8) ((mData1 >> 1) & 0xc); }
	bool IsRead() const			{ return (mFlags & IS_READ) != 0; }
	bool IsAccessPort() const	{ return (mFlags & IS_ACCESS_PORT) != 0; }
	bool IsDebugPort() const	{ return !IsAccessPort(); }

	void SetRegister(SWDRegisters reg)		{ mData2 = reg; }
	SWDRegisters GetRegister() const		{ return SWDRegisters(mData2); }
	std::string GetRegisterName() const;
};

class SWDAnalyzer;

// This object parses and buffers the bits of the SWD stream.
// IsOperation and IsLineReset return true if the subsequent bits in
// the stream are a valid operation or line reset.
class SWDParser
{
private:
	AnalyzerChannelData*	mSWDIO;
	AnalyzerChannelData*	mSWCLK;

	SWDAnalyzer*			mAnalyzer;

	std::vector<SWDBit>		mBitsBuffer;
	U32						mSelectRegister;

	SWDBit ParseBit();
	void BufferBits(size_t num_bits);

public:
	SWDParser();

	void Setup(AnalyzerChannelData* pSWDIO, AnalyzerChannelData* pSWCLK, SWDAnalyzer* pAnalyzer);

	void Clear()
	{
		mBitsBuffer.clear();
		mSelectRegister = 0;
	}

	bool IsOperation(SWDOperation& tran);
	bool IsLineReset(SWDLineReset& reset);

	SWDBit PopFrontBit();
};

#endif	// SWD_TYPES_H
