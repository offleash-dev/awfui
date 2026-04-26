#pragma once

//// AFCompilerCompat.h
//// Part of the AWFUI library
////
//// Compiler compatibility definitions for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


// =============================================================================
// Compiler Compatibility - to be included before all source files to manage 
// inconsistent definitions between platforms and compilers.
// =============================================================================


// MSVC doesn't have GCC's __builtin_bswap* - provide equivalents
#ifdef _MSC_VER
#include <stdlib.h>
#define __builtin_bswap16(x) _byteswap_ushort(x)
#define __builtin_bswap32(x) _byteswap_ulong(x)
#define __builtin_bswap64(x) _byteswap_uint64(x)
#endif
