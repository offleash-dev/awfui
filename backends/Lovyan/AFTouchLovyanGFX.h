#pragma once

//// AFTouchLovyanGFX.h
//// Part of the AWFUI library
////
//// Touch input interface for LovyanGFX displays.
//// Provides touch event handling for LovyanGFX-based displays.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFTouchInterface.h"
#include <LovyanGFX.hpp>

class AFTouchLovyanGFX : public AFTouchInterface {
public:
    AFTouchLovyanGFX() = default;
    explicit AFTouchLovyanGFX(lgfx::LGFX_Device* gfx) : m_gfx(gfx), m_lastTouchX(0), m_lastTouchY(0), m_touchPressed(false) {}

    // Set the display reference
    void setDisplay(lgfx::LGFX_Device* gfx) { m_gfx = gfx; }

    // Initialize touch interface
    bool begin() override {
        // Touch is typically initialized with the display in LovyanGFX
        // This is a no-op - initialization happens in the LGFX driver
        return true;
    }

    // Get current touch point
    AFTouchPoint getPoint() override {
        AFTouchPoint point;
        uint16_t x, y;
        bool pressed = false;
        
        if (m_gfx) {
            pressed = m_gfx->getTouch(&x, &y);
        }
        
        point.x = x;
        point.y = y;
        point.touched = pressed;
        
        return point;
    }

    // Access the underlying LovyanGFX for backend-specific operations
    lgfx::LGFX_Device* getLGFX() { return m_gfx; }

private:
    lgfx::LGFX_Device* m_gfx = nullptr;
    uint16_t m_lastTouchX;
    uint16_t m_lastTouchY;
    bool m_touchPressed;
};
