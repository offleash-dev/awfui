// AFButton.cpp

#include <string.h>



#include "AFButton.h"



// Constructor
//
AFButton::AFButton(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id, const char* labelText)
    : AFWidget(x, y, w, h, id), m_label(labelText), 
      m_bgColor(0xFFFF),                                           // white
      m_fgColor(0x0000),                                           // black
      m_borderColor(0x0000),                                       // black
      m_bgColorPressed(0xC618),                                    // light gray
      m_fgColorPressed(0x0000),                                    // black
      m_borderColorPressed(0x0000) {
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
void AFButton::draw(Adafruit_GFX& gfx) {
      if (!m_visible)
            return;

      // Choose colors based on pressed state
      uint16_t bg     = m_pressed ? m_bgColorPressed : m_bgColor;
      uint16_t fg     = m_pressed ? m_fgColorPressed : m_fgColor;
      uint16_t border = m_pressed ? m_borderColorPressed : m_borderColor;

      // Draw background
      gfx.fillRect(m_x, m_y, m_width, m_height, bg);

      // Draw border
      gfx.drawRect(m_x, m_y, m_width, m_height, border);

      // Draw label centered
      if (m_label && strlen(m_label) > 0) {
            gfx.setTextSize(m_textSize);
            gfx.setTextColor(fg);

            int16_t  x1, y1;
            uint16_t w, h;
            gfx.getTextBounds(m_label, 0, 0, &x1, &y1, &w, &h);

            int16_t tx = m_x + (m_width - w) / 2;
            int16_t ty = m_y + (m_height - h) / 2;

            gfx.setCursor(tx, ty);
            gfx.print(m_label);
      }
}

// Event handling
//

// Handle press event
//
void AFButton::onPress(const AFEvent& e) {
    m_pressed = true;
    markDirty();
}



// Handle release event
//
void AFButton::onRelease(const AFEvent& e) {
    m_pressed = false;
    markDirty();
}



// Handle click event
//
void AFButton::onClick(const AFEvent& e) {
      if (m_onClickCallback) {
            m_onClickCallback();
      }
}
