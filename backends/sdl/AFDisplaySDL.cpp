//// AFDisplaySDL.cpp
//// Pure-SDL AFDisplayInterface backend implementation.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFDisplaySDL.h"
#include <cstring>
#include <cstdlib>



// ============================================================
// Embedded 5x7 bitmap font (ASCII 0x20–0x7E, same as Adafruit default)
// Each character is 5 bytes wide; each byte is a column, LSB = top row.
// ============================================================
static const uint8_t font5x7[][5] = {
    {0x00,0x00,0x00,0x00,0x00}, // 0x20 ' '
    {0x00,0x00,0x5F,0x00,0x00}, // 0x21 '!'
    {0x00,0x07,0x00,0x07,0x00}, // 0x22 '"'
    {0x14,0x7F,0x14,0x7F,0x14}, // 0x23 '#'
    {0x24,0x2A,0x7F,0x2A,0x12}, // 0x24 '$'
    {0x23,0x13,0x08,0x64,0x62}, // 0x25 '%'
    {0x36,0x49,0x55,0x22,0x50}, // 0x26 '&'
    {0x00,0x05,0x03,0x00,0x00}, // 0x27 '''
    {0x00,0x1C,0x22,0x41,0x00}, // 0x28 '('
    {0x00,0x41,0x22,0x1C,0x00}, // 0x29 ')'
    {0x08,0x2A,0x1C,0x2A,0x08}, // 0x2A '*'
    {0x08,0x08,0x3E,0x08,0x08}, // 0x2B '+'
    {0x00,0x50,0x30,0x00,0x00}, // 0x2C ','
    {0x08,0x08,0x08,0x08,0x08}, // 0x2D '-'
    {0x00,0x60,0x60,0x00,0x00}, // 0x2E '.'
    {0x20,0x10,0x08,0x04,0x02}, // 0x2F '/'
    {0x3E,0x51,0x49,0x45,0x3E}, // 0x30 '0'
    {0x00,0x42,0x7F,0x40,0x00}, // 0x31 '1'
    {0x42,0x61,0x51,0x49,0x46}, // 0x32 '2'
    {0x21,0x41,0x45,0x4B,0x31}, // 0x33 '3'
    {0x18,0x14,0x12,0x7F,0x10}, // 0x34 '4'
    {0x27,0x45,0x45,0x45,0x39}, // 0x35 '5'
    {0x3C,0x4A,0x49,0x49,0x30}, // 0x36 '6'
    {0x01,0x71,0x09,0x05,0x03}, // 0x37 '7'
    {0x36,0x49,0x49,0x49,0x36}, // 0x38 '8'
    {0x06,0x49,0x49,0x29,0x1E}, // 0x39 '9'
    {0x00,0x36,0x36,0x00,0x00}, // 0x3A ':'
    {0x00,0x56,0x36,0x00,0x00}, // 0x3B ';'
    {0x00,0x08,0x14,0x22,0x41}, // 0x3C '<'
    {0x14,0x14,0x14,0x14,0x14}, // 0x3D '='
    {0x41,0x22,0x14,0x08,0x00}, // 0x3E '>'
    {0x02,0x01,0x51,0x09,0x06}, // 0x3F '?'
    {0x32,0x49,0x79,0x41,0x3E}, // 0x40 '@'
    {0x7E,0x11,0x11,0x11,0x7E}, // 0x41 'A'
    {0x7F,0x49,0x49,0x49,0x36}, // 0x42 'B'
    {0x3E,0x41,0x41,0x41,0x22}, // 0x43 'C'
    {0x7F,0x41,0x41,0x22,0x1C}, // 0x44 'D'
    {0x7F,0x49,0x49,0x49,0x41}, // 0x45 'E'
    {0x7F,0x09,0x09,0x01,0x01}, // 0x46 'F'
    {0x3E,0x41,0x41,0x51,0x32}, // 0x47 'G'
    {0x7F,0x08,0x08,0x08,0x7F}, // 0x48 'H'
    {0x00,0x41,0x7F,0x41,0x00}, // 0x49 'I'
    {0x20,0x40,0x41,0x3F,0x01}, // 0x4A 'J'
    {0x7F,0x08,0x14,0x22,0x41}, // 0x4B 'K'
    {0x7F,0x40,0x40,0x40,0x40}, // 0x4C 'L'
    {0x7F,0x02,0x04,0x02,0x7F}, // 0x4D 'M'
    {0x7F,0x04,0x08,0x10,0x7F}, // 0x4E 'N'
    {0x3E,0x41,0x41,0x41,0x3E}, // 0x4F 'O'
    {0x7F,0x09,0x09,0x09,0x06}, // 0x50 'P'
    {0x3E,0x41,0x51,0x21,0x5E}, // 0x51 'Q'
    {0x7F,0x09,0x19,0x29,0x46}, // 0x52 'R'
    {0x46,0x49,0x49,0x49,0x31}, // 0x53 'S'
    {0x01,0x01,0x7F,0x01,0x01}, // 0x54 'T'
    {0x3F,0x40,0x40,0x40,0x3F}, // 0x55 'U'
    {0x1F,0x20,0x40,0x20,0x1F}, // 0x56 'V'
    {0x7F,0x20,0x18,0x20,0x7F}, // 0x57 'W'
    {0x63,0x14,0x08,0x14,0x63}, // 0x58 'X'
    {0x03,0x04,0x78,0x04,0x03}, // 0x59 'Y'
    {0x61,0x51,0x49,0x45,0x43}, // 0x5A 'Z'
    {0x00,0x00,0x7F,0x41,0x41}, // 0x5B '['
    {0x02,0x04,0x08,0x10,0x20}, // 0x5C '\'
    {0x41,0x41,0x7F,0x00,0x00}, // 0x5D ']'
    {0x04,0x02,0x01,0x02,0x04}, // 0x5E '^'
    {0x40,0x40,0x40,0x40,0x40}, // 0x5F '_'
    {0x00,0x01,0x02,0x04,0x00}, // 0x60 '`'
    {0x20,0x54,0x54,0x54,0x78}, // 0x61 'a'
    {0x7F,0x48,0x44,0x44,0x38}, // 0x62 'b'
    {0x38,0x44,0x44,0x44,0x20}, // 0x63 'c'
    {0x38,0x44,0x44,0x48,0x7F}, // 0x64 'd'
    {0x38,0x54,0x54,0x54,0x18}, // 0x65 'e'
    {0x08,0x7E,0x09,0x01,0x02}, // 0x66 'f'
    {0x08,0x14,0x54,0x54,0x3C}, // 0x67 'g'
    {0x7F,0x08,0x04,0x04,0x78}, // 0x68 'h'
    {0x00,0x44,0x7D,0x40,0x00}, // 0x69 'i'
    {0x20,0x40,0x44,0x3D,0x00}, // 0x6A 'j'
    {0x00,0x7F,0x10,0x28,0x44}, // 0x6B 'k'
    {0x00,0x41,0x7F,0x40,0x00}, // 0x6C 'l'
    {0x7C,0x04,0x18,0x04,0x78}, // 0x6D 'm'
    {0x7C,0x08,0x04,0x04,0x78}, // 0x6E 'n'
    {0x38,0x44,0x44,0x44,0x38}, // 0x6F 'o'
    {0x7C,0x14,0x14,0x14,0x08}, // 0x70 'p'
    {0x08,0x14,0x14,0x18,0x7C}, // 0x71 'q'
    {0x7C,0x08,0x04,0x04,0x08}, // 0x72 'r'
    {0x48,0x54,0x54,0x54,0x20}, // 0x73 's'
    {0x04,0x3F,0x44,0x40,0x20}, // 0x74 't'
    {0x3C,0x40,0x40,0x20,0x7C}, // 0x75 'u'
    {0x1C,0x20,0x40,0x20,0x1C}, // 0x76 'v'
    {0x3C,0x40,0x30,0x40,0x3C}, // 0x77 'w'
    {0x44,0x28,0x10,0x28,0x44}, // 0x78 'x'
    {0x0C,0x50,0x50,0x50,0x3C}, // 0x79 'y'
    {0x44,0x64,0x54,0x4C,0x44}, // 0x7A 'z'
    {0x00,0x08,0x36,0x41,0x00}, // 0x7B '{'
    {0x00,0x00,0x7F,0x00,0x00}, // 0x7C '|'
    {0x00,0x41,0x36,0x08,0x00}, // 0x7D '}'
    {0x08,0x08,0x2A,0x1C,0x08}, // 0x7E '~'
};

