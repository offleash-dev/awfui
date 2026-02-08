#pragma once

//// AFRadioButton.h
//// Part of the AWFUI library
////
//// Lightweight radio button widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFWidget.h"
#include "AFTheme.h"

class AFRadioButtonGroup;   // forward declaration

using AFRadioCallback = void (*)(uint32_t id);


class AFRadioButton : public AFWidget {
public:
    AFRadioButton(int16_t x,
                  int16_t y,
                  int16_t radius = 0,
                  uint32_t id = 0,
                  const char* label = nullptr);

    void setLabel(const char* text);
    const char* getLabel() const { return m_label; }

    void setSelected(bool sel);
    bool isSelected() const { return m_selected; }

    void setTextSize(uint8_t size) { m_textSize = size; }
    uint8_t getTextSize() const { return m_textSize; }

    void setOnSelectCallback(AFRadioCallback cb) { m_onSelectCallback = cb; }

    void draw(AFDisplayInterface& gfx) override;
    void onClick(const AFEvent& e) override;


private:
    friend class AFRadioButtonGroup;

    const char* m_label;
    bool        m_selected = false;

    uint16_t    m_circleColor;
    uint16_t    m_dotColor;
    uint16_t    m_labelColor;

    uint16_t     m_radius;
    uint8_t     m_textSize = 1;

    AFRadioButtonGroup* m_group = nullptr;
    AFRadioCallback m_onSelectCallback = nullptr;
};