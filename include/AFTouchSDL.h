#pragma once

//// AFTouchSDL.h
//// Part of the AWFUI library
////
//// Touch interface implementation for SDL (desktop simulation).
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <SDL.h>

#include "AFTouchInterface.h"



class AFTouchSDL : public AFTouchInterface {
public:
    AFTouchSDL(int displayScale = 2);  // e.g., window is 2x the logical resolution

    bool begin() override;
    AFTouchPoint getPoint() override;

    // Called by your SDL event loop
    void handleEvent(const SDL_Event& e);

    
private:
    int scale;
    bool isDown;
    int16_t lastX;
    int16_t lastY;
};
