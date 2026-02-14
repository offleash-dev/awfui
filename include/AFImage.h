#pragma once

//// AFCheckbox.h
//// Part of the AWFUI library
////
//// Lightweight checkbox widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <stdint.h>



//// AImage
//// Part of the AWFUI library
////
//// Lightweight image class for image based widgets.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

// Read image from memory.  Format is header + data.
// Header is AFImageHeader.
// Data is raw pixel data in the format specified in the header.
// Note that the header and data are little-endian.
// 
// Useful conversion tools:
// - Adafruit’s image2cpp tool
// - LVGL’s image converter

// An example image:
//
// 'equalsIcon', 15x18px, 1 bit
// const unsigned char epd_bitmap_basicVariableIcon[] PROGMEM = {
//     0x0F, 0x00,   // width  = 15
//     0x12, 0x00,   // height = 18
//     0x01,         // format = 1-bit
//     0x00,         // reserved
//
//     // pixel data follows...
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x1f, 0xf0, 0x1f, 0xf0, 0x1f, 0xf0,
//     0x00, 0x00, 0x0f, 0xe0, 0x1f, 0xf0, 0x1f, 0xf0, 0x0f, 0xe0,
//     0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//     0x00, 0x00, 0x00, 0x00
// };



enum AFImageFormat {kAFImageFormatRGB565 = 0, kAFImageFormat1bit =1 };  // kAFImageFormatRLE would be 2


struct AFImageHeader { uint16_t w; uint16_t h; uint8_t format; uint8_t reserved; };


// read from memory without header
class AFImage { 
public: 
    // reads from memory with header
    //
    AFImage(const uint8_t* data); 

    // reads from memory without header (raw pixel data)
    //
    AFImage(uint8_t format, const uint8_t* data, uint16_t w, uint16_t h); 

    uint16_t width() const { return m_w; } 
    uint16_t height() const { return m_h; } 
    uint8_t format() const { return m_format; } 
    const uint8_t* pixels() const { return m_data; }


private:
    uint8_t m_format;
    const uint8_t* m_data;
    uint16_t m_w;
    uint16_t m_h;
};

