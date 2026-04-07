#pragma once

//// AFDisplayLovyanGFX.h
//// Part of the AWFUI library
////
//// AFDisplayInterface backend that wraps LovyanGFX.
//// Use this when your display driver uses LovyanGFX (LGFX).
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "../../include/AFDisplayInterface.h"
#include <LovyanGFX.hpp>
#include "../../include/AFBase.h"

class AFDisplayLovyanGFX : public AFDisplayInterface {
public:
    AFDisplayLovyanGFX() = default;
    explicit AFDisplayLovyanGFX(lgfx::LGFX_Device* gfx) : m_gfx(gfx) {}
    virtual ~AFDisplayLovyanGFX() = default;

    // Set the display reference
    void setDisplay(lgfx::LGFX_Device* gfx) { m_gfx = gfx; }

    // Initialize the display
    void begin() {
        if (m_gfx) m_gfx->begin();
    }

    // --- Dimensions & orientation ---
    int16_t width()  const override;
    int16_t height() const override;
    uint8_t getRotation() const override;
    void    setRotation(uint8_t r) override;

    // --- Core drawing primitives (from AFDisplayBase) ---
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;

    // --- Higher-level drawing (overrides AFDisplayInterface fallbacks) ---
    void fillScreen(uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) override;
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) override;
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) override;
    void drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;
    void fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) override;

    // --- Text ---
    void setCursor(int16_t x, int16_t y) override;
    void setTextColor(uint16_t color) override;
    void setTextSize(uint8_t size) override;
    void print(const char* text) override;
    void getTextBounds(const char* str, int16_t x, int16_t y,
                       int16_t* x1, int16_t* y1,
                       uint16_t* w, uint16_t* h) override;

    // --- Bitmap drawing ---
    void drawBitmap(int16_t x, int16_t y,
                    const uint8_t* bitmap,
                    int16_t w, int16_t h,
                    uint16_t color) override;
    void drawBitmap(int16_t x, int16_t y,
                    const uint8_t* bitmap,
                    int16_t w, int16_t h,
                    uint16_t color, uint16_t bg) override;

    // --- RGB bitmap / canvas support ---
    void drawRGBBitmap(int16_t x, int16_t y,
                       const uint16_t* bitmap,
                       int16_t w, int16_t h) override;

    // --- Canvas support ---
    bool supportsCanvas() const override { return false; }
    AFDisplayInterface* createCanvas() override;
    const uint16_t* getCanvasBuffer() const override;

    // Access the underlying LovyanGFX for backend-specific operations
    lgfx::LGFX_Device* getLGFX() { return m_gfx; }

private:
    lgfx::LGFX_Device* m_gfx = nullptr;
};

// Canvas wrapper: owns a buffer and LovyanGFX sprite
class AFCanvasLovyanGFX : public AFDisplayLovyanGFX {
public:
    AFCanvasLovyanGFX(int16_t w, int16_t h)
        : AFDisplayLovyanGFX(), m_width(w), m_height(h) {
        // Create a simple buffer for now - sprite implementation can be added later
        m_buffer = new uint16_t[w * h];
        // Clear to black (RGB565 black = 0x0000)
        for (int i = 0; i < w * h; i++) {
            m_buffer[i] = 0x0000;
        }
    }

    ~AFCanvasLovyanGFX() {
        delete[] m_buffer;
    }

    // Override width/height to return canvas dimensions
    int16_t width() const override { return m_width; }
    int16_t height() const override { return m_height; }

    const uint16_t* getCanvasBuffer() const override {
        return m_buffer;
    }

    // Override drawPixel to write to our buffer
    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            m_buffer[y * m_width + x] = color;
        }
    }

    // Override fillScreen to clear our buffer
    void fillScreen(uint16_t color) override {
        for (int i = 0; i < m_width * m_height; i++) {
            m_buffer[i] = color;
        }
    }

private:
    uint16_t* m_buffer;
    int16_t m_width;
    int16_t m_height;
};
