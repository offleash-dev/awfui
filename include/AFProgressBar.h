#pragma once

//// AFProgressBar.h
//// Part of the AWFUI library
////
//// Lightweight progress bar widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

//  Example usage:
//  
//  AFProgressBar bar(20, 80, 200, 20);
//  bar.setRange(0, 100);
//  bar.setValue(45);
//  bar.showText(true);
//  ui.add(&bar);

#include "AFValueWidget.h"



class AFProgressBar : public AFValueWidget {
public:
    AFProgressBar(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);

    void setColors(uint16_t border, uint16_t bg, uint16_t fill);

    void showText(bool enable);


protected:
    virtual void draw(AFDisplayInterface& d) override;


private:
    uint16_t m_borderColor;
    uint16_t m_bgColor;
    uint16_t m_fillColor;

    bool m_showText = false;

    int mapValueToPixels(int v) const;
};
