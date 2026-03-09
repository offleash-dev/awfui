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
#include <cstring>

// Version-based compatibility control
// Define version thresholds based on research of Adafruit_GFX releases
// Should be adjusted based on actual version history
#define ADAFRUIT_GFX_VERSION_HAS_SIMPLE_PRINT  10010  // Assume versions >= 1.0.10 have print(const char*)
#define ADAFRUIT_GFX_VERSION_HAS_GETTEXTBOUNDS 10020  // Assume versions >= 1.0.20 have getTextBounds
#define ADAFRUIT_GFX_VERSION_HAS_DRAWRGBBITMAP 10030  // Assume versions >= 1.0.30 have drawRGBBitmap
#define ADAFRUIT_GFX_VERSION_HAS_GFXCANVAS16 10040   // Assume versions >= 1.0.40 have GFXcanvas16

// Helper macros for version comparison
#if ADAFRUIT_GFX_VERSION >= ADAFRUIT_GFX_VERSION_HAS_SIMPLE_PRINT
#define HAS_SIMPLE_PRINT 1
#else
#define HAS_SIMPLE_PRINT 0
#endif

#if ADAFRUIT_GFX_VERSION >= ADAFRUIT_GFX_VERSION_HAS_GETTEXTBOUNDS
#define HAS_GETTEXTBOUNDS 1
#else
#define HAS_GETTEXTBOUNDS 0
#endif

#if ADAFRUIT_GFX_VERSION >= ADAFRUIT_GFX_VERSION_HAS_DRAWRGBBITMAP
#define HAS_DRAWRGBBITMAP 1
#else
#define HAS_DRAWRGBBITMAP 0
#endif

#if ADAFRUIT_GFX_VERSION >= ADAFRUIT_GFX_VERSION_HAS_GFXCANVAS16
#define HAS_GFXCANVAS16 1
#else
#define HAS_GFXCANVAS16 0
#endif



class AFDisplayAdafruitGFX : public AFDisplayInterface {
public:
    explicit AFDisplayAdafruitGFX(Adafruit_GFX& gfx) : m_gfx(gfx) {}

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
#if 1
        m_gfx.fillScreen(color);
#else
        fillScreenOptimized(color);
#endif
    }


    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override {
        // Check if this is a full screen fill - optimize it!
        if (x == 0 && y == 0 && w == m_gfx.width() && h == m_gfx.height()) {
            fillScreenOptimized(color);
            return;
        }
        
        // Use default implementation for other rectangles
        m_gfx.fillRect(x, y, w, h, color);
    }


    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override {
        m_gfx.drawRect(x, y, w, h, color);
    }

    // Optimized fillScreen for full screen - internal use
    void fillScreenOptimized(uint16_t color);

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
#if HAS_SIMPLE_PRINT
        // Newer Adafruit_GFX has simple print(const char*)
        m_gfx.print(text);
#else
        // Older Adafruit_GFX has printf-style print, print each character
        if (text) {
            while (*text) {
                char buffer[2];
                buffer[0] = *text;
                buffer[1] = '\0';
                m_gfx.print(buffer, 1, "%c");
                text++;
            }
        }
