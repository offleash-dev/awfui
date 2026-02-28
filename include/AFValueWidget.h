#pragma once

//// AFValueWidget.h
//// Part of the AWFUI library
////
//// Abstract base for widgets that display a value within a range
//// (progress bars, sliders, gauges, etc.).
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFWidget.h"



class AFValueWidget : public AFWidget {
public:
    AFValueWidget(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0)
        : AFWidget(x, y, w, h, id),
          m_minimum(0), m_maximum(100), m_value(0) {}

    void setRange(int minimumValue, int maximumValue) {
        m_minimum = minimumValue;
        m_maximum = maximumValue;
        clampValue();
        markDirty();
    }


    void setValue(int v) {
        m_value = v;
        clampValue();
        markDirty();
    }


    int value() const { return m_value; }
    int minimum() const { return m_minimum; }
    int maximum() const { return m_maximum; }


protected:
    int m_minimum, m_maximum;
    int m_value;

    void clampValue() {
        if (m_value < m_minimum) m_value = m_minimum;
        if (m_value > m_maximum) m_value = m_maximum;
    }


    int range() const { return m_maximum - m_minimum; }
};