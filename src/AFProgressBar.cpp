//// AFProgressBar.cpp
//// Part of the AWFUI library
////
//// Lightweight progress bar widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdio.h>

#include "AFProgressBar.h"
#include "AFPanel.h"
#include "AFDisplayInterface.h"
#include "AFWorld.h"
#include "AFTheme.h"



AFProgressBar::AFProgressBar(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id)
        : AFValueWidget(x, y, w, h, id)
{
      const AFTheme& theme = AFWorld::instance()->getTheme();
      m_bgColor            = theme.widgetBgColor;
      m_borderColor        = theme.widgetBorderColor;
      m_fillColor          = theme.widgetFgColor;
}



// Initialize method for stack objects
//
void AFProgressBar::init(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id) 
{
      // Initialize the base AFValueWidget
      AFValueWidget::init(x, y, w, h, id);
      
      const AFTheme& theme = AFWorld::instance()->getTheme();
      m_bgColor            = theme.widgetBgColor;
      m_borderColor        = theme.widgetBorderColor;
      m_fillColor          = theme.widgetFgColor;
}



void AFProgressBar::setColors(uint16_t border, uint16_t bg, uint16_t fill) {
        m_borderColor = border;
        m_bgColor = bg;
        m_fillColor = fill;
        markDirty();
}



void AFProgressBar::showText(bool enable) {
        m_showText = enable;
        markDirty();
}


 
//// two ways of drawing a widget for example
//// in the first, the widget is fully in control, but needs local->screen awareness
//// in the second, the widget is given a localized draw
//// either is ok.
 void AFProgressBar::draw(AFDisplayInterface& d) {
        // Convert local coordinates to screen coordinates if needed
        int16_t screenX = m_x;
        int16_t screenY = m_y;
        
        // If we have a parent panel, convert local to screen coordinates
        if (m_parent) {
            screenX = static_cast<AFPanel*>(m_parent)->toScreenX(m_x);
            screenY = static_cast<AFPanel*>(m_parent)->toScreenY(m_y);
        }

        // Border
        d.drawRect(screenX, screenY, m_width, m_height, m_borderColor);

        // Background
        d.fillRect(screenX + 1, screenY + 1, m_width - 2, m_height - 2, m_bgColor);

        // Filled portion
        int filled = mapValueToPixels(m_value);
        if (filled > 0) {
            d.fillRect(screenX + 1, screenY + 1, static_cast<int16_t>(filled), m_height - 2, m_fillColor);
        }

        // Optional text overlay
        if (m_showText) {
            char buf[8];
            int pct = (range() > 0) ? (m_value - m_minimum) * 100 / range() : 0;
            snprintf(buf, sizeof(buf), "%d%%", pct);

            d.setTextColor(m_borderColor);
            d.setTextSize(1);
            d.drawTextJustified(buf, screenX, screenY, m_width, m_height, AFJustificationCenter);
        }
    }



void AFProgressBar::draw(AFDisplayInterface& d, int16_t screenOffsetX, int16_t screenOffsetY) {
        // Use screen coordinates for drawing
        const int16_t x0 = m_x + screenOffsetX;
        const int16_t y0 = m_y + screenOffsetY;

        // Border
        d.drawRect(x0, y0, m_width, m_height, m_borderColor);

        // Background
        d.fillRect(x0 + 1, y0 + 1, m_width - 2, m_height - 2, m_bgColor);

        // Filled portion
        int filled = mapValueToPixels(m_value);
        if (filled > 0) {
            d.fillRect(x0 + 1, y0 + 1, static_cast<int16_t>(filled), m_height - 2, m_fillColor);
        }

        // Optional text overlay
        if (m_showText) {
            char buf[8];
            int pct = (range() > 0) ? (m_value - m_minimum) * 100 / range() : 0;
            snprintf(buf, sizeof(buf), "%d%%", pct);

            d.setTextColor(m_borderColor);
            d.setTextSize(1);
            d.drawTextJustified(buf, x0, y0, m_width, m_height, AFJustificationCenter);
        }
    }

int AFProgressBar::mapValueToPixels(int v) const {
        if (range() == 0) return 0;
        return (int32_t)(v - m_minimum) * (m_width - 2) / range();
}

