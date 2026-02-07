#pragma once

//// AFDisplayInterface.h
//// Part of the AWFUI library
////
//// Abstract display interface for AWFUI.
//// Decouples the framework from any specific graphics library (Adafruit_GFX,
//// TFT_eSPI, etc.).  Backend implementations provide the concrete class.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>



class AFDisplayInterface {
public:
    virtual ~AFDisplayInterface() = default;

    // --- Dimensions & orientation ---
    virtual int16_t width()  const = 0;
    virtual int16_t height() const = 0;
    virtual uint8_t getRotation() const = 0;
    virtual void    setRotation(uint8_t r) = 0;

    // --- Drawing primitives ---
    virtual void fillScreen(uint16_t color) = 0;
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) = 0;
    virtual void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) = 0;
    virtual void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) = 0;
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) = 0;
    virtual void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) = 0;
    virtual void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) = 0;

    // --- Text ---
    virtual void setCursor(int16_t x, int16_t y) = 0;
    virtual void setTextColor(uint16_t color) = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual void print(const char* text) = 0;
    virtual void getTextBounds(const char* str, int16_t x, int16_t y,
                               int16_t* x1, int16_t* y1,
                               uint16_t* w, uint16_t* h) = 0;

    // --- Canvas (off-screen buffer) support ---
    // Create an off-screen canvas matching the display dimensions.
    // Returns a heap-allocated AFDisplayInterface that draws to a buffer.
    // Caller owns the returned pointer.  Returns nullptr if not supported.
    virtual AFDisplayInterface* createCanvas() { return nullptr; }

    // Return the raw RGB565 pixel buffer of a canvas, or nullptr if N/A.
    virtual const uint16_t* getCanvasBuffer() const { return nullptr; }

    // Push an RGB565 bitmap to the display at (x, y).
    // Default implementation is a no-op; backends with canvas support override.
    virtual void drawRGBBitmap(int16_t x, int16_t y,
                               const uint16_t* bitmap,
                               int16_t w, int16_t h) {
        (void)x; (void)y; (void)bitmap; (void)w; (void)h;
    }
};
