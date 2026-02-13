//// AFImageButton.cpp
//// Part of the AWFUI library
////
//// Lightweight image button widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include "AFImageButton.h"
#include "AFWorld.h"



AFImageButton::AFImageButton(int16_t x, int16_t y, const AFImage* img, uint32_t id)
    : AFButton(x, y, img ? img->width() : 0, img ? img->height() : 0, id),
      m_img(img)
{
}



void AFImageButton::draw(AFDisplayInterface& gfx)
{
    if (!m_visible || !m_img) {
        return;
    }

    if (m_img->format() == kAFImageFormat1bit) {
        const AFTheme& theme = AFWorld::instance()->getTheme();
        uint16_t color = m_enabled ? theme.widgetFgColor : theme.widgetDisabledFgColor;
        gfx.drawBitmap(m_x, m_y, m_img->pixels(), m_img->width(), m_img->height(), color);
    } else {
        gfx.drawRGBBitmap(m_x, m_y, reinterpret_cast<const uint16_t*>(m_img->pixels()),
                          m_img->width(), m_img->height());
    }
}