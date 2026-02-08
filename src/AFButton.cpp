//// AFButton.cpp
//// Implementation of the AWFUI button widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <string.h>



#include "AFButton.h"
#include "AFWorld.h"



// Constructor
//
AFButton::AFButton(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id, const char* labelText)
    : AFWidget(x, y, w, h, id), m_label(labelText) {
      const AFTheme& theme = AFWorld::instance()->getTheme();
      m_bgColor            = theme.bgColor;
      m_fgColor            = theme.fgColor;
      m_borderColor        = theme.borderColor;
      m_bgColorPressed     = theme.accentColor;
      m_fgColorPressed     = theme.fgColor;
      m_borderColorPressed = theme.borderColor;
}



// Set the Label text
//
void AFButton::setLabel(const char* text) {
    m_label = text;
    markDirty();
}



// Set the Colors for the button (background, foreground, border) when in a normal state
//
void AFButton::setColors(uint16_t bg, uint16_t fg, uint16_t border) {
    m_bgColor     = bg;
    m_fgColor     = fg;
    m_borderColor = border;
    markDirty();
}



// Set the Colors for the button (background, foreground, border) when in a pressed state
//
void AFButton::setPressedColors(uint16_t bg, uint16_t fg, uint16_t border) {
    m_bgColorPressed     = bg;
    m_fgColorPressed     = fg;
    m_borderColorPressed = border;
    // No markDirty() - these only matter when pressed
}



// Draw the button
//
void AFButton::draw(AFDisplayInterface& gfx) {
      if (!m_visible)
            return;

      // Choose colors based on pressed state
      uint16_t bg     = m_pressed ? m_bgColorPressed : m_bgColor;
      uint16_t fg     = m_pressed ? m_fgColorPressed : m_fgColor;
      uint16_t border = m_pressed ? m_borderColorPressed : m_borderColor;
      // now override that if disabled
      if (!m_enabled) {
            bg     = AFWorld::instance()->getTheme().disabledBgColor;
            fg     = AFWorld::instance()->getTheme().disabledFgColor;
            border = AFWorld::instance()->getTheme().borderColor;
      }

      uint8_t radius = AFWorld::instance()->getTheme().cornerRadius;

      // Draw background
      gfx.fillRoundRect(m_x, m_y, m_width, m_height, radius, bg);

      // Draw border
      gfx.drawRoundRect(m_x, m_y, m_width, m_height, radius, border);

      // Draw label with justification
      if (m_label && strlen(m_label) > 0) {
            gfx.setTextSize(m_textSize);
            gfx.setTextColor(fg);

            int16_t  x1, y1;
            uint16_t w, h;
            gfx.getTextBounds(m_label, 0, 0, &x1, &y1, &w, &h);

            int16_t tx, ty;

            // Horizontal justification
            switch (m_justification) {
                  case AFJustificationCenter:
                        tx = m_x + (m_width - w) / 2;
                        break;
                  case AFJustificationRight:
                        tx = m_x + m_width - w;
                        break;
                  case AFJustificationLeft:
                  default:
                        tx = m_x;
                        break;
            }

            // Vertical centering
            ty = m_y + (m_height - h) / 2;

            gfx.setCursor(tx, ty);
            gfx.print(m_label);
      }
}

// Event handling
//

// Handle press event
//
void AFButton::onPress(const AFEvent& e) {
    unused(e);
    m_pressed = true;
    markDirty();
}



// Handle release event
//
void AFButton::onRelease(const AFEvent& e) {
    unused(e);
    m_pressed = false;
    markDirty();
}



// Handle click event
//
void AFButton::onClick(const AFEvent& e) {
      unused(e);
      if (m_onClickCallback) {
            m_onClickCallback();
      }
}