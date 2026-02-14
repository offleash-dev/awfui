//// AFFullscreenDialog.cpp
//// Implementation of the AWFUI fullscreen dialog widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFImage.h"



AFImage::AFImage(const uint8_t* data) {
    m_w = data[0] | (data[1] << 8);
    m_h = data[2] | (data[3] << 8);
    m_format = data[4];
    m_data = data + 6;
}



AFImage::AFImage(uint8_t format, const uint8_t* data, uint16_t w, uint16_t h) {
    m_w = w;
    m_h = h;
    m_format = format;
    m_data = data;
}