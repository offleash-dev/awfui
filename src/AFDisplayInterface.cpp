//// AFDisplayInterface.cpp
//// Part of the AWFUI library
////
//// Implementation of display interface for AWFUI.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFDisplayInterface.h"



void AFDisplayInterface::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)   {
      drawHLine(x, y, w, color);
      drawHLine(x, y + h - 1, w, color);
      drawVLine(x, y, h, color);
      drawVLine(x + w - 1, y, h, color);
}



void AFDisplayInterface::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)  {
      int16_t dx = (int16_t)abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
      int16_t dy = (int16_t)(-abs(y1 - y0)), sy = y0 < y1 ? 1 : -1;
      int16_t err = dx + dy, e2;

      while (true) {
            drawPixel(x0, y0, color);
            if (x0 == x1 && y0 == y1) 
                  break;
            e2 = 2 * err;
            if (e2 >= dy) { 
                  err += dy; 
                  x0 += sx; 
            }
            if (e2 <= dx) { 
                err += dx; 
                y0 += sy; 
            }
      }
}


void AFDisplayInterface::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color)  {
      int16_t f = 1 - r;
      int16_t ddF_x = 1;
      int16_t ddF_y = -2 * r;
      int16_t xi = 0;
      int16_t yi = r;

      drawPixel(x, y + r, color);
      drawPixel(x, y - r, color);
      drawPixel(x + r, y, color);
      drawPixel(x - r, y, color);

      while (xi < yi) {
            if (f >= 0) {
                  yi--;
                  ddF_y += 2;
                  f += ddF_y;
              }
              xi++;
              ddF_x += 2;
              f += ddF_x;

              drawPixel(x + xi, y + yi, color);
              drawPixel(x - xi, y + yi, color);
              drawPixel(x + xi, y - yi, color);
              drawPixel(x - xi, y - yi, color);
              drawPixel(x + yi, y + xi, color);
              drawPixel(x - yi, y + xi, color);
              drawPixel(x + yi, y - xi, color);
              drawPixel(x - yi, y - xi, color);
      }
}



void AFDisplayInterface::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h,
                               int16_t radius, uint16_t color)
    {
        int16_t maxR = ((w < h) ? w : h) / 2;
        if (radius > maxR) radius = maxR;

        // Horizontal edges (between the rounded corners)
        drawHLine(x + radius, y,         w - 2 * radius, color);  // top
        drawHLine(x + radius, y + h - 1, w - 2 * radius, color);  // bottom
        // Vertical edges
        drawVLine(x,         y + radius, h - 2 * radius, color);  // left
        drawVLine(x + w - 1, y + radius, h - 2 * radius, color);  // right

        // Four quarter-circles (midpoint algorithm)
        int16_t cx1 = x + radius;          // top-left center
        int16_t cy1 = y + radius;
        int16_t cx2 = x + w - 1 - radius;  // top-right center
        int16_t cy2 = y + h - 1 - radius;  // bottom-left/right center y

        int16_t f     = 1 - radius;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * radius;
        int16_t xi    = 0;
        int16_t yi    = radius;

        while (xi < yi) {
            if (f >= 0) { yi--; ddF_y += 2; f += ddF_y; }
            xi++; ddF_x += 2; f += ddF_x;

            // top-right quarter
            drawPixel(cx2 + xi, cy1 - yi, color);
            drawPixel(cx2 + yi, cy1 - xi, color);
            // top-left quarter
            drawPixel(cx1 - xi, cy1 - yi, color);
            drawPixel(cx1 - yi, cy1 - xi, color);
            // bottom-right quarter
            drawPixel(cx2 + xi, cy2 + yi, color);
            drawPixel(cx2 + yi, cy2 + xi, color);
            // bottom-left quarter
            drawPixel(cx1 - xi, cy2 + yi, color);
            drawPixel(cx1 - yi, cy2 + xi, color);
        }
    }



