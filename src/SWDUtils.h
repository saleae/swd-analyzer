#pragma once

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <string>
#include <LogicPublicTypes.h>

#include "SWDTypes.h"

// returns string descriptions of the register name and the bits with values
std::string GetRegisterName( SWDRegisters reg );
std::string GetRegisterValueDesc( SWDRegisters reg, U32 val, DisplayBase displayBase, DPVersion version );

std::string Int2StrSal( const U64 i, DisplayBase base, const U32 maxBits = 8 );
inline std::string Int2Str( const U64 i )
{
    return Int2StrSal( i, Decimal, 64 );
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