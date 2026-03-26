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

// Forward declarations
class AFRadioButton;
class AFRadioButtonGroup;

using AFRadioCallback = void (*)(AFRadioButton& sender, uint32_t id);


class AFRadioButton : public AFWidget {
public:
    AFRadioButton() = default;  // Default constructor for stack objects
    AFRadioButton(int16_t x,
                  int16_t y,
                  int16_t radius = 0,
                  uint32_t id = 0,
                  const char* label = nullptr);

    // Initialize method for stack objects
    void init(int16_t x, int16_t y, int16_t radius = 0, uint32_t id = 0, const char* label = nullptr);

    void setLabel(const char* text);
    const char* getLabel() const { return m_label; }

    void setSelected(bool sel);
    bool isSelected() const { return m_selected; }

    void setOrdinal(int16_t ordinal) { m_Ordinal = ordinal; }
    int16_t getOrdinal() const { return m_Ordinal; }

    void setTextSize(uint8_t size) { 
        m_textSize = size;
        markDirty(); 
    }
    

    uint8_t getTextSize() const { return m_textSize; }

    void setColors(uint16_t circleColor, uint16_t dotColor, uint16_t labelColor);


    void setOnSelectCallback(AFRadioCallback cb) { 
        m_onSelectCallback = cb; 
    }

    
    void draw(AFDisplayInterface& displayInterface) override;
    void onClick(const AFEvent& e) override;


private:
    friend class AFRadioButtonGroup;

    const char* m_label;
    bool        m_selected = false;

    uint16_t    m_circleColor;
    uint16_t    m_dotColor;
    uint16_t    m_labelColor;

    uint16_t    m_radius;
    uint8_t     m_textSize = 1;

    int16_t     m_Ordinal; 

    AFRadioButtonGroup* m_group = nullptr;
    AFRadioCallback m_onSelectCallback = nullptr;
};