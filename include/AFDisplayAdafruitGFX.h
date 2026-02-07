#pragma once

//// AFDisplayAdafruitGFX.h
//// Part of the AWFUI library
////
//// AFDisplayInterface backend that wraps Adafruit_GFX.
//// Use this when your display driver inherits from Adafruit_GFX
//// (e.g. Adafruit_ILI9341, GFX_SDL).
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFAdafruitCompat.h"
#include "AFDisplayInterface.h"



class AFDisplayAdafruitGFX : public AFDisplayInterface {
public:
    explicit AFDisplayAdafruitGFX(Adafruit_GFX& gfx) : m_gfx(gfx) {}

    // --- Dimensions & orientation ---
    int16_t width()  const override { return m_gfx.width();  }
    int16_t height() const override { return m_gfx.height(); }
    uint8_t getRotation() const override { return m_gfx.getRotation(); }
    void    setRotation(uint8_t r) override { m_gfx.setRotation(r); }

    // --- Drawing primitives ---
    void fillScreen(uint16_t color) override {
        m_gfx.fillScreen(color);
    }

    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override {
        m_gfx.fillRect(x, y, w, h, color);
    }

    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override {
        m_gfx.drawRect(x, y, w, h, color);
    }

    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) override {
        m_gfx.fillRoundRect(x, y, w, h, radius, color);
    }

    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) override {
        m_gfx.drawRoundRect(x, y, w, h, radius, color);
    }

    // --- Text ---
    void setCursor(int16_t x, int16_t y) override {
        m_gfx.setCursor(x, y);
    }

    void setTextColor(uint16_t color) override {
        m_gfx.setTextColor(color);
    }

    void setTextSize(uint8_t size) override {
        m_gfx.setTextSize(size);
    }

    void print(const char* text) override {
        m_gfx.print(text);
    }

    void getTextBounds(const char* str, int16_t x, int16_t y,
                       int16_t* x1, int16_t* y1,
                       uint16_t* w, uint16_t* h) override {
        m_gfx.getTextBounds(str, x, y, x1, y1, w, h);
    }

    // --- Bitmap / canvas support ---
    void drawRGBBitmap(int16_t x, int16_t y,
                       const uint16_t* bitmap,
                       int16_t w, int16_t h) override {
        m_gfx.drawRGBBitmap(x, y, const_cast<uint16_t*>(bitmap), w, h);
    }

    // --- Canvas support ---
    AFDisplayInterface* createCanvas() override;
    const uint16_t* getCanvasBuffer() const override;

    // Access the underlying Adafruit_GFX for backend-specific operations
    Adafruit_GFX& getGFX() { return m_gfx; }

private:
    Adafruit_GFX& m_gfx;
};



// Canvas wrapper: an AFDisplayAdafruitGFX that owns a GFXcanvas16
class AFCanvasAdafruitGFX : public AFDisplayAdafruitGFX {
public:
    AFCanvasAdafruitGFX(int16_t w, int16_t h)
        : AFDisplayAdafruitGFX(m_canvas), m_canvas(w, h) {}

    const uint16_t* getCanvasBuffer() const override {
        return m_canvas.getBuffer();
    }

private:
    GFXcanvas16 m_canvas;
};
