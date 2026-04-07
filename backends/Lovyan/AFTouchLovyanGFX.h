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
    explicit AFTouchLovyanGFX(lgfx::LGFX& gfx) : m_gfx(gfx), m_lastTouchX(0), m_lastTouchY(0), m_touchPressed(false) {}

    // Initialize touch interface
    void begin() override {
        // Touch is typically initialized with the display in LovyanGFX
        // This is a no-op - initialization happens in the LGFX driver
    }

    // Check if touch is currently pressed
    bool isTouchPressed() override {
        uint16_t x, y;
        bool pressed = m_gfx.getTouch(&x, &y);
        if (pressed) {
            m_lastTouchX = x;
            m_lastTouchY = y;
            m_touchPressed = true;
        } else {
            m_touchPressed = false;
        }
        return pressed;
    }

    // Get current touch coordinates
    void getTouchPosition(int16_t& x, int16_t& y) override {
        if (m_touchPressed) {
            x = m_lastTouchX;
            y = m_lastTouchY;
        } else {
            // Return last known position if not currently pressed
            x = m_lastTouchX;
            y = m_lastTouchY;
        }
    }

    // Get touch coordinates in display space (with rotation applied)
    void getTouchPositionDisplay(int16_t& x, int16_t& y) override {
        // LovyanGFX getTouch() already returns coordinates in display space
        // considering the current rotation
        getTouchPosition(x, y);
    }

    // Check if touch coordinates are within bounds
    bool isTouchWithinBounds(int16_t x, int16_t y, int16_t width, int16_t height) override {
        int16_t touchX, touchY;
        getTouchPosition(touchX, touchY);
        
        return (touchX >= x && touchX < x + width &&
                touchY >= y && touchY < y + height);
    }

    // Wait for touch release (blocking)
    void waitForTouchRelease() override {
        while (isTouchPressed()) {
            delay(10);  // Small delay to prevent busy waiting
        }
    }

    // Wait for touch press (blocking)
    void waitForTouchPress() override {
        while (!isTouchPressed()) {
            delay(10);  // Small delay to prevent busy waiting
        }
    }

    // Calibrate touch interface (if needed)
    void calibrate() override {
        // LovyanGFX typically handles calibration automatically
        // or provides its own calibration methods
        // This is a placeholder for any custom calibration needed
    }

    // Access the underlying LovyanGFX for backend-specific operations
    lgfx::LGFX& getLGFX() { return m_gfx; }

private:
    lgfx::LGFX& m_gfx;
    uint16_t m_lastTouchX;
    uint16_t m_lastTouchY;
    bool m_touchPressed;
};
