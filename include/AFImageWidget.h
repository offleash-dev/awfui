#pragma once

//// AFImageWidget.h
//// Part of the AWFUI library
////
//// Lightweight image widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include "AFImage.h"
#include "AFWidget.h"



class AFImageWidget : public AFWidget {
public:
    AFImageWidget(int16_t x, int16_t y, const AFImage* img, uint32_t id = 0);

    void draw(AFDisplayInterface& displayInterface) override;


private:
    const AFImage* m_img;
};