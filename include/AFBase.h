#pragma once

//// AFBase.h
//// Part of the AWFUI library
////
//// The root header for AWFUI — the place where you put:
////      Configuration constants for the AWFUI library
////      core typedefs
////      forward declarations
////      global constants
////      small enums that are used everywhere
////      basic macros (assert, min/max, compile‑time helpers)
////      fundamental types like AFPoint, AFSize, etc.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


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



enum AFJustification {
      AFJustificationCenter,
      AFJustificationLeft,
      AFJustificationRight
};



// Helpers to create 4-character IDs used for widgets, screens, dialogs, etc.
#define MAKE_ID(a,b,c,d) \ ((uint32_t)(a)<<24 | (uint32_t)(b)<<16 | (uint32_t)(c)<<8 | (uint32_t)(d))
// Example: constexpr uint32_t myID = MAKE_ID('T','E','S','T');

constexpr uint32_t makeID(const char (&s)[5]) {
    return (uint32_t(s[0]) << 24) |
           (uint32_t(s[1]) << 16) |
           (uint32_t(s[2]) << 8)  |
           (uint32_t(s[3]));
}
// Example: constexpr uint32_t myID = makeID("TEST");



// Macro to mark unused parameters and avoid compiler warnings
#define unused(x) (void) (x)