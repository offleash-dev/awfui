#pragma once

//// AFCheckbox.h
//// Part of the AWFUI library
////
//// Lightweight checkbox widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFTheme.h"
#include "AFWidget.h"

// Forward declaration
class AFCheckbox;

// Callback type for checkbox state changes (passes sender and checked state)
using AFCheckboxCallback = void (*)(AFCheckbox& sender, bool checked);



class AFCheckbox : public AFWidget {
public:
      AFCheckbox(int16_t x,
                 int16_t y,
                 int16_t boxSize = 0,
                 uint32_t id = 0,
                 const char* label = nullptr);

      void setLabel(const char* text);

      const char* getLabel() const {
            return m_label;
      }


      void setChecked(bool checked);

      bool isChecked() const {
            return m_checked;
      }


      void toggle() {
            setChecked(!m_checked);
      }


      void setTextSize(uint8_t size) {
            m_textSize = size;
            markDirty();
      }


      uint8_t getTextSize() const {
            return m_textSize;
      }


      void setColors(uint16_t boxColor, uint16_t checkColor, uint16_t borderColor, uint16_t labelColor);


      void setOnChangeCallback(AFCheckboxCallback cb) {
            m_onChangeCallback = cb;
      }


      void draw(AFDisplayInterface& displayInterface) override;

      // Event handling
      void onClick(const AFEvent& e) override;


private:
      const char*      m_label;
      bool             m_checked = false;

      uint16_t         m_boxColor;
      uint16_t         m_checkColor;
      uint16_t         m_borderColor;
      uint16_t         m_labelColor;

      uint8_t          m_textSize = 1;
      uint8_t          m_boxSize;   // width/height of the square box

      AFCheckboxCallback m_onChangeCallback = nullptr;
};