void AFDisplayInterface::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {
      int16_t maxR = ((w < h) ? w : h) / 2;
      if (radius > maxR) radius = maxR;

      // Central rectangle (full height, between corners)
      fillRect(x + radius, y, w - 2 * radius, h, color);

      // Two side rectangles (between the rounded parts)
      fillRect(x, y + radius, radius, h - 2 * radius, color);
      fillRect(x + w - radius, y + radius, radius, h - 2 * radius, color);

      // Four filled quarter-circles at corners
      int16_t cx1 = x + radius;
      int16_t cy1 = y + radius;
      int16_t cx2 = x + w - 1 - radius;
      int16_t cy2 = y + h - 1 - radius;

      int16_t f     = 1 - radius;
      int16_t ddF_x = 1;
      int16_t ddF_y = -2 * radius;
      int16_t xi    = 0;
      int16_t yi    = radius;

      while (xi < yi) {
            if (f >= 0) { 
                  yi--; 
                  ddF_y += 2; 
                  f += ddF_y; 
            }
            xi++; 
            ddF_x += 2; 
            f += ddF_x;

            // Horizontal spans for each pair of symmetric points
            drawHLine(cx2, cy1 - yi, xi + 1, color);  // top-right
            drawHLine(cx1 - xi, cy1 - yi, xi + 1, color);  // top-left
            drawHLine(cx2, cy2 + yi, xi + 1, color);  // bottom-right
            drawHLine(cx1 - xi, cy2 + yi, xi + 1, color);  // bottom-left

            drawHLine(cx2, cy1 - xi, yi + 1, color);
            drawHLine(cx1 - yi, cy1 - xi, yi + 1, color);
            drawHLine(cx2, cy2 + xi, yi + 1, color);
            drawHLine(cx1 - yi, cy2 + xi, yi + 1, color);
      }
}



void AFDisplayInterface::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
      // Midpoint filled circle using horizontal spans
      drawVLine(x, y - r, 2 * r + 1, color);  // center column

      int16_t f     = 1 - r;
      int16_t ddF_x = 1;
      int16_t ddF_y = -2 * r;
      int16_t xi    = 0;
      int16_t yi    = r;

      while (xi < yi) {
            if (f >= 0) { 
                  yi--; 
                  ddF_y += 2; 
                  f += ddF_y; 
            }
            xi++; ddF_x += 2; f += ddF_x;

            drawHLine(x - xi, y + yi, 2 * xi + 1, color);
            drawHLine(x - xi, y - yi, 2 * xi + 1, color);
            drawHLine(x - yi, y + xi, 2 * yi + 1, color);
            drawHLine(x - yi, y - xi, 2 * yi + 1, color);
      }
}


void AFDisplayInterface::getTextBounds(const char* str, int16_t x, int16_t y,
                               int16_t* x1, int16_t* y1,
                               uint16_t* w, uint16_t* h) {
      // crude fallback: assume fixed-width 6x8 font
      uint16_t len = (uint16_t)strlen(str);
      *x1 = x;
      *y1 = y - 8;
      *w  = len * 6;
      *h  = 8;
}


void AFDisplayInterface::fillScreen(uint16_t color) {
        fillRect(0, 0, width(), height(), color);
}


void AFDisplayInterface::drawTextJustified(const char* text,
                           int16_t x, int16_t y, int16_t w, int16_t h,
                           AFJustification justification) {
      if (!text || text[0] == '\0') 
          return;

      int16_t  x1, y1;
      uint16_t tw, th;
      getTextBounds(text, 0, 0, &x1, &y1, &tw, &th);

      int16_t tx;
      switch (justification) {
            case AFJustificationCenter:
                  tx = x + (w - tw) / 2;
                  break;
            case AFJustificationRight:
                  tx = x + w - tw;
                  break;
            case AFJustificationLeft:
            default:
                  tx = x;
                  break;
      }

      int16_t ty = y + (h - th) / 2;

      setCursor(tx, ty);
      print(text);
}
