#pragma once

//// AFDisplayLovyanGFX.h
//// Part of the AWFUI library
////
//// AFDisplayInterface backend that wraps LovyanGFX.
//// Use this when your display driver uses LovyanGFX (LGFX).
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFDisplayInterface.h"
#include <LovyanGFX.hpp>

class AFDisplayLovyanGFX : public AFDisplayInterface {
public:
    explicit AFDisplayLovyanGFX(lgfx::LGFX& gfx) : m_gfx(gfx) {}

    // Initialize the display
    void begin() {
        // This is a no-op
        // The actual display initialization should be done by the specific driver
    }

    // --- Dimensions & orientation ---
    int16_t width()  const override { return m_gfx.width();  }
    int16_t height() const override { return m_gfx.height(); }
    uint8_t getRotation() const override { return m_gfx.getRotation(); }
    void    setRotation(uint8_t r) override { m_gfx.setRotation(r); }

    // --- Core drawing primitives (from AFDisplayBase) ---
    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        m_gfx.drawPixel(x, y, color);
    }

    void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override {
        m_gfx.drawFastHLine(x, y, w, color);
    }

    void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override {
        m_gfx.drawFastVLine(x, y, h, color);
    }

    // --- Higher-level drawing (overrides AFDisplayInterface fallbacks) ---
    void fillScreen(uint16_t color) override {
        m_gfx.fillScreen(color);
    }

    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override {
        // Check if this is a full screen fill - optimize it!
        if (x == 0 && y == 0 && w == m_gfx.width() && h == m_gfx.height()) {
            fillScreen(color);
            return;
        }
        
        // Use default implementation for other rectangles
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

    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override {
        m_gfx.drawLine(x0, y0, x1, y1, color);
    }

    void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override {
        m_gfx.drawCircle(x, y, r, color);
    }

    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override {
        m_gfx.fillCircle(x, y, r, color);
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

    // --- Bitmap drawing ---
    void drawBitmap(int16_t x, int16_t y,
                    const uint8_t* bitmap,
                    int16_t w, int16_t h,
                    uint16_t color) override {
        m_gfx.drawBitmap(x, y, bitmap, w, h, color);
    }

    void drawBitmap(int16_t x, int16_t y,
                    const uint8_t* bitmap,
                    int16_t w, int16_t h,
                    uint16_t color, uint16_t bg) override {
        m_gfx.drawBitmap(x, y, bitmap, w, h, color, bg);
    }

    // --- RGB bitmap / canvas support ---
    void drawRGBBitmap(int16_t x, int16_t y,
                       const uint16_t* bitmap,
                       int16_t w, int16_t h) override {
        m_gfx.pushImage(x, y, w, h, bitmap);
    }

    // --- Canvas support ---
    AFDisplayInterface* createCanvas() override;
    const uint16_t* getCanvasBuffer() const override;

    // Access the underlying LovyanGFX for backend-specific operations
    lgfx::LGFX& getLGFX() { return m_gfx; }

private:
    lgfx::LGFX& m_gfx;
};

// Canvas wrapper: owns a buffer and LovyanGFX sprite
class AFCanvasLovyanGFX : public AFDisplayLovyanGFX {
public:
    AFCanvasLovyanGFX(int16_t w, int16_t h)
        : AFDisplayLovyanGFX(m_sprite), m_sprite(&m_parentLGFX), m_width(w), m_height(h) {
        // Create the sprite with the specified dimensions
        m_sprite.createSprite(w, h);
        // Clear to black
        m_sprite.fillScreen(TFT_BLACK);
    }

    ~AFCanvasLovyanGFX() {
        m_sprite.deleteSprite();
    }

    // Override width/height to return canvas dimensions
    int16_t width() const override { return m_width; }
    int16_t height() const override { return m_height; }

    const uint16_t* getCanvasBuffer() const override {
        return m_sprite.getBuffer();
    }

    // Render canvas to parent display at specified position
    void pushToDisplay(int16_t x, int16_t y) {
        m_sprite.pushSprite(&m_parentLGFX, x, y);
    }

    // Render canvas to parent display at origin
    void pushToDisplay() {
        m_sprite.pushSprite(&m_parentLGFX, 0, 0);
    }

private:
    lgfx::LGFX m_parentLGFX;  // Dummy parent for sprite constructor
    lgfx::Sprite m_sprite;
    int16_t m_width;
    int16_t m_height;
};
