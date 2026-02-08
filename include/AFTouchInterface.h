#pragma once

//// AFTouchInterface.h
//// Part of the AWFUI library
////
//// Abstract interface for touch input devices.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>



struct AFTouchPoint {
    int16_t x;
    int16_t y;
    bool    touched;
};



class AFTouchInterface {
public:
    virtual ~AFTouchInterface() = default;
    virtual bool begin() = 0;
    virtual AFTouchPoint getPoint() = 0;
};