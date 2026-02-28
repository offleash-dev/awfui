#pragma once

//// AFSlider.h
//// Part of the AWFUI library
////
//// Lightweight slider widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.
////
////  Example usage:
////  AFSlider slider(20, 40, 200, 30);
////  slider.setRange(0, 255);
////  slider.setValue(128);
////  ui.add(&slider);


#include "AFDisplayInterface.h"
#include "AFWidget.h"
#include "AFEvent.h"



// Callback type for slider release (passes final value)
using AFSliderCallback = void (*)(int value);



class AFSlider : public AFWidget {
public:
    AFSlider(int16_t x, int16_t y, int16_t w, int16_t h);

    void setRange(int minVal, int maxVal);

    void setValue(int v);
    int value() const { return m_value; }

    void setColors(uint16_t track, uint16_t fill, uint16_t thumb);

    void setOnReleaseCallback(AFSliderCallback cb) {
        m_onReleaseCallback = cb;
    }


protected:
    virtual void draw(AFDisplayInterface& d) override;

    virtual void onPress(const AFEvent& e) override;
    virtual void onMove(const AFEvent& e) override;
    virtual void onRelease(const AFEvent& e) override;


private:
    int m_min, m_max;
    int m_value;

    uint16_t m_trackColor;
    uint16_t m_fillColor;
    uint16_t m_thumbColor;

    static constexpr int kThumbW = 12;
    static constexpr int kThumbH = 16;

    void clampValue();

    int mapValueToPixels(int v) const;
    int mapPixelsToValue(int px) const;

    AFSliderCallback m_onReleaseCallback = nullptr;
};
