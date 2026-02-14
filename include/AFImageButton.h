#pragma once

//// AFImageButton.h
//// Part of the AWFUI library
////
//// Lightweight image button widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include "AFImage.h"
#include "AFButton.h"



class AFImageButton : public AFButton {
public:
    AFImageButton(int16_t x, int16_t y, const AFImage* img, uint32_t id = 0);

    void draw(AFDisplayInterface& gfx) override;


private:
    const AFImage* m_img;
};