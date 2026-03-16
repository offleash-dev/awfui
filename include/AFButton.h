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
using AFButtonCallback = void (*)(AFWidget& sender);



class AFButton : public AFWidget {
public:
      AFButton() = default;  // Default constructor for stack objects
      AFButton(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0, const char* label = nullptr);

      // Initialize method for stack objects
      void init(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0, const char* label = nullptr);

      void setLabel(const char* text);

      const char* getLabel() const {
            return m_label;
      }


      void setColors(uint16_t bg, uint16_t fg, uint16_t border);
      void setPressedColors(uint16_t bg, uint16_t fg, uint16_t border);

      void setTextSize(uint8_t size) {
            m_textSize = size;
            markDirty();
      }


      uint8_t getTextSize() const {
            return m_textSize;
      }


      bool isPressed() const {
            return m_pressed;
      }


      // for a label style button, hide the border
      void setShowBorder(bool show) {
            m_showBorder = show;
            markDirty();
      }

      
      bool getShowBorder() const {
            return m_showBorder;
      }


      // Drawing
      void draw(AFDisplayInterface& displayInterface) override;

      // Event handling
      void onPress(const AFEvent& e) override;
      void onRelease(const AFEvent& e) override;
      void onClick(const AFEvent& e) override;


      // Set callback function for click event 
      //
      void setOnClickCallback(AFButtonCallback cb) {
            m_onClickCallback = cb;
      }

      
protected:
      bool    m_pressed  = false;
      bool    m_showBorder = true;

      
private:
      const char* m_label;

      uint16_t m_bgColor;
      uint16_t m_fgColor;
      uint16_t m_borderColor;

      uint16_t m_bgColorPressed;
      uint16_t m_fgColorPressed;
      uint16_t m_borderColorPressed;

      uint8_t m_textSize = 1;

      AFButtonCallback m_onClickCallback = nullptr;
};