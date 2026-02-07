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
                       int16_t boxSize,
                       uint32_t id,
                       const char* labelText)
    : AFWidget(x, y, boxSize, boxSize, id),
      m_label(labelText),
      m_boxSize(static_cast<uint8_t>(boxSize)) {

      const AFTheme& theme = AFWorld::instance()->getTheme();

      if (boxSize == 0) {
            m_boxSize = 16; // Default box size if not specified
      }
      
      m_boxColor    = theme.bgColor;
      m_checkColor  = theme.accentColor;
      m_borderColor = theme.borderColor;
      m_labelColor  = theme.fgColor;
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
            m_onChangeCallback(m_checked);
      }
}



// Draw the checkbox
//
void AFCheckbox::draw(AFDisplayInterface& gfx) {
      if (!m_visible) {
            return;
      }

      const AFTheme& theme = AFWorld::instance()->getTheme();
      uint8_t radius       = theme.cornerRadius;
      uint16_t bgColor     = theme.bgColor;
      uint16_t boxColor    = theme.bgColor;
      uint16_t checkColor  = theme.accentColor;
      uint16_t borderColor = theme.borderColor;
      uint16_t labelColor  = theme.fgColor;

      // set colors to theme colors if disabled
      if (!m_enabled) {
            boxColor    = theme.disabledBgColor;
            checkColor  = theme.disabledFgColor;
            borderColor = theme.borderColor;
            labelColor  = theme.disabledFgColor;
      }

      // Draw checkbox box
      gfx.fillRoundRect(m_x, m_y, m_boxSize, m_boxSize, radius, boxColor);
      gfx.drawRoundRect(m_x, m_y, m_boxSize, m_boxSize, radius, borderColor);

      // Draw check mark if checked
      if (m_checked) {
            // Simple "X" check mark
            int16_t x0 = m_x + 2;
            int16_t y0 = m_y + 2;
            int16_t x1 = m_x + m_boxSize - 3;
            int16_t y1 = m_y + m_boxSize - 3;

            gfx.drawLine(x0, y0, x1, y1, checkColor);
            gfx.drawLine(x0, y1, x1, y0, checkColor);
      }

      // Draw label (if any)
      if (m_label && strlen(m_label) > 0) {
            gfx.setTextSize(m_textSize);
            gfx.setTextColor(labelColor);

            int16_t  x1, y1;
            uint16_t w, h;
            gfx.getTextBounds(m_label, 0, 0, &x1, &y1, &w, &h);

            // Horizontal: box + small gap, then label
            int16_t gap = 4;
            int16_t tx  = m_x + m_boxSize + gap;

            // Vertical: center label relative to box
            int16_t ty = m_y + (m_boxSize - h) / 2;

            gfx.setCursor(tx, ty);
            gfx.print(m_label);
      }
}



// Handle click event: toggle checkbox
//
void AFCheckbox::onClick(const AFEvent& e) {
      unused(e);
      toggle();
}
