#ifndef AF_ADAFRUIT_COMPAT_H
#define AF_ADAFRUIT_COMPAT_H

// =============================================================================
// Adafruit GFX Compatibility Wrapper
//
// This header safely includes Adafruit_GFX.h and resolves conflicts with the
// C++ standard library. The Adafruit library defines a macro `swap(a, b)` that
// collides with std::swap. We include STL headers first, then undef the macro.
// =============================================================================

// Include STL headers first to get proper std::swap
#include <algorithm>
#include <type_traits>
#include <utility>

// Now include Adafruit GFX
#include <Adafruit_GFX.h>


// Undefine the problematic macro that conflicts with std::swap
#undef swap

// Provide a safe alternative macro for Adafruit code that needs it
#define ADAFRUIT_SWAP(a, b)                                                                                            \
      {                                                                                                                \
            int16_t t = a;                                                                                             \
            a         = b;                                                                                             \
            b         = t;                                                                                             \
      }

#endif // AF_ADAFRUIT_COMPAT_H
