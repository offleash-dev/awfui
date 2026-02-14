//// gfx_sdl.cpp
//// Implementation of the SDL-based Adafruit GFX for desktop simulation.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "GFX_SDL.hpp"



// Convert RGB565 to ARGB8888 with full alpha
// 
static uint32_t rgb565_to_argb8888(uint16_t c) {
    uint8_t r = ((c >> 11) & 0x1F) << 3;
    uint8_t g = ((c >> 5)  & 0x3F) << 2;
    uint8_t b = (c & 0x1F) << 3;
    return 0xFF000000 | (r << 16) | (g << 8) | b;
}



// Constructor
//
GFX_SDL::GFX_SDL(int16_t w, int16_t h)
    : Adafruit_GFX(w, h), m_physWidth(w), m_physHeight(h)
{
    framebuffer = new uint32_t[w * h];
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("AFUI Simulator",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              w * 2, h * 2, 0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                w, h);
}



// Destructor
//
GFX_SDL::~GFX_SDL() {
    delete[] framebuffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



// Draw a pixel to the framebuffer
//
void GFX_SDL::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || y < 0 || x >= _width || y >= _height) return;
    framebuffer[y * _width + x] = rgb565_to_argb8888(color);
}



// Set rotation and resize SDL window to match
//
void GFX_SDL::setRotation(uint8_t r) {
    // Set rotation value but bypass Adafruit_GFX's internal coordinate transform.
    // GFX_SDL's framebuffer is in logical (rotated) space, so drawPixel expects
    // logical coords directly.  We just need _width/_height swapped correctly.
    rotation = r & 3;
    switch (rotation) {
        case 0:
        case 2:
            _width  = m_physWidth;
            _height = m_physHeight;
            break;
        case 1:
        case 3:
            _width  = m_physHeight;
            _height = m_physWidth;
            break;
    }
    
    // Recreate framebuffer at rotated dimensions
    delete[] framebuffer;
    framebuffer = new uint32_t[_width * _height];
    memset(framebuffer, 0, _width * _height * sizeof(uint32_t));

    // Resize window and recreate texture to match
    SDL_SetWindowSize(window, _width * 2, _height * 2);
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                _width, _height);
}



// Initialize / clear framebuffer
//
void GFX_SDL::begin() {
    memset(framebuffer, 0, _width * _height * sizeof(uint32_t));
}



// Present the framebuffer to the window
//
void GFX_SDL::present() {
    // Texture dimensions match the rotated WIDTH x HEIGHT
    // Framebuffer pitch is always physical width
    SDL_UpdateTexture(texture, nullptr, framebuffer, _width * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}



// Process SDL events
//  
bool GFX_SDL::processEvents(bool& quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
            return false;
        }
        // You can map mouse → AFUI touch here
    }
    return true;
}