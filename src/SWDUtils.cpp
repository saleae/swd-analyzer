#include <AnalyzerHelpers.h>

#include "SWDUtils.h"
#include "SWDTypes.h"

std::string GetRegisterName(SWDRegisters reg)
{
	switch (reg)
	{
	case SWDR_DP_IDCODE:		return "IDCODE";
	case SWDR_DP_ABORT:			return "ABORT";
	case SWDR_DP_CTRL_STAT:		return "CTRL/STAT";
	case SWDR_DP_WCR:			return "WCR";
	case SWDR_DP_RESEND:		return "RESEND";
	case SWDR_DP_SELECT:		return "SELECT";
	case SWDR_DP_RDBUFF:		return "RDBUFF";
	case SWDR_DP_ROUTESEL:		return "ROUTESEL";

	case SWDR_AP_CSW:			return "CSW";
	case SWDR_AP_TAR:			return "TAR";
	case SWDR_AP_DRW:			return "DRW";
	case SWDR_AP_BD0:			return "BD0";
	case SWDR_AP_BD1:			return "BD1";
	case SWDR_AP_BD2:			return "BD2";
	case SWDR_AP_BD3:			return "BD3";
	case SWDR_AP_CFG:			return "CFG";
	case SWDR_AP_BASE:			return "BASE";
	case SWDR_AP_RAZ_WI:		return "RAZ_WI";
	case SWDR_AP_IDR:			return "IDR";
	}

	return "??";
}

