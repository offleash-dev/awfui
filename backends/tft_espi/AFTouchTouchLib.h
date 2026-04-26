#pragma once

//// AFTouchTouchLib.h
//// Part of the AWFUI library
////
//// Touch interface implementation for TouchLib (I2C-based capacitive touch controllers).
//// Supports CST816, CST328, and other I2C touch ICs via the TouchLib library.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFTouchInterface.h"
#include <TouchLib.h>



class AFTouchTouchLib : public AFTouchInterface {
    public:
      AFTouchTouchLib(TouchLib* touch) : m_touch(touch), m_rotation(0), m_width(240), m_height(320) {
      }


      bool begin() override {
            // TouchLib is typically initialized externally with Wire.begin() and touch.init()
            // This is a no-op - initialization happens before AWFUI starts
            return true;
      }


      // Set rotation to match display rotation (0-3) and sets the panel dimensions in native orientation (before
      // rotation)
      void setRotation(uint8_t rotation, int16_t width = 240, int16_t height = 320) {
            m_rotation = rotation % 4;
            m_width    = width;
            m_height   = height;
      }


      AFTouchPoint getPoint() override {
            AFTouchPoint pt = {-1, -1, false};

            if (!m_touch) {
                  return pt;
            }

            if (m_touch->read()) {
                  uint8_t n = m_touch->getPointNum();
                  if (n > 0) {
                        TP_Point t = m_touch->getPoint(0); // Get first touch point

                        // Transform coordinates based on rotation
                        switch (m_rotation) {
                              case 0: // Portrait, no rotation
                                    pt.x = t.x;
                                    pt.y = t.y;
                                    break;
                              case 1: // Landscape, 90° CW
                                    pt.x = t.y;
                                    pt.y = m_width - 1 - t.x;
                                    break;
                              case 2: // Portrait, 180°
                                    pt.x = m_width - 1 - t.x;
                                    pt.y = m_height - 1 - t.y;
                                    break;
                              case 3: // Landscape, 270° CW
                                    pt.x = m_height - 1 - t.y;
                                    pt.y = t.x;
                                    break;
                        }
                        pt.touched = true;
                  }
            }

            return pt;
      }
    private:
      TouchLib* m_touch;
      uint8_t   m_rotation;
      int16_t   m_width;  // Native panel width
      int16_t   m_height; // Native panel height
};
