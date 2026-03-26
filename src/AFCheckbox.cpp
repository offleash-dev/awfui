//// AFCheckbox.cpp
//// Implementation of the AWFUI checkbox widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <string.h>



#include "AFCheckbox.h"
#include "AFWorld.h"



// Constructor
//
AFCheckbox::AFCheckbox(int16_t x,
                       int16_t y,
                       int16_t boxSize
                       const char* labelText,
                       uint32_t id,)
    : AFWidget(x, y, boxSize, boxSize, id),
      m_label(labelText),
      m_boxSize(static_cast<uint8_t>(boxSize)) {

      const AFTheme& theme = AFWorld::instance()->getTheme();
      m_boxColor    = theme.widgetBgColor;
      m_checkColor  = theme.widgetAccentColor;
      m_borderColor = theme.widgetBorderColor;
      m_labelColor  = theme.widgetFgColor;

      if (boxSize == 0) {
            m_boxSize = 16; // Default box size if not specified
      }
      
      m_textSize    = theme.widgetTextSize;
}



// Initialize method for stack objects
//
void AFCheckbox::init(int16_t x,
                      int16_t y,
                      int16_t boxSize,
                      const char* labelText,
                      uint32_t id) {
      // Initialize the base AFWidget
      m_x = x;
      m_y = y;
      m_width = boxSize;
      m_height = boxSize;
      m_id = id;
      m_label = labelText;
      m_boxSize = static_cast<uint8_t>(boxSize);

      if (boxSize == 0) {
            m_boxSize = 16; // Default box size if not specified
      }
      
      const AFTheme& theme = AFWorld::instance()->getTheme();
      m_boxColor    = theme.widgetBgColor;
      m_checkColor  = theme.widgetAccentColor;
      m_borderColor = theme.widgetBorderColor;
      m_labelColor  = theme.widgetFgColor;
      m_textSize    = theme.widgetTextSize;
}



// Set the label text
//
void AFCheckbox::setLabel(const char* text) {
      m_label = text;
      markDirty();
}



// Set checked state
//
void AFCheckbox::setChecked(bool checked) {
      if (m_checked == checked) {
            return;
      }

      m_checked = checked;
      markDirty();

      if (m_onChangeCallback) {
            m_onChangeCallback(*this, m_checked);
      }
}



void AFCheckbox::setColors(uint16_t boxColor, uint16_t checkColor, uint16_t borderColor, uint16_t labelColor) {
      m_boxColor = boxColor;
      m_checkColor = checkColor;
      m_borderColor = borderColor;
      m_labelColor = labelColor;

      markDirty();
}



// Draw the checkbox
//
void AFCheckbox::draw(AFDisplayInterface& displayInterface) {
      if (!isVisible()) {
            return;
      }

      const AFTheme& theme = AFWorld::instance()->getTheme();
      uint16_t boxColor    = m_boxColor;
      uint16_t checkColor  = m_checkColor;
      uint16_t borderColor = m_borderColor;
      uint16_t labelColor  = m_labelColor;
      uint8_t radius       = theme.widgetCornerRadius;

      // set colors to theme colors if disabled
      if (!isEnabled()) {
            boxColor    = theme.widgetDisabledBgColor;
            checkColor  = theme.widgetDisabledFgColor;
            borderColor = theme.widgetBorderColor;
            labelColor  = theme.widgetDisabledFgColor;
      }

      // Draw checkbox box
      displayInterface.fillRoundRect(m_x, m_y, m_boxSize, m_boxSize, radius, boxColor);
      displayInterface.drawRoundRect(m_x, m_y, m_boxSize, m_boxSize, radius, borderColor);

      // Draw check mark if checked
      if (m_checked) {
            // Simple "X" check mark
            int16_t x0 = m_x + 2;
            int16_t y0 = m_y + 2;
            int16_t x1 = m_x + m_boxSize - 3;
            int16_t y1 = m_y + m_boxSize - 3;

            displayInterface.drawLine(x0, y0, x1, y1, checkColor);
            displayInterface.drawLine(x0, y1, x1, y0, checkColor);
      }

      // Draw label (if any)
      if (m_label && strlen(m_label) > 0) {
            displayInterface.setTextSize(m_textSize);
            displayInterface.setTextColor(labelColor);

            int16_t  x1, y1;
            uint16_t w, h;
            displayInterface.getTextBounds(m_label, 0, 0, &x1, &y1, &w, &h);

            // Horizontal: box + small gap, then label
            int16_t gap = 4;
            int16_t tx  = m_x + m_boxSize + gap;

            // Vertical: center label relative to box
            int16_t ty = m_y + (m_boxSize - h) / 2;

            displayInterface.setCursor(tx, ty);
            displayInterface.print(m_label);
      }
}



// Handle click event: toggle checkbox
//
void AFCheckbox::onClick(const AFEvent& e) {
      unused(e);
      toggle();
}