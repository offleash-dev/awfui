//// AFRadioButton.cpp
//// Implementation of the AWFUI dialog widget.
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
    m_circleColor = theme.borderColor;
    m_dotColor    = theme.accentColor;
    m_labelColor  = theme.fgColor;
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
        m_onSelectCallback(m_id);
}



void AFRadioButton::draw(AFDisplayInterface& gfx) {
    if (!m_visible)
        return;

    uint16_t circleColor = AFWorld::instance()->getTheme().borderColor;
    uint16_t dotColor    = AFWorld::instance()->getTheme().accentColor;
    uint16_t labelColor  = AFWorld::instance()->getTheme().fgColor;

    if (!m_enabled) {
        circleColor = AFWorld::instance()->getTheme().disabledFgColor;
        dotColor    = AFWorld::instance()->getTheme().disabledFgColor;
        labelColor  = AFWorld::instance()->getTheme().disabledFgColor;
    }

    // Outer circle
    gfx.drawCircle(m_x + m_radius, m_y + m_radius, m_radius, circleColor);

    // Filled dot if selected
    if (m_selected) {
        gfx.fillCircle(m_x + m_radius, m_y + m_radius, m_radius / 2, dotColor);
    }

    // Label
    if (m_label && strlen(m_label) > 0) {
        gfx.setTextSize(m_textSize);
        gfx.setTextColor(labelColor);

        int16_t x1, y1;
        uint16_t w, h;
        gfx.getTextBounds(m_label, 0, 0, &x1, &y1, &w, &h);

        int16_t gap = 6;
        int16_t tx = m_x + m_radius * 2 + gap;
        int16_t ty = m_y + (m_radius * 2 - h) / 2;

        gfx.setCursor(tx, ty);
        gfx.print(m_label);
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
