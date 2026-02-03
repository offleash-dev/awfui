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
