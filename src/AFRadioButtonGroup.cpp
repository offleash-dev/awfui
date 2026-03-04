//// AFRadioButtonGroup.cpp
//// Implementation of the AWFUI radio button group.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include "AFRadioButtonGroup.h"
#include "AFRadioButton.h"



AFRadioButtonGroup::AFRadioButtonGroup() {
    for (uint8_t i = 0; i < MAX_WIDGETS_PER_PANEL; ++i)
        m_buttons[i] = nullptr;
}



bool AFRadioButtonGroup::addButton(AFRadioButton* btn) {
    if (m_count >= MAX_WIDGETS_PER_PANEL)
        return false;

    m_buttons[m_count++] = btn;
    btn->m_group = this;
    return true;
}



void AFRadioButtonGroup::select(AFRadioButton* btn) {
    if (m_selected == btn)
        return;

    // Unselect all
    for (uint8_t i = 0; i < m_count; ++i) {
        if (m_buttons[i])
            m_buttons[i]->setSelected(false);
    }

    // Select the new one
    btn->setSelected(true);
    m_selected = btn;

    if (m_onChangeCallback)
        m_onChangeCallback(*this, btn->getId());
}