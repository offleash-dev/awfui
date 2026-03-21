//// AFImageWidget.cpp
//// Part of the AWFUI library
////
//// Lightweight image widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include "AFImageWidget.h"
#include "AFWorld.h"



AFImageWidget::AFImageWidget(int16_t x, int16_t y, const AFImage* img, ID_TYPE id)
    : AFWidget(x, y, img ? img->width() : 0, img ? img->height() : 0, id),
      m_img(img)
{
}



void AFImageWidget::draw(AFDisplayInterface& displayInterface)
{
    if (!isVisible() || !m_img) {
        return;
    }

    if (m_img->format() == kAFImageFormat1bit) {
        const AFTheme& theme = AFWorld::instance()->getTheme();
        uint16_t color = isEnabled() ? theme.widgetFgColor : theme.widgetDisabledFgColor;
        displayInterface.drawBitmap(m_x, m_y, m_img->pixels(), m_img->width(), m_img->height(), color);
    } else {
        displayInterface.drawRGBBitmap(m_x, m_y, reinterpret_cast<const uint16_t*>(m_img->pixels()),
                          m_img->width(), m_img->height());
    }
}