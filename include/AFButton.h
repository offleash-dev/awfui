#pragma once

//// AFButton.h
//// Part of the AWFUI library
////
//// Lightweight button widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFTheme.h"
#include "AFWidget.h"



// Callback type for button clicks (no STL, just a function pointer)
using AFButtonCallback = void (*)();



class AFButton : public AFWidget {
public:
      AFButton(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0, const char* label = nullptr);

      void setLabel(const char* text);

      const char* getLabel() const {
            return m_label;
      }


      void setColors(uint16_t bg, uint16_t fg, uint16_t border);
      void setPressedColors(uint16_t bg, uint16_t fg, uint16_t border);

      void setTextSize(uint8_t size) {
            m_textSize = size;
      }


      uint8_t getTextSize() const {
            return m_textSize;
      }


      bool isPressed() const {
            return m_pressed;
      }

      // Drawing
      void draw(AFDisplayInterface& gfx) override;

      // Event handling
      void onPress(const AFEvent& e) override;
      void onRelease(const AFEvent& e) override;
      void onClick(const AFEvent& e) override;


      // Set callback function for click event 
      //
      void setOnClickCallback(AFButtonCallback cb) {
            m_onClickCallback = cb;
      }

      
private:
      const char* m_label;

      uint16_t m_bgColor;
      uint16_t m_fgColor;
      uint16_t m_borderColor;

      uint16_t m_bgColorPressed;
      uint16_t m_fgColorPressed;
      uint16_t m_borderColorPressed;

      uint8_t m_textSize = 1;
      bool    m_pressed  = false;

      AFButtonCallback m_onClickCallback = nullptr;
      AFButtonCallback m_onPressCallback = nullptr;
      AFButtonCallback m_onReleaseCallback = nullptr;
};
