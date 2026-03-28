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

#include "AFVersion.h"
#include "AFResult.h"
#include "AFIdConfig.h"


// Forward declarations
class AFWidget;
class AFPanel;


// Modify these for restraining memory usage in small UIs. 
//These limits are per screen/panel/dialog, not global.
constexpr size_t MAX_WIDGETS_PER_PANEL  = 32;
constexpr size_t MAX_WIDGETS_PER_SCREEN = 32;
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



// this will safely convert the id from a 4 bytes to a usable char string
// the 5 character memory is common for every call this, so it only works
// one ID at a time.    
extern char* getIdAsCharPtr(uint32_t id);



// Macro to mark unused parameters and avoid compiler warnings
#define unused(x) (void) (x)