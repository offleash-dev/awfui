//// AFDisplayLovyanGFX.cpp
//// Part of the AWFUI library
////
//// AFDisplayInterface backend that wraps LovyanGFX.
//// Implementation file for canvas creation.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFDisplayLovyanGFX.h"
// may need to make this local for arduino, e.g. #include "../lib/awfui/backends/Lovyan/AFDisplayLovyanGFX.h"
#include <LovyanGFX.hpp>
#include <cstring>

// --- Dimensions & orientation ---
int16_t AFDisplayLovyanGFX::width() const {
    return m_gfx ? m_gfx->width() : 0;
}

int16_t AFDisplayLovyanGFX::height() const {
    return m_gfx ? m_gfx->height() : 0;
}

uint8_t AFDisplayLovyanGFX::getRotation() const {
    return m_gfx ? m_gfx->getRotation() : 0;
}

void AFDisplayLovyanGFX::setRotation(uint8_t r) {
    if (m_gfx) m_gfx->setRotation(r);
}

AFDisplayInterface* AFDisplayLovyanGFX::createCanvas() {
    // Create a canvas at half resolution for memory efficiency
    int16_t canvasWidth = width() / 2;
    int16_t canvasHeight = height() / 2;
    
    // Ensure minimum size
    if (canvasWidth < 1) canvasWidth = 1;
    if (canvasHeight < 1) canvasHeight = 1;
    
    return new AFCanvasLovyanGFX(canvasWidth, canvasHeight);
}

const uint16_t* AFDisplayLovyanGFX::getCanvasBuffer() const {
    // This is only valid for canvas instances
    return nullptr;
}

// Method implementations
void AFDisplayLovyanGFX::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (m_gfx) m_gfx->drawPixel(x, y, color);
}

void AFDisplayLovyanGFX::drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    if (m_gfx) m_gfx->drawFastHLine(x, y, w, color);
}

void AFDisplayLovyanGFX::drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    if (m_gfx) m_gfx->drawFastVLine(x, y, h, color);
}

void AFDisplayLovyanGFX::fillScreen(uint16_t color) {
    if (m_gfx) m_gfx->fillScreen(color);
}

void AFDisplayLovyanGFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (!m_gfx) return;
    // Check if this is a full screen fill - optimize it!
    if (x == 0 && y == 0 && w == m_gfx->width() && h == m_gfx->height()) {
        fillScreen(color);
        return;
    }
    
    // Use default implementation for other rectangles
    m_gfx->fillRect(x, y, w, h, color);
}

void AFDisplayLovyanGFX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    if (m_gfx) m_gfx->drawRect(x, y, w, h, color);
}

void AFDisplayLovyanGFX::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {
    if (m_gfx) m_gfx->fillRoundRect(x, y, w, h, radius, color);
}

void AFDisplayLovyanGFX::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {
    if (m_gfx) m_gfx->drawRoundRect(x, y, w, h, radius, color);
}

void AFDisplayLovyanGFX::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    if (m_gfx) m_gfx->drawLine(x0, y0, x1, y1, color);
}

void AFDisplayLovyanGFX::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    if (m_gfx) m_gfx->drawCircle(x, y, r, color);
}

void AFDisplayLovyanGFX::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
    if (m_gfx) m_gfx->fillCircle(x, y, r, color);
}

// --- Text ---
void AFDisplayLovyanGFX::setCursor(int16_t x, int16_t y) {
    if (m_gfx) m_gfx->setCursor(x, y);
}

void AFDisplayLovyanGFX::setTextColor(uint16_t color) {
    if (m_gfx) m_gfx->setTextColor(color);
}

void AFDisplayLovyanGFX::setTextSize(uint8_t size) {
    if (m_gfx) m_gfx->setTextSize(size);
}

void AFDisplayLovyanGFX::print(const char* text) {
    if (m_gfx) m_gfx->print(text);
}

void AFDisplayLovyanGFX::getTextBounds(const char* str, int16_t x, int16_t y,
                           int16_t* x1, int16_t* y1,
                           uint16_t* w, uint16_t* h) {
    // LovyanGFX doesn't have getTextBounds, provide reasonable defaults
    if (x1) *x1 = x;
    if (y1) *y1 = y;
    if (w) *w = strlen(str) * 6;  // Approximate character width
    if (h) *h = 8;  // Approximate character height
}

// --- Bitmap drawing ---
void AFDisplayLovyanGFX::drawBitmap(int16_t x, int16_t y,
                        const uint8_t* bitmap,
                        int16_t w, int16_t h,
                        uint16_t color) {
    if (m_gfx) m_gfx->drawBitmap(x, y, bitmap, w, h, color);
}

void AFDisplayLovyanGFX::drawBitmap(int16_t x, int16_t y,
                        const uint8_t* bitmap,
                        int16_t w, int16_t h,
                        uint16_t color, uint16_t bg) {
    if (m_gfx) m_gfx->drawBitmap(x, y, bitmap, w, h, color, bg);
}

// --- RGB bitmap / canvas support ---
void AFDisplayLovyanGFX::drawRGBBitmap(int16_t x, int16_t y,
                           const uint16_t* bitmap,
                           int16_t w, int16_t h) {
    if (m_gfx) m_gfx->pushImage(x, y, w, h, bitmap);
}
