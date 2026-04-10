#pragma once

//// AFDisplaySDL.h
//// Part of the AWFUI library
////
//// Pure-SDL AFDisplayInterface backend — no Adafruit_GFX dependency.
//// Renders to an ARGB8888 framebuffer and presents via SDL_Texture.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <SDL.h>
#include <cstdint>

#include "AFDisplayInterface.h"



class AFDisplaySDL : public AFDisplayInterface {
public:
    AFDisplaySDL(int16_t w, int16_t h, int displayScale = 2);
    ~AFDisplaySDL();

    virtual bool supportsCanvas() const override { return false; }
    virtual bool supportsFastFill() const override { return false; }
    virtual bool supportsFastBitmap() const override { return false; }

    virtual void begin() override {}

    // --- Dimensions & orientation ---
    int16_t width()  const override { return m_width; }
    int16_t height() const override { return m_height; }
    uint8_t getRotation() const override { return m_rotation; }
    void    setRotation(uint8_t r) override;

    // --- Core drawing primitives (AFDisplayBase) ---
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
    void drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;

    // --- Text (AFDisplayBase) ---
    void setCursor(int16_t x, int16_t y) override;
    void setTextColor(uint16_t color) override;
    void setTextSize(uint8_t size) override;
    void print(const char* text) override;
    void getTextBounds(const char* str, int16_t x, int16_t y,
                       int16_t* x1, int16_t* y1,
                       uint16_t* w, uint16_t* h) override;

    // --- Bitmap drawing (AFDisplayBase) ---
    void drawBitmap(int16_t x, int16_t y,
                    const uint8_t* bitmap,
                    int16_t w, int16_t h,
                    uint16_t color) override;

    void drawBitmap(int16_t x, int16_t y,
                    const uint8_t* bitmap,
                    int16_t w, int16_t h,
                    uint16_t color, uint16_t bg) override;

    void drawRGBBitmap(int16_t x, int16_t y,
                       const uint16_t* bitmap,
                       int16_t w, int16_t h) override;

    void initializeLineBuffer(int width) { unused(width); };

    // --- SDL lifecycle ---
    void begin();
    void present();


private:
    void setPixel32(int16_t x, int16_t y, uint32_t argb);
    void rebuildFramebuffer();
    void drawChar(int16_t x, int16_t y, unsigned char c, uint32_t color32, uint8_t size);

    static uint32_t rgb565_to_argb8888(uint16_t c);

    int16_t  m_physWidth;
    int16_t  m_physHeight;
    int16_t  m_width;
    int16_t  m_height;
    uint8_t  m_rotation = 0;
    int      m_scale;

    // Text state
    int16_t  m_cursorX = 0;
    int16_t  m_cursorY = 0;
    uint32_t m_textColor32 = 0xFFFFFFFF;
    uint16_t m_textColor565 = 0xFFFF;
    uint8_t  m_textSize = 1;

    // SDL resources
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    SDL_Texture*  m_texture  = nullptr;
    uint32_t*     m_framebuffer = nullptr;
};