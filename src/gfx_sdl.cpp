// gfx_sdl.cpp

#include "GFX_SDL.hpp"



// Convert RGB565 to ARGB8888
// with full alpha
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
    // Adafruit_GFX base class handles rotation - we just draw to the physical framebuffer
    if (x < 0 || y < 0 || x >= m_physWidth || y >= m_physHeight) return;
    framebuffer[y * m_physWidth + x] = rgb565_to_argb8888(color);
}



// Initialize / clear framebuffer
//
void GFX_SDL::begin() {
    memset(framebuffer, 0, m_physWidth * m_physHeight * sizeof(uint32_t));
}



// Present the framebuffer to the window
//
void GFX_SDL::present() {
    SDL_UpdateTexture(texture, nullptr, framebuffer, m_physWidth * sizeof(uint32_t));
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
