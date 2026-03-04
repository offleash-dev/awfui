//// AFDisplayAdafruitGFX.cpp
//// Implementation of the Adafruit_GFX display backend for AWFUI.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFDisplayAdafruitGFX.h"

// Static canvas buffer for embedded systems with limited heap
#if !HAS_GFXCANVAS16
uint16_t AFCanvasAdafruitGFX::s_canvasBuffer[120 * 160];
#endif



// Create an off-screen canvas matching the display dimensions
//
AFDisplayInterface* AFDisplayAdafruitGFX::createCanvas() {
    int16_t w = width();
    int16_t h = height();
    if (w > 0 && h > 0) 
           return new AFCanvasAdafruitGFX(w, h);
    else
           return nullptr;
}



// Base wrapper has no canvas buffer
//
const uint16_t* AFDisplayAdafruitGFX::getCanvasBuffer() const {
    return nullptr;
}

// Optimized fillScreen for full screen - uses bulk SPI transfers
//
void AFDisplayAdafruitGFX::fillScreenOptimized(uint16_t color) {
    // Get screen dimensions
    int16_t width = m_gfx.width();
    int16_t height = m_gfx.height();
    
    // Only optimize for common ILI9341 screen sizes
    if (!((width == 240 && height == 320) || (width == 320 && height == 240))) {
        // Not a standard ILI9341 size, use fallback
        m_gfx.fillScreen(color);
        return;
    }
    
    // The key optimization: bypass Adafruit_GFX::fillRect which calls VLine for each column
    // Instead, call ILI9341::fillRect directly which does pixel-by-pixel in bulk
    
    // This works because of virtual function dispatch:
    // - m_gfx points to Adafruit_ILI9341
    // - Adafruit_ILI9341 overrides fillRect() with optimized version
    // - Calling m_gfx.fillRect() goes directly to ILI9341::fillRect(), NOT Adafruit_GFX::fillRect()
    
    // This is the optimization! We skip the terrible VLine-per-column approach
    // and go straight to the reasonably optimized ILI9341 pixel-by-pixel implementation
    
    m_gfx.fillRect(0, 0, width, height, color);
}
