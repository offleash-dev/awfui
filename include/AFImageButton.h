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
    AFImageButton() = default;  // Default constructor for stack objects
    AFImageButton(int16_t x, int16_t y, const AFImage* img, ID_TYPE id = 0);

    // Initialize method for stack objects
    void init(int16_t x, int16_t y, const AFImage* img, ID_TYPE id = 0);

    void setSelectable(bool selectable) { m_selectable = selectable; }
    void setSelected(bool selected) { m_selected = selected; }
    bool isSelected() const { return m_selected; }
    
    void setPressedImage(const AFImage* img) { m_imagePressed = img; }
    void setDisabledImage(const AFImage* img) { m_imageDisabled = img; }
    void setSelectedImage(const AFImage* img) { m_imageSelected = img; }

    void draw(AFDisplayInterface& displayInterface) override;


protected:
    bool m_selected = false;
    bool m_selectable = false;
    
    void onClick(const AFEvent& e) override;

private:
    const AFImage* m_image;
    const AFImage* m_imagePressed = nullptr;
    const AFImage* m_imageDisabled = nullptr;
    const AFImage* m_imageSelected = nullptr;
};