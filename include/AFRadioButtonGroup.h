#pragma once

//// AFRadioButtonGroup.h
//// Part of the AWFUI library
////
//// Manages exclusivity for a set of AFRadioButtons.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>
#include "AFBase.h"



class AFRadioButton;
class AFRadioButtonGroup;

using AFRadioGroupCallback = void (*)(AFRadioButtonGroup& sender, uint32_t id);



class AFRadioButtonGroup {
public:
    AFRadioButtonGroup();

    bool addButton(AFRadioButton* btn);
    void select(AFRadioButton* btn);

    AFRadioButton* getSelected() const { return m_selected; }

    void setOnChangeCallback(AFRadioGroupCallback cb) {
        m_onChangeCallback = cb;
    }


private:
    AFRadioButton* m_buttons[MAX_WIDGETS_PER_PANEL];
    uint8_t        m_count = 0;

    AFRadioButton* m_selected = nullptr;
    AFRadioGroupCallback m_onChangeCallback = nullptr;
};