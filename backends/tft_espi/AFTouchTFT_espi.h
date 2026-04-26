#pragma once

//// AFTouchTFT_espi.h
//// Part of the AWFUI library
////
//// Touch input interface for TFT_eSPI displays.
//// Provides touch event handling for TFT_eSPI-based displays.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


//// WARNING -- this is based on loyvan version of AFTouch.
////            It is untested at this point because I don't
////            have a means of testing it
////            (aka, I don't have a board with resistive touch)
////            It is offered as starter or placeholder for
////            TFT_eSPI native touch handling.


// change this if needed for arduino projects, e.g.
// #include "../../include/AFTouchInterface.h"
#include "AFTouchInterface.h"
#include <TFT_eSPI.h>



class AFTouchTFT_espi : public AFTouchInterface {
    public:
      AFTouchTFT_espi() = default;
      explicit AFTouchTFT_espi(TFT_eSPI* tft) : m_tft(tft) {
      }


      // Set the display reference
      void setDisplay(TFT_eSPI* tft) {
            m_tft = tft;
      }


      // Initialize touch interface
      bool begin() override {
            // Touch is typically initialized with the display in TFT_eSPI
            // This is a no-op - initialization happens when TFT_eSPI is configured
            return true;
      }


      // Get current touch point
      AFTouchPoint getPoint() override {
            AFTouchPoint point = {-1, -1, false};
            uint16_t     x = 0, y = 0;

            if (m_tft) {
                  bool pressed = m_tft->getTouch(&x, &y);
                  if (pressed) {
                        point.x       = static_cast<int16_t>(x);
                        point.y       = static_cast<int16_t>(y);
                        point.touched = true;
                  }
            }

            return point;
      }


      // Access the underlying TFT_eSPI for backend-specific operations
      TFT_eSPI* getTFT() {
            return m_tft;
      }
    private:
      TFT_eSPI* m_tft = nullptr;
};
