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
#include "AFValueWidget.h"
#include "AFEvent.h"

// Forward declaration
class AFSlider;

// Callback type for slider release (passes sender and final value)
using AFSliderCallback = void (*)(AFSlider& sender, int value);



class AFSlider : public AFValueWidget {
public:
    AFSlider(int16_t x, int16_t y, int16_t w, int16_t h);

    void setColors(uint16_t track, uint16_t fill, uint16_t thumb);


    void setOnReleaseCallback(AFSliderCallback cb) {
        m_onReleaseCallback = cb;
    }


    void setOnMoveCallback(AFSliderCallback cb) {
        m_onMoveCallback = cb;
    }


protected:
    virtual void draw(AFDisplayInterface& d) override;

    virtual void onPress(const AFEvent& e) override;
    virtual void onMove(const AFEvent& e) override;
    virtual void onRelease(const AFEvent& e) override;


private:
    uint16_t m_trackColor;
    uint16_t m_fillColor;
    uint16_t m_thumbColor;

    static constexpr int kThumbW = 12;
    static constexpr int kThumbH = 16;

    // Previous thumb position for erasure
    int16_t m_lastThumbX = -1;
    int16_t m_lastThumbY = -1;

    void updateValueFromTouch(const AFEvent& e);
    int mapValueToPixels(int v) const;
    int mapPixelsToValue(int px) const;

    AFSliderCallback m_onReleaseCallback = nullptr;
    AFSliderCallback m_onMoveCallback = nullptr;
};