static const int kFontW     = 5;   // glyph width in pixels
static const int kFontH     = 7;   // glyph height in pixels
static const int kCharGap   = 1;   // 1-pixel gap between characters
static const int kCharCellW = kFontW + kCharGap;  // 6 px per character at size 1



// ============================================================
// Construction / destruction
// ============================================================

AFDisplaySDL::AFDisplaySDL(int16_t w, int16_t h, int displayScale)
    : m_physWidth(w), m_physHeight(h),
      m_width(w), m_height(h),
      m_scale(displayScale)
{
    m_framebuffer = new uint32_t[w * h];
    std::memset(m_framebuffer, 0, w * h * sizeof(uint32_t));

    SDL_Init(SDL_INIT_VIDEO);

    m_window = SDL_CreateWindow("AWFUI Simulator",
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                w * displayScale, h * displayScale, 0);

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    m_texture  = SDL_CreateTexture(m_renderer,
                                   SDL_PIXELFORMAT_ARGB8888,
                                   SDL_TEXTUREACCESS_STREAMING,
                                   w, h);
}


AFDisplaySDL::~AFDisplaySDL() {
    delete[] m_framebuffer;
    if (m_texture)  SDL_DestroyTexture(m_texture);
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
    SDL_Quit();
}



// ============================================================
// Helpers
// ============================================================

