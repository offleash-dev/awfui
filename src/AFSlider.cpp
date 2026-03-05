//// AFSlider.cpp
//// Part of the AWFUI library
////
//// Lightweight slider widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdlib.h>
#include "AFSlider.h"
#include "AFWorld.h"


AFSlider::AFSlider(int16_t x, int16_t y, int16_t w, int16_t h)
    : AFValueWidget(x, y, w, h)
{
    const AFTheme& theme = AFWorld::instance()->getTheme();
    m_trackColor = theme.widgetBorderColor;
    m_fillColor  = theme.widgetFgColor;
    m_thumbColor = theme.widgetAccentColor;
}



void AFSlider::setColors(uint16_t track, uint16_t fill, uint16_t thumb) {
    m_trackColor = track;
    m_fillColor = fill;
    m_thumbColor = thumb;
    markDirty();
}



void AFSlider::draw(AFDisplayInterface& d) {
      if (!m_visible)
            return;

      uint16_t trackColor = m_trackColor;
      uint16_t fillColor  = m_fillColor;
      uint16_t thumbColor = m_thumbColor;
      if (!m_enabled) {
            const AFTheme& theme = AFWorld::instance()->getTheme();
            trackColor = theme.widgetDisabledFgColor;
            fillColor  = theme.widgetDisabledFgColor;
            thumbColor = theme.widgetDisabledFgColor;
      }

      d.fillRect(m_x, m_y + m_height/2 - 2, m_width, 4, trackColor);

      // Filled portion
      int filled = mapValueToPixels(m_value);
      d.fillRect(m_x, static_cast<int16_t>(m_y + m_height/2 - 2), static_cast<int16_t>(filled), 4, fillColor);

      // Thumb
      int thumbX = m_x + filled - kThumbW/2;
      int thumbY = m_y + m_height/2 - kThumbH/2;
      d.fillRect(static_cast<int16_t>(thumbX), static_cast<int16_t>(thumbY), kThumbW, kThumbH, thumbColor);
}



void AFSlider::onPress(const AFEvent& e) {
    updateValueFromTouch(e);
}



void AFSlider::onMove(const AFEvent& e) {
    updateValueFromTouch(e);
}



void AFSlider::onRelease(const AFEvent& e) {
      unused(e);

      if (m_onReleaseCallback) {
          m_onReleaseCallback(m_value);
    }
}



void AFSlider::updateValueFromTouch(const AFEvent& e) {
    int localX = e.x - m_x;
    int newVal = mapPixelsToValue(localX);

    if (newVal != m_value) {
        m_value = newVal;
        clampValue();
        markDirty();
    }
}



int AFSlider::mapValueToPixels(int v) const {
      if (range() == 0) 
            return 0;

      return (int32_t)(v - m_minimum) * (m_width - kThumbW) / range() + kThumbW/2;
}



int AFSlider::mapPixelsToValue(int px) const {
      if (px < 0) 
            px = 0;
      if (px > m_width) 
            px = m_width;

      if (range() == 0) 
            return m_minimum;

    int v = (int32_t)(px - kThumbW/2) * range() / (m_width - kThumbW) + m_minimum;
    if (v < m_minimum) 
            v = m_minimum;
    if (v > m_maximum) 
            v = m_maximum;
            
    return v;
}
