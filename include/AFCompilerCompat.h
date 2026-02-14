#pragma once

//// AFCheckbox.h
//// Part of the AWFUI library
////
//// Lightweight checkbox widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.
#ifndef AF_COMPILER_COMPAT_H
#define AF_COMPILER_COMPAT_H

// =============================================================================
// Compiler Compatibility - Force-included before all source files
// =============================================================================

// MSVC doesn't have GCC's __builtin_bswap* - provide equivalents
#ifdef _MSC_VER
#include <stdlib.h>
#define __builtin_bswap16(x) _byteswap_ushort(x)
#define __builtin_bswap32(x) _byteswap_ulong(x)
#define __builtin_bswap64(x) _byteswap_uint64(x)
#endif

#endif // AF_COMPILER_COMPAT_H
