#pragma once

// AFBase.h
// The root header for AFUI — the place where you put:
//      Configuration constants for the AWFUI library
//      core typedefs
//      forward declarations
//      global constants
//      small enums that are used everywhere
//      basic macros (assert, min/max, compile‑time helpers)
//      fundamental types like AFPoint, AFSize, etc.


#include "AFResult.h"
#include "AFVersion.h"


constexpr size_t MAX_WIDGETS_PER_DIALOG = 32;
constexpr size_t MAX_WIDGETS_PER_SCREEN = 32;
constexpr size_t MAX_DIALOGS_PER_SCREEN = 8;
constexpr size_t MAX_SCREENS_PER_WORLD  = 8;



struct AFPoint {
      int16_t x;
      int16_t y;
};

struct AFSize {
      int16_t w;
      int16_t h;
};
