//// AFTouchSDL.cpp
//// Implementation of the AWFUI SDL touch interface.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFTouchSDL.h"



// Constructor
//
AFTouchSDL::AFTouchSDL(int displayScale)
    : scale(displayScale), isDown(false), lastX(-1), lastY(-1)
{}



// Initialize touch interface
//
bool AFTouchSDL::begin() {
    // Nothing to initialize for SDL touch
    return true;
}



// Convert an SDL event to AF touch point
//
void AFTouchSDL::handleEvent(const SDL_Event& e) {
    switch (e.type) {
        case SDL_MOUSEBUTTONDOWN:
            isDown = true;
            lastX = e.button.x / scale;
            lastY = e.button.y / scale;
            break;

        case SDL_MOUSEBUTTONUP:
            isDown = false;
            break;

        case SDL_MOUSEMOTION:
            if (isDown) {
                lastX = e.motion.x / scale;
                lastY = e.motion.y / scale;
            }
            break;
    }
}



// Get the current touch point
//
AFTouchPoint AFTouchSDL::getPoint() {
    return AFTouchPoint{ lastX, lastY, isDown };
}
