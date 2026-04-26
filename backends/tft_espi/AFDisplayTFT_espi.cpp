//// AFDisplayTFT_espi.cpp
//// Part of the AWFUI library
////
//// AFDisplayInterface backend that wraps TFT_eSPI.
//// Implementation file for canvas creation.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFDisplayTFT_espi.h"
// change as needed for you arduino project, e.g.
#include "../lib/awfui/backends/tft_espi/AFDisplayTFT_espi.h"
#include <TFT_eSPI.h>
#include <cstring>
//#include <Arduino.h>


void AFDisplayTFT_espi::begin() {
      if (m_tft) {
            m_tft->begin();
      }
}



// --- Dimensions & orientation ---
int16_t AFDisplayTFT_espi::width() const {
    return m_tft ? m_tft->width() : 0;
}



int16_t AFDisplayTFT_espi::height() const {
    return m_tft ? m_tft->height() : 0;
}



uint8_t AFDisplayTFT_espi::getRotation() const {
    return m_tft ? m_tft->getRotation() : 0;
}



void AFDisplayTFT_espi::setRotation(uint8_t r) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->setRotation(r);
    } else {
        m_tft->setRotation(r);
    }
}



// Method implementations
void AFDisplayTFT_espi::drawPixel(int16_t x, int16_t y, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->drawPixel(x, y, color);
    } else {
        m_tft->drawPixel(x, y, color);
    }
}



void AFDisplayTFT_espi::drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->drawFastHLine(x, y, w, color);
    } else {
        m_tft->drawFastHLine(x, y, w, color);
    }
}



void AFDisplayTFT_espi::drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->drawFastVLine(x, y, h, color);
    } else {
        m_tft->drawFastVLine(x, y, h, color);
    }
}



void AFDisplayTFT_espi::fillScreen(uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->fillScreen(color);
    } else {
        m_tft->fillScreen(color);
    }
}



void AFDisplayTFT_espi::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->fillRect(x, y, w, h, color);  // To canvas
    } else {

        // Check if this is a full screen fill - optimize it!
        if (x == 0 && y == 0 && w == m_tft->width() && h == m_tft->height()) {
            fillScreen(color);
            return;
        }
        
        // Use default implementation for other rectangles
        m_tft->fillRect(x, y, w, h, color);
    }
}



void AFDisplayTFT_espi::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
           m_currentSprite->drawRect(x, y, w, h, color);
    } else {
          m_tft->drawRect(x, y, w, h, color);
    }
}



void AFDisplayTFT_espi::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->fillRoundRect(x, y, w, h, radius, color);
    } else {
        m_tft->fillRoundRect(x, y, w, h, radius, color);
    }
}



void AFDisplayTFT_espi::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->drawRoundRect(x, y, w, h, radius, color);
    } else {
        m_tft->drawRoundRect(x, y, w, h, radius, color);
    }
}



void AFDisplayTFT_espi::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->drawLine(x0, y0, x1, y1, color);
    } else {
        m_tft->drawLine(x0, y0, x1, y1, color);
    }   
}



void AFDisplayTFT_espi::drawCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->drawCircle(x, y, r, color);
    } else {
        m_tft->drawCircle(x, y, r, color);
    }
}



void AFDisplayTFT_espi::fillCircle(int16_t x, int16_t y, int16_t r, uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->fillCircle(x, y, r, color);
    } else {
        m_tft->fillCircle(x, y, r, color);
    }
}



// --- Text ---
void AFDisplayTFT_espi::setCursor(int16_t x, int16_t y) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->setCursor(x, y);
    } else {
        m_tft->setCursor(x, y);
    }
}



void AFDisplayTFT_espi::setTextColor(uint16_t color) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->setTextColor(color);
    } else {
        m_tft->setTextColor(color);
    }
}



void AFDisplayTFT_espi::setTextSize(uint8_t size) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->setTextSize(size);
    } else {
        m_tft->setTextSize(size);
    }
}



void AFDisplayTFT_espi::print(const char* text) {
      if (!m_tft)
            return;

    if (m_currentSprite) {
        m_currentSprite->print(text);
    } else {
        m_tft->print(text);
    }   
}



void AFDisplayTFT_espi::getTextBounds(const char* str, int16_t x, int16_t y,
                           int16_t* x1, int16_t* y1,
                           uint16_t* w, uint16_t* h) {
    // TFT_eSPI has getTextBounds, use it
    if (m_tft) {
        m_tft->getTextBounds(str, x, y, x1, y1, w, h);
    } else {
        // Fallback defaults
        if (x1) *x1 = x;
        if (y1) *y1 = y;
        if (w) *w = strlen(str) * 6;
        if (h) *h = 8;
    }
}



// --- Bitmap drawing ---
void AFDisplayTFT_espi::drawBitmap(int16_t x, int16_t y,
                        const uint8_t* bitmap,
                        int16_t w, int16_t h,
                        uint16_t color) {
      if (!m_tft)
            return;
    if (m_currentSprite) {
        m_currentSprite->drawBitmap(x, y, bitmap, w, h, color);
    } else {
        m_tft->drawBitmap(x, y, bitmap, w, h, color);
    }   
}



void AFDisplayTFT_espi::drawBitmap(int16_t x, int16_t y,
                        const uint8_t* bitmap,
                        int16_t w, int16_t h,
                        uint16_t color, uint16_t bg) {
      if (!m_tft)
            return;
    if (m_currentSprite) {
        m_currentSprite->drawBitmap(x, y, bitmap, w, h, color, bg);
    } else {
        m_tft->drawBitmap(x, y, bitmap, w, h, color, bg);
    }
}



// --- RGB bitmap / canvas support ---
void AFDisplayTFT_espi::drawRGBBitmap(int16_t x, int16_t y,
                           const uint16_t* bitmap,
                           int16_t w, int16_t h) {
      if (!m_tft)
            return;
    if (m_currentSprite) {
        m_currentSprite->pushImage(x, y, w, h, bitmap);
    } else {
        m_tft->pushImage(x, y, w, h, bitmap);
    }
}



AFCanvas* AFDisplayTFT_espi::createCanvas(uint16_t w, uint16_t h) {
      if (!m_tft)
            return;

    AFCanvas* canvas = new AFCanvas();
    canvas->width = w;
    canvas->height = h;
    canvas->data = new TFT_eSprite(m_tft);
    static_cast<TFT_eSprite*>(canvas->data)->createSprite(w, h);

    return canvas;
};



void AFDisplayTFT_espi::destroyCanvas(AFCanvas* canvas) {
      if (canvas) {
           delete static_cast<TFT_eSprite*>(canvas->data);
           delete canvas;
      }
};



void AFDisplayTFT_espi::setCurrentCanvas(AFCanvas* canvas) {
        m_currentSprite = canvas ? static_cast<TFT_eSprite*>(canvas->data) : nullptr;
};



void AFDisplayTFT_espi::startCanvasUpdate() {
    AFDisplayBase::startCanvasUpdate();
};



void AFDisplayTFT_espi::endCanvasUpdate(bool copyToScreen) {
      if (m_currentSprite && copyToScreen && m_canvasNeedsUpdate) {
           m_currentSprite->pushSprite(0, 0);
      }
      m_canvasNeedsUpdate = false;
};



AFCanvasData AFDisplayTFT_espi::getCurrentCanvasBuffer() {
    return static_cast<AFCanvasData>(m_currentSprite);
};
