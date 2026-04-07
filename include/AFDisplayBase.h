#pragma once

//// AFDisplayBase.h
//// Part of the AWFUI library
////
//// Abstract display interface for AWFUI.
//// Decouples the framework from any specific graphics library (Adafruit_GFX,
//// TFT_eSPI, etc.).  Backend implementations provide the concrete class.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include <stdint.h>
#include <stddef.h>



class AFDisplayInterface;


// Callback types for drawing on a shared hardware bus.
// Eg. these are used to manage locking of the SPI bus when multiple components need to access it.
// If defined, AWFUI will call the hold callback before drawing operations and the release callback after, 
// to allow the backend to manage bus access appropriately.  
// They are optional if you manage the bus on your own or the display is not shared with other components.
//
// Also note that these callbacks work for quick operations.  Long operation might be better handled by the host app directly,
// or you might need to prebuild a UI and cache its instead of building it on the fly.
using AFSharedDrawCallback = void (*)();


class AFDisplayBase {
public:
    virtual ~AFDisplayBase() = default;

    // --- Dimensions & orientation ---
    virtual int16_t width()  const = 0;
    virtual int16_t height() const = 0;

    virtual uint8_t getRotation() const { return 0; }
    virtual void    setRotation(uint8_t) {}

    // --- Core drawing primitives ---
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

    virtual void drawHLine(int16_t x, int16_t y,
                           int16_t w, uint16_t color) = 0;

    virtual void drawVLine(int16_t x, int16_t y,
                           int16_t h, uint16_t color) = 0;

    virtual void fillRect(int16_t x, int16_t y,
                          int16_t w, int16_t h,
                          uint16_t color) = 0;


    // --- Higher-level shapes (optional, with fallbacks) ---

    // --- Text ---
    virtual void setCursor(int16_t x, int16_t y) = 0;
    virtual void setTextColor(uint16_t color) = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual void print(const char* text) = 0;

    // --- Bitmap drawing ---
    // Draw a 1-bit bitmap at (x, y) with the given foreground color.
    // Each byte holds 8 horizontal pixels, MSB first.
    virtual void drawBitmap(int16_t x, int16_t y,
                            const uint8_t* bitmap,
                            int16_t w, int16_t h,
                            uint16_t color) = 0;

    // Draw a 1-bit bitmap with foreground and background colors.
    virtual void drawBitmap(int16_t x, int16_t y,
                            const uint8_t* bitmap,
                            int16_t w, int16_t h,
                            uint16_t color, uint16_t bg) = 0;

    // Push an RGB565 bitmap to the display at (x, y).
    // Default implementation is a no-op; backends with canvas support override.
    virtual void drawRGBBitmap(int16_t x, int16_t y,
                               const uint16_t* bitmap,
                               int16_t w, int16_t h) = 0;

    virtual bool supportsCanvas() const { return false; }                           

    // Create an off-screen canvas matching the display dimensions.
    // Returns a heap-allocated AFDisplayInterface that draws to a buffer.
    // Caller owns the returned pointer.  Returns nullptr if not supported.
    virtual AFDisplayInterface* createCanvas() { return nullptr; }

    // Return the raw RGB565 pixel buffer of a canvas, or nullptr if N/A.
    virtual const uint16_t* getCanvasBuffer() const { return nullptr; }



    // --- DMA-accelerated primitives (optional) ---
    // This is exploratory and as much a thought process.
    // It maybe expanded or revised as needed for to find a standard for multiple platforms.
    
    // Query if DMA acceleration is available on this backend
    virtual bool isDMAAvailable() const { return false; }

    // DMA-accelerated solid fill for large rectangles (screen clears, dialog backgrounds, etc.)
    // Backends with DMA support should override this for significant performance gains.
    // Default fallback uses standard fillRect.
    virtual void fastFillRectDMA(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
        fillRect(x, y, w, h, color);
    }

    // DMA-accelerated pixel buffer push for canvas/bitmap operations
    // Pushes 'count' RGB565 pixels from 'data' to the current drawing window.
    // Caller must set the drawing window (via setAddrWindow or equivalent) before calling.
    // No default fallback - backends must implement if they support this.
    virtual void pushPixelsDMA(const uint16_t* data, size_t count) {
        // No generic fallback - backend-specific
        (void)data;
        (void)count;
    }


    // Set SPI bus locking callbacks for drawing operations
    // Called before/after display rendering to aquire and hold the SPI bus exclusively
    void setSharedDrawCallbacks(AFSharedDrawCallback aquire, AFSharedDrawCallback release) {
        m_aquireDrawCallback = aquire;
        m_releaseDrawCallback = release;
    }


    void sharedDrawAquire() {
        if (m_aquireDrawCallback) {
            m_aquireDrawCallback();
        }
    }


    void sharedDrawRelease() {
        if (m_releaseDrawCallback) {
            m_releaseDrawCallback();
        }
    }
    

protected:
    AFSharedDrawCallback m_aquireDrawCallback = nullptr;
    AFSharedDrawCallback m_releaseDrawCallback = nullptr;
};