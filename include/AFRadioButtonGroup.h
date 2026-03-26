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

using AFRadioGroupIdCallback = void (*)(AFRadioButtonGroup& sender, uint32_t selectedId);
using AFRadioGroupOrdinalCallback = void (*)(AFRadioButtonGroup& sender, int16_t ordinal);



class AFRadioButtonGroup {
public:
    AFRadioButtonGroup();

    int16_t addButton(AFRadioButton* btn);
    void select(AFRadioButton* btn);

    AFRadioButton* getSelected() const { return m_selected; }

    void setOnChangeIdCallback(AFRadioGroupIdCallback cb) {
        m_onChangeIdCallback = cb;
    }

    void setOnChangeOrdinalCallback(AFRadioGroupOrdinalCallback cb) {
        m_onChangeOrdinalCallback = cb;
    }

private:
    AFRadioButton* m_buttons[MAX_WIDGETS_PER_PANEL];
    uint8_t        m_count = 0;

    AFRadioButton* m_selected = nullptr;
    AFRadioGroupIdCallback m_onChangeIdCallback = nullptr;
    AFRadioGroupOrdinalCallback m_onChangeOrdinalCallback = nullptr;
};