uint32_t AFDisplaySDL::rgb565_to_argb8888(uint16_t c) {
    uint8_t r = ((c >> 11) & 0x1F) << 3;
    uint8_t g = ((c >> 5)  & 0x3F) << 2;
    uint8_t b = (c & 0x1F) << 3;
    return 0xFF000000u | (uint32_t(r) << 16) | (uint32_t(g) << 8) | b;
}


void AFDisplaySDL::setPixel32(int16_t x, int16_t y, uint32_t argb) {
    if (x < 0 || y < 0 || x >= m_width || y >= m_height) return;
    m_framebuffer[y * m_width + x] = argb;
}


void AFDisplaySDL::rebuildFramebuffer() {
    delete[] m_framebuffer;
    m_framebuffer = new uint32_t[m_width * m_height];
    std::memset(m_framebuffer, 0, m_width * m_height * sizeof(uint32_t));

    SDL_SetWindowSize(m_window, m_width * m_scale, m_height * m_scale);
    SDL_DestroyTexture(m_texture);
    m_texture = SDL_CreateTexture(m_renderer,
                                  SDL_PIXELFORMAT_ARGB8888,
                                  SDL_TEXTUREACCESS_STREAMING,
                                  m_width, m_height);
}



// ============================================================
// Dimensions & orientation
// ============================================================

void AFDisplaySDL::setRotation(uint8_t r) {
    m_rotation = r & 3;
    switch (m_rotation) {
        case 0: case 2:
            m_width  = m_physWidth;
            m_height = m_physHeight;
            break;
        case 1: case 3:
            m_width  = m_physHeight;
            m_height = m_physWidth;
            break;
    }
    rebuildFramebuffer();
}



// ============================================================
// Core drawing primitives
// ============================================================

void AFDisplaySDL::drawPixel(int16_t x, int16_t y, uint16_t color) {
    setPixel32(x, y, rgb565_to_argb8888(color));
}


void AFDisplaySDL::drawHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    uint32_t c32 = rgb565_to_argb8888(color);
    for (int16_t i = 0; i < w; ++i) {
        setPixel32(x + i, y, c32);
    }
}


void AFDisplaySDL::drawVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    uint32_t c32 = rgb565_to_argb8888(color);
    for (int16_t i = 0; i < h; ++i) {
        setPixel32(x, y + i, c32);
    }
}


void AFDisplaySDL::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    uint32_t c32 = rgb565_to_argb8888(color);
    for (int16_t row = y; row < y + h; ++row) {
        for (int16_t col = x; col < x + w; ++col) {
            setPixel32(col, row, c32);
        }
    }
}



// ============================================================
// Text
// ============================================================

