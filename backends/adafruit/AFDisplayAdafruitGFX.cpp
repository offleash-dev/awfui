//// AFDisplayAdafruitGFX.cpp
//// Implementation of the Adafruit_GFX display backend for AWFUI.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#ifdef HAVE_ADAFRUIT_ILI9341
#include <Adafruit_ILI9341.h>
#endif

#include "AFDisplayAdafruitGFX.h"



// Initialize line buffer when screen size is known
void AFDisplayAdafruitGFX::initializeLineBuffer(int16_t screenWidth) {
    if (m_lineBuffer) free(m_lineBuffer);
    m_screenWidth = screenWidth;
    m_lineBuffer = (uint16_t*)malloc(screenWidth * 2);
}



void AFDisplayAdafruitGFX::fastFillRectDMA(int x1, int y1, int x2, int y2, uint16_t color) {
#ifdef HAVE_ADAFRUIT_ILI9341
    if (supportsFastFill() && m_lineBuffer != nullptr) {
        int w = x2 - x1 + 1;
        int h = y2 - y1 + 1;
 
        // Cast to ILI9341 to access hardware-specific methods
        Adafruit_ILI9341* ili9341 = static_cast<Adafruit_ILI9341*>(&m_gfx);
        
        // Use ILI9341's setAddrWindow instead of set_window
        ili9341->setAddrWindow(x1, y1, x2, y2);
 
        // Fill line buffer with color (only once per fill)
        for (int x = 0; x < w; x++)
            m_lineBuffer[x] = color;
 
        // Use ILI9341's SPI methods instead of spi_dma_transmit/wait_for_dma
        for (int y = 0; y < h; y++) {
            // Write the line buffer using ILI9341's writedata
            for (int x = 0; x < w; x++) {
                ili9341->writedata(m_lineBuffer[x] >> 8);  // High byte
                ili9341->writedata(m_lineBuffer[x] & 0xFF); // Low byte
            }
        }
        
        ili9341->spiFlush();  // Flush the SPI buffer
    }
    else
#endif
    {
        // Fallback to regular fill if no ILI9341 support or no buffer
        m_gfx.fillRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, color);
    }
}


void AFDisplayAdafruitGFX::drawRGBBitmap(int16_t x, int16_t y, const uint16_t* pixmap, int16_t w, int16_t h) {
        if (supportsFastBitmap() && supportsFastBitmap()) {
            fastDrawBitmapDMA(x, y, pixmap, w, h);
            return;
        }

#if HAS_DRAWRGBBITMAP
        // Newer Adafruit_GFX has drawRGBBitmap
        m_gfx.drawRGBBitmap(x, y, const_cast<uint16_t*>(pixmap), w, h);
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
