//// AFDisplayAdafruitGFX.cpp
//// Implementation of the Adafruit_GFX display backend for AWFUI.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFDisplayAdafruitGFX.h"



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



// Initialize line buffer when screen size is known
void AFDisplayAdafruitGFX::createLineBuffer(int16_t screenWidth) {
    if (m_lineBuffer) free(m_lineBuffer);
    m_screenWidth = screenWidth;
    m_lineBuffer = (uint16_t*)malloc(screenWidth * 2);
}



void AFDisplayAdafruitGFX::fastFillRectDMA(int x1, int y1, int x2, int y2, uint16_t color) {
    if (m_lineBuffer != nullptr) {
        int w = x2 - x1 + 1;
        int h = y2 - y1 + 1;

        set_window(x1, y1, x2, y2);

        // Fill line buffer with color (only once per fill)
        for (int x = 0; x < w; x++)
            m_lineBuffer[x] = color;

        // DMA each line
        for (int y = 0; y < h; y++) {
            spi_dma_transmit((uint8_t*)m_lineBuffer, w * 2);
        }
        
        wait_for_dma();
    }
    else {
        // Fallback to regular fill if no buffer
        m_gfx.fillRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, color);
    }
}



void AFDisplayAdafruitGFX::drawRGBBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t* pixmap) {
        if (supportsFastBitmap()) {
            fastDrawBitmapDMA(x, y, w, h, pixmap);
            return;
        }

#if HAS_DRAWRGBBITMAP
        // Newer Adafruit_GFX has drawRGBBitmap
        m_gfx.drawRGBBitmap(x, y, const_cast<uint16_t*>(bitmap), w, h);
#else
        // Method not available in older Adafruit_GFX version
        // Convert RGB565 to 8-bit grayscale and draw as regular bitmap
        for (int16_t row = 0; row < h; row++) {
            for (int16_t col = 0; col < w; col++) {
                uint16_t pixel = bitmap[row * w + col];
                // Convert RGB565 to 8-bit grayscale (simple approximation)
                uint8_t gray = ((pixel >> 11) & 0x1F) * 8/32 + 
                               ((pixel >> 5) & 0x3F) * 4/64 + 
                               (pixel & 0x1F) * 8/32;
                m_gfx.drawPixel(x + col, y + row, gray);
            }
        }
#endif
    }

  

// Optimized fillScreen for full screen - uses bulk SPI transfers
//
void AFDisplayAdafruitGFX::fillScreenOptimized(uint16_t color) {
    // Get screen dimensions
    int16_t width = m_gfx.width();
    int16_t height = m_gfx.height();

    if (m_lineBuffer != nullptr) {
        fastFillRectDMA(0, 0, width - 1, height - 1, color);
        return;
    }
   
    m_gfx.fillScreen(color);
}
