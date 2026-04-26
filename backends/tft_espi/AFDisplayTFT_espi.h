#pragma once

//// AFDisplayTFT_espi.h
//// Part of the AWFUI library
////
//// AFDisplayInterface backend that wraps TFT_eSPI.
//// Use this when your display driver uses TFT_eSPI.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <TFT_eSPI.h>

#include "../../include/AFDisplayInterface.h"
#include "../../include/AFBase.h"



class AFDisplayTFT_espi : public AFDisplayInterface {
public:
    AFDisplayTFT_espi() = default;
    explicit AFDisplayTFT_espi(TFT_eSPI* tft) : m_tft(tft  ) {}
    virtual ~AFDisplayTFT_espi() = default;

    // Set the display reference
    void setDisplay(TFT_eSPI* tft) { m_tft = tft; }

    virtual bool supportsFastFill() const override { return false; }
    virtual bool supportsFastBitmap() const override { return false; }


    // Initialize the display
    void begin() override;

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
    virtual bool supportsCanvas() const override { return true; }

    AFCanvas* createCanvas(uint16_t w, uint16_t h) override;
    virtual void destroyCanvas(AFCanvas* canvas) override;

    virtual void setCurrentCanvas(AFCanvas* canvas) override;
    virtual void startCanvasUpdate() override;
    virtual void endCanvasUpdate(bool copyToScreen = true) override;

    // Return the raw pixel buffer of a canvas, or nullptr if N/A.
    virtual AFCanvasData getCurrentCanvasBuffer() override;

    // Access the underlying TFT_eSPI for backend-specific operations
    TFT_eSPI* getTFT() { return m_tft; }

private:
    TFT_eSPI* m_tft = nullptr;
    TFT_eSprite* m_currentSprite = nullptr;
};