void AFDisplaySDL::setCursor(int16_t x, int16_t y) {
    m_cursorX = x;
    m_cursorY = y;
}


void AFDisplaySDL::setTextColor(uint16_t color) {
    m_textColor565 = color;
    m_textColor32  = rgb565_to_argb8888(color);
}


void AFDisplaySDL::setTextSize(uint8_t size) {
    m_textSize = (size < 1) ? 1 : size;
}


void AFDisplaySDL::drawChar(int16_t x, int16_t y, unsigned char c,
                            uint32_t color32, uint8_t size)
{
    if (c < 0x20 || c > 0x7E) c = '?';
    const uint8_t* glyph = font5x7[c - 0x20];

    for (int8_t col = 0; col < kFontW; ++col) {
        uint8_t colBits = glyph[col];
        for (int8_t row = 0; row < kFontH; ++row) {
            if (colBits & (1 << row)) {
                if (size == 1) {
                    setPixel32(x + col, y + row, color32);
                } else {
                    // Scaled: draw a size×size block
                    for (uint8_t sy = 0; sy < size; ++sy)
                        for (uint8_t sx = 0; sx < size; ++sx)
                            setPixel32(x + col * size + sx,
                                       y + row * size + sy, color32);
                }
            }
        }
    }
}


void AFDisplaySDL::print(const char* text) {
    while (*text) {
        drawChar(m_cursorX, m_cursorY, (unsigned char)*text,
                 m_textColor32, m_textSize);
        m_cursorX += kCharCellW * m_textSize;
        ++text;
    }
}


void AFDisplaySDL::getTextBounds(const char* str, int16_t x, int16_t y,
                                 int16_t* x1, int16_t* y1,
                                 uint16_t* w, uint16_t* h)
{
    uint16_t len = (uint16_t)std::strlen(str);
    *x1 = x;
    *y1 = y;
    *w  = len * kCharCellW * m_textSize;
    *h  = kFontH * m_textSize;
}



// ============================================================
// Bitmap drawing
// ============================================================

void AFDisplaySDL::drawBitmap(int16_t x, int16_t y,
                              const uint8_t* bitmap,
                              int16_t w, int16_t h,
                              uint16_t color)
{
    uint32_t c32 = rgb565_to_argb8888(color);
    int16_t bytesPerRow = (w + 7) / 8;

    for (int16_t row = 0; row < h; ++row) {
        for (int16_t col = 0; col < w; ++col) {
            uint8_t byte = bitmap[row * bytesPerRow + (col / 8)];
            if (byte & (0x80 >> (col & 7))) {
                setPixel32(x + col, y + row, c32);
            }
        }
    }
}


void AFDisplaySDL::drawBitmap(int16_t x, int16_t y,
                              const uint8_t* bitmap,
                              int16_t w, int16_t h,
                              uint16_t color, uint16_t bg)
{
    uint32_t c32  = rgb565_to_argb8888(color);
    uint32_t bg32 = rgb565_to_argb8888(bg);
    int16_t bytesPerRow = (w + 7) / 8;

    for (int16_t row = 0; row < h; ++row) {
        for (int16_t col = 0; col < w; ++col) {
            uint8_t byte = bitmap[row * bytesPerRow + (col / 8)];
            if (byte & (0x80 >> (col & 7))) {
                setPixel32(x + col, y + row, c32);
            } else {
                setPixel32(x + col, y + row, bg32);
            }
        }
    }
}


void AFDisplaySDL::drawRGBBitmap(int16_t x, int16_t y,
                                 const uint16_t* bitmap,
                                 int16_t w, int16_t h)
{
    for (int16_t row = 0; row < h; ++row) {
        for (int16_t col = 0; col < w; ++col) {
            setPixel32(x + col, y + row,
                       rgb565_to_argb8888(bitmap[row * w + col]));
        }
    }
}



// ============================================================
// SDL lifecycle
// ============================================================

void AFDisplaySDL::begin() {
    std::memset(m_framebuffer, 0, m_width * m_height * sizeof(uint32_t));
}


void AFDisplaySDL::present() {
    SDL_UpdateTexture(m_texture, nullptr, m_framebuffer,
                      m_width * sizeof(uint32_t));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);
    SDL_RenderPresent(m_renderer);
}