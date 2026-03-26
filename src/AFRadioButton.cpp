//// AFRadioButton.cpp
//// Implementation of the AWFUI radio button widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include <string.h>



#include "AFWorld.h"
#include "AFRadioButton.h"
#include "AFRadioButtonGroup.h"



AFRadioButton::AFRadioButton(int16_t x,
                             int16_t y,
                             int16_t radius,
                             uint32_t id,
                             const char* labelText)
    : AFWidget(x, y, radius * 2, radius * 2, id),
      m_label(labelText),
      m_radius(radius)
{
    if (radius == 0) {
        m_radius = 8; // Default radius if not specified
    }   

    const AFTheme& theme = AFWorld::instance()->getTheme();
    m_circleColor = theme.widgetBorderColor;
    m_dotColor = theme.widgetAccentColor;
    m_labelColor = theme.widgetFgColor;

    m_textSize    = theme.widgetTextSize;
}



// Initialize method for stack objects
//
void AFRadioButton::init(int16_t x,
                          int16_t y,
                          int16_t radius,
                          uint32_t id,
                          const char* labelText) {
      // Initialize the base AFWidget
      m_x = x;
      m_y = y;
      m_width = radius * 2;
      m_height = radius * 2;
      m_id = id;
      m_label = labelText;
      m_radius = radius;
      
      if (radius == 0) {
            m_radius = 8; // Default radius if not specified
      }   
      
      const AFTheme& theme = AFWorld::instance()->getTheme();
      m_circleColor = theme.widgetBorderColor;
      m_dotColor = theme.widgetAccentColor;
      m_labelColor = theme.widgetFgColor;
      m_textSize = theme.widgetTextSize;
}



void AFRadioButton::setLabel(const char* text) {
    m_label = text;
    markDirty();
}



void AFRadioButton::setSelected(bool sel) {
    if (m_selected == sel)
        return;

    m_selected = sel;
    markDirty();

    if (sel && m_onSelectCallback)
        m_onSelectCallback(*this, m_id);
}



void AFRadioButton::setColors(uint16_t circleColor, uint16_t dotColor, uint16_t labelColor) {
      m_circleColor = circleColor;
      m_dotColor = dotColor;
      m_labelColor = labelColor;

      markDirty();
}



void AFRadioButton::draw(AFDisplayInterface& displayInterface) {
    if (!isVisible())
        return;

    uint16_t circleColor = m_circleColor;
    uint16_t dotColor    = m_dotColor;
    uint16_t labelColor  = m_labelColor;

    if (!isEnabled()) {
        AFTheme theTheme = AFWorld::instance()->getTheme();
        circleColor = theTheme.widgetDisabledFgColor;
        dotColor    = theTheme.widgetDisabledFgColor;
        labelColor  = theTheme.widgetDisabledFgColor;
    }

    // Clear interior so deselected dot is erased
    uint16_t bgColor = AFWorld::instance()->getTheme().screenBgColor;
    displayInterface.fillCircle(m_x + m_radius, m_y + m_radius, m_radius, bgColor);

    // Outer circle
    displayInterface.drawCircle(m_x + m_radius, m_y + m_radius, m_radius, circleColor);

    // Filled dot if selected
    if (m_selected) {
        displayInterface.fillCircle(m_x + m_radius, m_y + m_radius, m_radius / 2, dotColor);
    }

    // Label
    if (m_label && strlen(m_label) > 0) {
        displayInterface.setTextSize(m_textSize);
        displayInterface.setTextColor(labelColor);

        int16_t x1, y1;
        uint16_t w, h;
        displayInterface.getTextBounds(m_label, 0, 0, &x1, &y1, &w, &h);

        int16_t gap = 6;
        int16_t tx = m_x + m_radius * 2 + gap;
        int16_t ty = m_y + (m_radius * 2 - h) / 2;

        displayInterface.setCursor(tx, ty);
        displayInterface.print(m_label);
    }
}



void AFRadioButton::onClick(const AFEvent& e) {
    unused(e);

    if (m_group) {
        m_group->select(this);
    } else {
        // Standalone radio button (rare but allowed)
        setSelected(true);
    }
}