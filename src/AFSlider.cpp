//// AFSlider.cpp
//// Part of the AWFUI library
////
//// Lightweight slider widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdlib.h>
#include "AFSlider.h"



AFSlider::AFSlider(int16_t x, int16_t y, int16_t w, int16_t h)
    : AFWidget(x, y, w, h),
      m_min(0), m_max(100), m_value(0),
      m_trackColor(0x7BEF),   // light gray
      m_fillColor(0x001F),    // blue
      m_thumbColor(0xFFFF)    // white
{}



void AFSlider::setRange(int minVal, int maxVal) {
    m_min = minVal;
    m_max = maxVal;
    clampValue();
    markDirty();
}



void AFSlider::setValue(int v) {
    m_value = v;
    clampValue();
    markDirty();
}



void AFSlider::setColors(uint16_t track, uint16_t fill, uint16_t thumb) {
    m_trackColor = track;
    m_fillColor = fill;
    m_thumbColor = thumb;
    markDirty();
}



void AFSlider::draw(AFDisplayInterface& d) {
    d.fillRect(m_x, m_y + m_height/2 - 2, m_width, 4, m_trackColor);

    // Filled portion
    int filled = mapValueToPixels(m_value);
    d.fillRect(m_x, m_y + m_height/2 - 2, filled, 4, m_fillColor);

    // Thumb
    int thumbX = m_x + filled - kThumbW/2;
    int thumbY = m_y + m_height/2 - kThumbH/2;
    d.fillRect(thumbX, thumbY, kThumbW, kThumbH, m_thumbColor);
}



void AFSlider::onPress(const AFEvent& e) {
    int localX = e.x - m_x;
    int newVal = mapPixelsToValue(localX);

    if (newVal != m_value) {
        m_value = newVal;
        clampValue();
        markDirty();
    }
}



void AFSlider::onMove(const AFEvent& e) {
    int localX = e.x - m_x;
    int newVal = mapPixelsToValue(localX);

    if (newVal != m_value) {
        m_value = newVal;
        clampValue();
        markDirty();
    }
}



void AFSlider::onRelease(const AFEvent& e) {
    if (m_onReleaseCallback) {
        m_onReleaseCallback(m_value);
    }
}



void AFSlider::clampValue() {
    if (m_value < m_min) m_value = m_min;
    if (m_value > m_max) m_value = m_max;
}



int AFSlider::mapValueToPixels(int v) const {
    int range = m_max - m_min;
    if (range == 0) return 0;
    return (int32_t)(v - m_min) * (m_width - kThumbW) / range + kThumbW/2;
}



int AFSlider::mapPixelsToValue(int px) const {
    if (px < 0) px = 0;
    if (px > m_width) px = m_width;

    int range = m_max - m_min;
    if (range == 0) return m_min;

    int v = (int32_t)(px - kThumbW/2) * range / (m_width - kThumbW) + m_min;
    if (v < m_min) v = m_min;
    if (v > m_max) v = m_max;
    return v;
}
