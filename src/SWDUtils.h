#pragma once

#ifdef _WINDOWS
# include <windows.h>
#endif

#include <string>
#include <LogicPublicTypes.h>

#include "SWDTypes.h"

// returns string descriptions of the register name and the bits with values
std::string GetRegisterName(SWDRegisters reg);
std::string GetRegisterValueDesc(SWDRegisters reg, U32 val, DisplayBase display_base);

std::string int2str_sal(const U64 i, DisplayBase base, const int max_bits = 8);
inline std::string int2str(const U64 i)
{
	return int2str_sal(i, Decimal, 64);
}

/*
// debugging helper functions -- Windows only!!!
inline void debug(const std::string& str)
{
#if !defined(NDEBUG)  &&  defined(_WINDOWS)
	::OutputDebugStringA(("----- " + str + "\n").c_str());
#endif
}

inline void debug(const char* str)
{
#if !defined(NDEBUG)  &&  defined(_WINDOWS)
	debug(std::string(str));
#endif
}
*/