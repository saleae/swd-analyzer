#ifndef SWD_UTILS_H
#define SWD_UTILS_H

#ifdef _WINDOWS
#include <windows.h>
#endif

#include <string>
#include <LogicPublicTypes.h>

#include "SWDTypes.h"

bool IsApReg( const SWDRegisters reg );
bool IsApDataReg( const SWDRegisters reg );
U32 GetMemAddr( const SWDRegisters dataReg, const U32 tar );

// returns string descriptions of the register name and the bits with values
std::string GetRegisterName( const SWDRegisters reg );
std::string GetRegisterValueDesc( const SWDRegisters reg, const U32 val, const DisplayBase displayBase, const DPVersion version,
                                  const bool isRead );
std::string GetReadRegisterValueDesc( const SWDRegistersUnion& swdRegCouple, const U32 data, const DisplayBase displayBase,
                                     const DPVersion version );
std::string GetWriteRegisterValueDesc( const SWDRegistersUnion& swdRegCouple, const U32 data, const DisplayBase displayBase,
                                      const DPVersion version );

std::string Int2StrSal( const U64 i, DisplayBase base, const U32 maxBits = 8 );
inline std::string Int2Str( const U64 i )
{
    return Int2StrSal( i, Decimal, 64 );
}



// Template function that generates a bitmask of length 'onecount'.
// The generated bitmask has ones in the least significant 'onecount' bits.
template <typename T>
static constexpr T BitMask( const uint8_t onecount )
{
    if( onecount == 0u )
    {
        return static_cast<T>( 0u );
    }
    else
    {
        if( onecount > ( sizeof( T ) * CHAR_BIT ) )
        {
            return ~static_cast<T>( 0u );
        }
        else
        {
            return ( ( ~static_cast<T>( 0u ) ) >> ( ( sizeof( T ) * CHAR_BIT ) - onecount ) );
        }
    }
}

// Template function that generates a bitmask from range 'lsb' to 'msb'.
template <typename T>
static constexpr T BitMaskRange( const uint8_t msb, const uint8_t lsb )
{
    return BitMask<T>( msb ) ^ BitMask<T>( lsb );
}

struct RegisterFieldDescription
{
    const U8 Access; // Register version and access bitmask
    const U8 FiledMSB; // MSB bit number of bitfield
    const U8 FiledLSB; // LSB bit number of bitfield
    const std::string FieldName;
    const std::map<U32, std::string> ValuesDesc; // Values description if any :)
    std::string GetStrFieldValue( const U32 regValue, DisplayBase base ) const;
  private:
    const U32 GetFieldValue( const U32 regValue ) const;
};

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

#endif // SWD_UTILS_H
