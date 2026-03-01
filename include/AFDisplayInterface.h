#pragma once

//// AFDisplayInterface.h
//// Part of the AWFUI library
////
//// Base full interface for AWFUI.
//// Decouples the framework from any specific graphics library (Adafruit_GFX,
//// TFT_eSPI, etc.).  Default provides an unoptimized functional implementation.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <stdint.h>
#include <stddef.h>
#include <cmath>
#include <cstring>

#include "AFBase.h"
#include "AFDisplayBase.h"



class AFDisplayInterface : public AFDisplayBase {
public:
      virtual void drawRect(int16_t x, int16_t y,
                          int16_t w, int16_t h,
                          uint16_t color);

      virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

      virtual void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color);

      virtual void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               int16_t radius, uint16_t color);

      virtual void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               int16_t radius, uint16_t color);

    
      virtual void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color);


      virtual void getTextBounds(const char* str, int16_t x, int16_t y,
                               int16_t* x1, int16_t* y1,
                               uint16_t* w, uint16_t* h);
 
      virtual void fillScreen(uint16_t color);


      void drawTextJustified(const char* text,
                           int16_t x, int16_t y, int16_t w, int16_t h,
                           AFJustification justification);
};