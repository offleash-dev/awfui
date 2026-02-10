//// AFDisplayAdafruitGFX.cpp
//// Implementation of the Adafruit_GFX display backend for AWFUI.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFDisplayAdafruitGFX.h"



// Create an off-screen canvas matching the display dimensions
//
AFDisplayInterface* AFDisplayAdafruitGFX::createCanvas() {
    return new AFCanvasAdafruitGFX(width(), height());
}



// Base wrapper has no canvas buffer
//
const uint16_t* AFDisplayAdafruitGFX::getCanvasBuffer() const {
    return nullptr;
}
