#pragma once

//// gfx_sdl.hpp
//// Part of the AWFUI library
////
//// SDL-based Adafruit GFX implementation for desktop simulation.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <SDL.h>

#include <cstdint>

#include "AFAdafruitCompat.h"
#include "Adafruit_GFX.h"



class GFX_SDL : public Adafruit_GFX {
public:
    GFX_SDL(int16_t w, int16_t h);
    ~GFX_SDL();

    void drawPixel(int16_t x, int16_t y, uint16_t color) override;
    void setRotation(uint8_t r) override;

    void begin();
    void present();
    bool processEvents(bool& quit);


private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;

    int16_t m_physWidth, m_physHeight;  // Physical (unrotated) dimensions
    uint32_t* framebuffer; // 32-bit ARGB for SDL
};