std::string GetRegisterValueDesc(SWDRegisters reg, U32 val, DisplayBase display_base)
{
	std::string ret_val;
	
	switch (reg)
	{
	case SWDR_DP_IDCODE:
		ret_val  = "DESIGNER=" + int2str_sal(val & 0xfff, display_base, 12);
		ret_val += ", PARTNO=" + int2str_sal((val >> 12) & 0xffff, display_base, 16);
		ret_val += ", Version=" + int2str_sal((val >> 28) & 0xf, display_base, 4);
		break;
	case SWDR_DP_ABORT:
		ret_val  = std::string("ORUNERRCLR=") + ((val & 16) ? "1" : "0");
		ret_val += std::string(", WDERRCLR=") + ((val & 8) ? "1" : "0");
		ret_val += std::string(", STKERRCLR=") + ((val & 4) ? "1" : "0");
		ret_val += std::string(", STKCMPCLR=") + ((val & 2) ? "1" : "0");
		ret_val += std::string(", DAPABORT=") + ((val & 1) ? "1" : "0");
		break;

	case SWDR_DP_CTRL_STAT:
		ret_val  = std::string("CSYSPWRUPACK=") + ((val & (1<<31)) ? "1" : "0");
		ret_val += std::string(", CSYSPWRUPREQ=") + ((val & (1<<30)) ? "1" : "0");
		ret_val += std::string(", CDBGPWRUPACK=") + ((val & (1<<29)) ? "1" : "0");
		ret_val += std::string(", CDBGPWRUPREQ=") + ((val & (1<<28)) ? "1" : "0");
		ret_val += std::string(", CDBGRSTACK=") + ((val & (1<<27)) ? "1" : "0");
		ret_val += std::string(", CDBGRSTREQ=") + ((val & (1<<26)) ? "1" : "0");
		ret_val += ", TRNCNT=" + int2str_sal((val >> 12) & 0xfff, display_base, 12);
		ret_val += ", MASKLANE=" + int2str_sal((val >> 8) & 0xf, display_base, 4);
		ret_val += std::string(", WDATAERR=") + ((val & (1<<7)) ? "1" : "0");
		ret_val += std::string(", READOK=") + ((val & (1<<6)) ? "1" : "0");
		ret_val += std::string(", STICKYERR=") + ((val & (1<<5)) ? "1" : "0");
		ret_val += std::string(", STICKYCMP=") + ((val & (1<<4)) ? "1" : "0");
		ret_val += ", TRNMODE=";
		switch ((val >> 2) & 3)
		{
		case 0:		ret_val += "Normal";			break;
		case 1:		ret_val += "Pushed verify";		break;
		case 2:		ret_val += "Pushed compare";	break;
		case 3:		ret_val += "Reserved";			break;
		}
		ret_val += std::string(", STICKYORUN=") + ((val & (1<<1)) ? "1" : "0");
		ret_val += std::string(", ORUNDETECT=") + ((val & (1<<0)) ? "1" : "0");
		break;
	case SWDR_DP_WCR:
		ret_val  = "TURNAROUND=" + int2str_sal((val >> 8) & 3, display_base, 2) + " data period(s)";
		ret_val += ", WIREMODE=" + int2str_sal((val >> 4) & 0xf, display_base, 4);
		switch ((val >> 6) & 3)
		{
		case 0:		ret_val += "Asynchronous";	break;
		case 1:		ret_val += "Synchronous";	break;
		default:	ret_val += "Reserved";		break;
		}
		break;
	//case SWDR_DP_RESEND:		break;		// just raw data
	case SWDR_DP_SELECT:
		ret_val  = "APSEL=" + int2str_sal((val >> 24) & 0xff, display_base);
		ret_val += ", APBANKSEL=" + int2str_sal((val >> 4) & 0xf, display_base, 4);
		ret_val += ", PRESCALER=" + int2str_sal(val & 0x3, display_base, 2);
		break;
	//case SWDR_DP_RDBUFF:		break;		// just raw data
	//case SWDR_DP_ROUTESEL:		break;		// just raw data

	// AP
	case SWDR_AP_IDR:
		ret_val  = "Revision=" + int2str_sal(val >> 28, display_base, 4);
		ret_val += ", JEP-106 continuation=" + int2str_sal((val >> 24) & 0xf, display_base, 4);
		ret_val += ", JEP-106 identity=" + int2str_sal((val >> 17) & 0x7f, display_base, 7);
		ret_val += std::string(", Class=") + ((val & (1<<16)) ? "This AP is a Memory Acces Port" : "This AP is not a Memory Acces Port");
		ret_val += ", AP Identfication=" + int2str_sal(val & 0xff, display_base);
		break;
	case SWDR_AP_CSW:
		ret_val  = std::string("DbgSwEnable=") + ((val & (1<<31)) ? "1" : "0");
		ret_val += ", Prot=" + int2str_sal((val >> 24) & 0x7f, display_base, 7);
		ret_val += std::string(", SPIDEN=") + ((val & (1<<23)) ? "1" : "0");
		ret_val += ", Mode=" + int2str_sal((val >> 8) & 0xf, display_base, 4);
		ret_val += std::string(", TrInProg=") + ((val & (1<<7)) ? "1" : "0");
		ret_val += std::string(", DeviceEn=") + ((val & (1<<6)) ? "1" : "0");
		ret_val += ", AddrInc=";
		switch ((val >> 4) & 0x3)
		{
		case 0:		ret_val += "Auto-increment off";	break;
		case 1:		ret_val += "Increment single";		break;
		case 2:		ret_val += "Increment packed";		break;
		case 3:		ret_val += "Reserved";				break;
		}
		ret_val += ", Size=";
		switch (val & 0x7)
		{
		case 0:		ret_val += "Byte (8 bits)";			break;
		case 1:		ret_val += "Halfword (16 bits)";	break;
		case 2:		ret_val += "Word (32 bits)";		break;
		default:	ret_val += "Reserved";				break;
		}

		break;
	// case SWDR_AP_TAR:			break;		// these are just raw data
	// case SWDR_AP_DRW:			break;
	// case SWDR_AP_BD0:			break;
	// case SWDR_AP_BD1:			break;
	// case SWDR_AP_BD2:			break;
	// case SWDR_AP_BD3:			break;
	case SWDR_AP_CFG:
		if (val & 1)
			ret_val = "Big-endian";
		else
			ret_val = "Little-endian";

		break;
	case SWDR_AP_BASE:
		ret_val += "BASEADDR=" + int2str_sal(val >> 12, display_base, 20);
		ret_val += std::string(", Format=") + ((val & (1<<1)) ? "1" : "0");
		ret_val += ", Entry present=";
		if (val & 1)
			ret_val += "Debug entry present";
		else
			ret_val += "No debug entry present";

		break;
	}

	return ret_val;
}

std::string int2str(const U8 i)
{
	char number_str[8];
	AnalyzerHelpers::GetNumberString(i, Decimal, 8, number_str, sizeof(number_str));
	return number_str;
}

std::string int2str_sal(const U64 i, DisplayBase base, const int max_bits)
{
	char number_str[256];
	AnalyzerHelpers::GetNumberString(i, base, max_bits, number_str, sizeof(number_str));
	return number_str;
}
