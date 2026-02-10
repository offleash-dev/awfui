#pragma once

//// AFFt6206Touch.h
//// Part of the AWFUI library
////
//// Touch interface implementation for Adafruit FT6206 capacitive touch controller.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFTouchInterface.h"
#include "Adafruit_FT6206.h"



class AFFt6206Touch : public AFTouchInterface {
public:
    AFFt6206Touch(uint8_t threshold = 40) : m_threshold(threshold) {
    }


    bool begin() override {
        return m_touch.begin(m_threshold);
    }


    // Set rotation to match display rotation (0-3) and sets the panel dimensions in native orientation (before rotation)
    void setRotation(uint8_t rotation, int16_t width = 240, int16_t height = 320) {
        m_rotation = rotation % 4;
        m_width = width;
        m_height = height;
    }
  

    AFTouchPoint getPoint() override {
        AFTouchPoint pt = {-1, -1, false};
        if (m_touch.touched()) {
            TS_Point p = m_touch.getPoint();
            
            // Transform coordinates based on rotation
            switch (m_rotation) {
                case 0:  // Portrait, no rotation
                    pt.x = p.x;
                    pt.y = p.y;
                    break;
                case 1:  // Landscape, 90° CW
                    pt.x = p.y;
                    pt.y = m_width - 1 - p.x;
                    break;
                case 2:  // Portrait, 180°
                    pt.x = m_width - 1 - p.x;
                    pt.y = m_height - 1 - p.y;
                    break;
                case 3:  // Landscape, 270° CW
                    pt.x = m_height - 1 - p.y;
                    pt.y = p.x;
                    break;
            }
            pt.touched = true;
        }
        return pt;
    }

    
private:
    Adafruit_FT6206 m_touch;
    uint8_t  m_threshold;
    uint8_t  m_rotation = 0;
    int16_t  m_width = 240;   // Native panel width
    int16_t  m_height = 320;  // Native panel height
};
