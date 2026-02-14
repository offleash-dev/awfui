#ifndef AF_ADAFRUIT_COMPAT_H
#define AF_ADAFRUIT_COMPAT_H

// =============================================================================
// Adafruit GFX Compatibility Wrapper
//
// This header safely includes Adafruit_GFX.h and resolves conflicts with the
// C++ standard library and compiler intrinsics. 
//
// Problems solved:
// 1. Arduino.h defines `abs` macro which breaks <intrin.h> on MSVC/MinGW
// 2. Adafruit_GFX.h defines `swap` macro which conflicts with std::swap
// 3. MSVC doesn't have GCC's __builtin_bswap* functions
//
// Solution: Include system/STL headers first, then Arduino/Adafruit, then
// undef the problematic macros.
// =============================================================================

// Include system intrinsics FIRST (before any Arduino macros can interfere)
#if defined(_WIN32) || defined(__MINGW32__)
#include <intrin.h>
#endif

// MSVC doesn't have GCC's __builtin_bswap* - provide equivalents
#ifdef _MSC_VER
#include <stdlib.h>
#define __builtin_bswap16(x) _byteswap_ushort(x)
#define __builtin_bswap32(x) _byteswap_ulong(x)
#define __builtin_bswap64(x) _byteswap_uint64(x)
#endif

// Include STL headers to get proper std::abs, std::swap, etc.
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <type_traits>
#include <utility>

// Suppress warnings from third-party Adafruit/Arduino headers
#if defined(_MSC_VER)
#define AF_THIRD_PARTY_WARNINGS_PUSH  __pragma(warning(push, 0))
#define AF_THIRD_PARTY_WARNINGS_POP   __pragma(warning(pop))
#elif defined(__GNUC__) || defined(__clang__)
#define AF_THIRD_PARTY_WARNINGS_PUSH \
    _Pragma("GCC diagnostic push") \
    _Pragma("GCC diagnostic ignored \"-Wall\"") \
    _Pragma("GCC diagnostic ignored \"-Wextra\"")
#define AF_THIRD_PARTY_WARNINGS_POP \
    _Pragma("GCC diagnostic pop")
#else
#define AF_THIRD_PARTY_WARNINGS_PUSH
#define AF_THIRD_PARTY_WARNINGS_POP
#endif

// Now include Adafruit GFX (which pulls in Arduino.h)
AF_THIRD_PARTY_WARNINGS_PUSH
#include <Adafruit_GFX.h>
AF_THIRD_PARTY_WARNINGS_POP

// Undefine problematic macros that conflict with standard library
#undef abs
#undef swap

// Provide safe alternative macros for Adafruit code that needs them
#define ADAFRUIT_SWAP(a, b)                                                                                            \
      {                                                                                                                \
            int16_t t = a;                                                                                             \
            a         = b;                                                                                             \
            b         = t;                                                                                             \
      }

#define ADAFRUIT_ABS(x) ((x) > 0 ? (x) : -(x))

#endif // AF_ADAFRUIT_COMPAT_H