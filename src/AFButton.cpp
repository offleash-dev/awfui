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
      m_bgColor            = theme.widgetBgColor;
      m_fgColor            = theme.widgetFgColor;
      m_borderColor        = theme.widgetBorderColor;
      m_bgColorPressed     = theme.widgetAccentColor;
      m_fgColorPressed     = theme.widgetFgColor;
      m_borderColorPressed = theme.widgetBorderColor;
      m_textSize           = theme.widgetTextSize;
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
void AFButton::draw(AFDisplayInterface& displayInterface) {
      if (!m_visible)
            return;

      // Choose colors based on pressed state
      uint16_t bg     = m_pressed ? m_bgColorPressed : m_bgColor;
      uint16_t fg     = m_pressed ? m_fgColorPressed : m_fgColor;
      uint16_t border = m_pressed ? m_borderColorPressed : m_borderColor;
      // now override that if disabled
      if (!m_enabled) {
            bg     = AFWorld::instance()->getTheme().widgetDisabledBgColor;
            fg     = AFWorld::instance()->getTheme().widgetDisabledFgColor;
            border = AFWorld::instance()->getTheme().widgetBorderColor;
      }

      uint8_t radius = AFWorld::instance()->getTheme().widgetCornerRadius;

      // Draw background
      displayInterface.fillRoundRect(m_x, m_y, m_width, m_height, radius, bg);

      // Draw border
      if (m_showBorder) {
            displayInterface.drawRoundRect(m_x, m_y, m_width, m_height, radius, border);
      }

      // Draw label with justification
      if (m_label && strlen(m_label) > 0) {
            displayInterface.setTextSize(m_textSize);
            displayInterface.setTextColor(fg);
            displayInterface.drawTextJustified(m_label, m_x, m_y, m_width, m_height, m_justification);
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
            m_onClickCallback(*this);
      }
}