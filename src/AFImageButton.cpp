//// AFImageButton.cpp
//// Part of the AWFUI library
////
//// Lightweight image button widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>

#include "AFImageButton.h"
#include "AFWorld.h"
#include "AFTheme.h"



AFImageButton::AFImageButton(int16_t x, int16_t y, const AFImage* img, uint32_t id)
    : AFButton(x, y, img ? img->width() : 0, img ? img->height() : 0, id),
      m_image(img)
{
}



void AFImageButton::onClick(const AFEvent& e) {
    (void)e; // Suppress unused parameter warning
    if (m_selectable) {
        m_selected = !m_selected;
        markDirty();
    }
    
    // Call the base class method to handle the callback
    AFButton::onClick(e);
}



void AFImageButton::draw(AFDisplayInterface& displayInterface)
{
    if (!m_visible || !m_image) {
        return;
    }

    const AFImage* image = m_image;
    if (m_enabled) {
        if (m_pressed) {
            if (m_imagePressed) {
                image = m_imagePressed;
            } else if (m_imageSelected && m_selected) {
                image = m_imageSelected;
            }
        }
    } else {
        if (m_imageDisabled) {
            image = m_imageDisabled;
        }
    }

    if (image->format() == kAFImageFormat1bit) {
        const AFTheme& theme = AFWorld::instance()->getTheme();
        uint16_t color = m_enabled ? theme.widgetFgColor : theme.widgetDisabledFgColor;
        displayInterface.drawBitmap(m_x, m_y, image->pixels(), image->width(), image->height(), color);
    } else {
        displayInterface.drawRGBBitmap(m_x, m_y, reinterpret_cast<const uint16_t*>(image->pixels()),
                          image->width(), image->height());
    }

    if ((m_pressed && m_imagePressed == nullptr) || (m_selected && m_imageSelected == nullptr)) {
        AFRect buttonRect(m_x, m_y, m_width, m_height);
        int16_t insetLines = AFWorld::instance()->getTheme().widgetBorderWidth;
 
        uint16_t borderColor = AFWorld::instance()->getTheme().widgetFgColor;
        if (m_selected)
            borderColor = AFWorld::instance()->getTheme().widgetAccentColor;

        for (int16_t i = 0; i < insetLines; i++) {
            buttonRect.inset();
            displayInterface.drawRect(buttonRect.x, buttonRect.y, buttonRect.w, buttonRect.h, borderColor);
        }
    }
}