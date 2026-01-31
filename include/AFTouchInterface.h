#pragma once

// AFTouchInterface.h
// Abstract interface for touch input devices

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
