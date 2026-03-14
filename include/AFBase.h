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


#include <cstddef>
#include <stdint.h>

#include "AFResult.h"
#include "AFVersion.h"


constexpr size_t MAX_WIDGETS_PER_PANEL  = 24;
constexpr size_t MAX_WIDGETS_PER_SCREEN = 24;
constexpr size_t MAX_PANELS_PER_SCREEN =  8;
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

class AFRect {
public:
    AFRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1) : x(x1), y(y1), w(w1), h(h1) {} 
    int16_t x, y, w, h;

    void inset(int16_t count = 1) { x+=count; y+=count; w-=count*2; h-=count*2; }
};



enum AFJustification {
      AFJustificationCenter,
      AFJustificationLeft,
      AFJustificationRight
};



// Helpers to create 4-character IDs used for widgets, screens, dialogs, etc.
// Example: constexpr uint32_t myID = MAKE_ID('T','E','S','T');
#define MAKE_ID(a,b,c,d) \ ((uint32_t)(a)<<24 | (uint32_t)(b)<<16 | (uint32_t)(c)<<8 | (uint32_t)(d))



// Example: constexpr uint32_t myID = makeID("TEST");
constexpr uint32_t makeID(const char (&s)[5]) {
    return (uint32_t(s[0]) << 24) |
           (uint32_t(s[1]) << 16) |
           (uint32_t(s[2]) << 8)  |
           (uint32_t(s[3]));
}



// Example: char idOut5[5];
//         getAsChars(makeID("TEST"), idOut5);
// will fill idOut5 with {'T', 'E', 'S', 'T', '\0'}
constexpr void getAsChars(int32_t id, char* out5) {
    out5[0] = (char)((id >> 24) & 0xFF);
    out5[1] = (char)((id >> 16) & 0xFF);
    out5[2] = (char)((id >> 8)  & 0xFF);
    out5[3] = (char)( id        & 0xFF);
    out5[4] = '\0';
}



extern char* getAsChars(int32_t id);



// Macro to mark unused parameters and avoid compiler warnings
#define unused(x) (void) (x)