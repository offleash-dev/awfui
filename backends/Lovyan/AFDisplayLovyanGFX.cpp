//// AFDisplayLovyanGFX.cpp
//// Part of the AWFUI library
////
//// AFDisplayInterface backend that wraps LovyanGFX.
//// Implementation file for canvas creation.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFDisplayLovyanGFX.h"

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
