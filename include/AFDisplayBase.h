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

#include "AFBase.h"



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


typedef void* AFCanvasData;

// Platform-agnostic canvas handle
typedef struct AFCanvas {
    uint16_t width;
    uint16_t height; 
    AFCanvasData data;  // Platform-specific (lgfx::Sprite*, uint16_t*, etc.)
} AFCanvas;




class AFDisplayBase {
public:
    virtual ~AFDisplayBase() = default;

    // functionality queries
    virtual bool supportsCanvas() const { return false; }
    virtual bool supportsFastFill() const { return false; }
    virtual bool supportsFastBitmap() const { return false; }

    // Initialize the display if needed
    virtual void begin() {}


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

                               
    // Fast drawing method fallbacks
    virtual void fastFillRectDMA(int x1, int y1, int x2, int y2, uint16_t color) {
        // Fallback to regular fillRect
        fillRect(x1, y1, x2 - x1 + 1, y2 - y1 + 1, color);
    }
    

    virtual void fastDrawBitmapDMA(int x, int y, const uint16_t* bitmap, 
                                   int16_t w, int16_t h) {
        // Fallback to regular RGB bitmap
        drawRGBBitmap(x, y, bitmap, w, h);
    }
    

    // Canvas management (optional, with fallbacks)

    // Create an off-screen canvas matching the display dimensions.
    // Returns a pointer to a heap-allocated AFCanvas that can be used with the AFDisplayBase methods to draw.
    // Caller owns the returned pointer.  Returns nullptr if not supported.
    // The created canvas should be freed with destroyCanvas when no longer needed.
    // The created canvas is not automatically set as the targert for drawing operations; call setCurrentCanvas to use it.
    virtual AFCanvas* createCanvas(uint16_t w, uint16_t h) { unused(w); unused(h); return nullptr; }
    virtual void destroyCanvas(AFCanvas* canvas) { unused(canvas); }
    
    virtual void setCurrentCanvas(AFCanvas* canvas) { m_currentCanvas = canvas; }
    virtual void startCanvasUpdate() { m_canvasNeedsUpdate = true; }
    virtual void endCanvasUpdate(bool copyToScreen = true) { unused(copyToScreen); }
    
    // Return the raw pixel buffer of a canvas, or nullptr if N/A.
    virtual AFCanvasData getCurrentCanvasBuffer() { return nullptr; }


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
    
    bool m_canvasNeedsUpdate = false;
    AFCanvas* m_currentCanvas = nullptr;
};