#endif
    }


    void getTextBounds(const char* str, int16_t x, int16_t y,
                       int16_t* x1, int16_t* y1,
                       uint16_t* w, uint16_t* h) override {
#if HAS_GETTEXTBOUNDS
        // Newer Adafruit_GFX has getTextBounds
        m_gfx.getTextBounds(str, x, y, x1, y1, w, h);
#else
        // Method not available in older Adafruit_GFX version
        // Provide reasonable defaults
        if (x1) *x1 = x;
        if (y1) *y1 = y;
        if (w) *w = 6;  // Approximate character width
        if (h) *h = 8;  // Approximate character height
#endif
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
#if HAS_DRAWRGBBITMAP
        // Newer Adafruit_GFX has drawRGBBitmap
        m_gfx.drawRGBBitmap(x, y, const_cast<uint16_t*>(bitmap), w, h);
#else
        // Method not available in older Adafruit_GFX version
        // Convert RGB565 to 8-bit grayscale and draw as regular bitmap
        for (int16_t row = 0; row < h; row++) {
            for (int16_t col = 0; col < w; col++) {
                uint16_t pixel = bitmap[row * w + col];
                // Convert RGB565 to 8-bit grayscale (simple approximation)
                uint8_t gray = ((pixel >> 11) & 0x1F) * 8/32 + 
                               ((pixel >> 5) & 0x3F) * 4/64 + 
                               (pixel & 0x1F) * 8/32;
                m_gfx.drawPixel(x + col, y + row, gray);
            }
        }
#endif
    }


    // --- Canvas support ---
    AFDisplayInterface* createCanvas() override;
    const uint16_t* getCanvasBuffer() const override;


    // Access the underlying Adafruit_GFX for backend-specific operations
    Adafruit_GFX& getGFX() { return m_gfx; }


private:
    Adafruit_GFX& m_gfx;
};



// Canvas wrapper: owns a buffer and optionally GFXcanvas16
class AFCanvasAdafruitGFX : public AFDisplayAdafruitGFX {
public:
    AFCanvasAdafruitGFX(int16_t w, int16_t h)
#if HAS_GFXCANVAS16
        : AFDisplayAdafruitGFX(m_canvas), m_canvas(w, h) {}
#else
        : AFDisplayAdafruitGFX(m_dummy_gfx), m_width(w/2), m_height(h/2) {
        // Use static buffer at half resolution
        m_buffer = s_canvasBuffer;
        // Clear buffer to black
        memset(m_buffer, 0, (w/2) * (h/2) * sizeof(uint16_t));
    }
#endif

#if !HAS_GFXCANVAS16
    // Override width/height to return correct values instead of dummy's
    int16_t width() const override { return m_width; }
    int16_t height() const override { return m_height; }
#endif

#if !HAS_GFXCANVAS16
    ~AFCanvasAdafruitGFX() {
        // Don't delete static buffer - it's allocated at compile time
    }
#endif

    const uint16_t* getCanvasBuffer() const override {
#if HAS_GFXCANVAS16
        return m_canvas.getBuffer();
#else
        return m_buffer;
#endif
    }

#if !HAS_GFXCANVAS16
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

    // Render scaled canvas to full display (2x scaling)
    void renderScaled() {
        Adafruit_GFX& gfx = getGFX();
        for (int y = 0; y < m_height; y++) {
            for (int x = 0; x < m_width; x++) {
                uint16_t color = m_buffer[y * m_width + x];
                // Draw 2x2 pixel block for each canvas pixel
                gfx.drawPixel(x*2, y*2, color);
                gfx.drawPixel(x*2+1, y*2, color);
                gfx.drawPixel(x*2, y*2+1, color);
                gfx.drawPixel(x*2+1, y*2+1, color);
            }
        }
    }
#endif


private:
#if !HAS_GFXCANVAS16
    // Instance-specific dummy GFX class for canvas constructor
    class DummyGFX : public Adafruit_GFX {
    public:
        DummyGFX() : Adafruit_GFX(1, 1) {}  // Default constructor
        DummyGFX(int16_t w, int16_t h) : Adafruit_GFX(w, h) {}
        virtual ~DummyGFX() = default;
        void drawPixel(int16_t x, int16_t y, uint16_t color) override {
            // Do nothing - this is just a dummy
        }
    };
    
    // Static buffer allocation for embedded systems with limited heap
    static uint16_t s_canvasBuffer[120 * 160]; // 37.5KB static buffer (smaller canvas)
    
    DummyGFX m_dummy_gfx;
    uint16_t* m_buffer;
    int16_t m_width;
    int16_t m_height;
#else
    GFXcanvas16 m_canvas;
#endif
};