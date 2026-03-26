//// AFSliderKeyboard.cpp
//// Implementation of the AWFUI slider keyboard widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFSliderKeyboard.h"
#include "AFWorld.h"



// Backspace image data (35x24px, 1-bit format)
// Converted from epd_bitmap_backspace to AWFUI format
static const uint8_t backspace_image_data[] = {
    // AFImageHeader: width=35, height=24, format=1-bit, reserved=0
    0x23, 0x00,  // width  = 35
    0x18, 0x00,  // height = 24
    0x01,        // format = 1-bit
    0x00,        // reserved
    
    // Pixel data (35x24 = 840 bits = 105 bytes)
    0xff, 0xe0, 0x00, 0x00, 0x00, 
    0xff, 0xc0, 0x00, 0x00, 0x00, 
    0xff, 0x87, 0xff, 0xff, 0x80, 
    0xff, 0x0f, 0xff, 0xff, 0x80, 
    0xfe, 0x1f, 0xff, 0xff, 0x80, 
    0xfc, 0x3f, 0x9f, 0x9f, 0x80, 
    0xf8, 0x7f, 0x9f, 0x1f, 0x80, 
    0xf0, 0xff, 0x8f, 0x1f, 0x80, 
    0xe1, 0xff, 0xc6, 0x3f, 0x80, 
    0xc3, 0xff, 0xc4, 0x7f, 0x80, 
    0x87, 0xff, 0xe0, 0xff, 0x80, 
    0x0f, 0xff, 0xf0, 0xff, 0x80, 
    0x0f, 0xff, 0xf1, 0xff, 0x80, 
    0x87, 0xff, 0xe0, 0xff, 0x80, 
    0xc3, 0xff, 0xe0, 0x7f, 0x80, 
    0xe1, 0xff, 0xc6, 0x3f, 0x80, 
    0xf0, 0xff, 0x8e, 0x3f, 0x80, 
    0xf8, 0x7f, 0x8f, 0x1f, 0x80, 
    0xfc, 0x3f, 0x9f, 0x9f, 0x80, 
    0xfe, 0x1f, 0xff, 0xff, 0x80, 
    0xff, 0x0f, 0xff, 0xff, 0x80, 
    0xff, 0x87, 0xff, 0xff, 0x80, 
    0xff, 0xc0, 0x00, 0x00, 0x00, 
    0xff, 0xe0, 0x00, 0x00, 0x00 
};



static AFImage backspaceImage(backspace_image_data);



// Callback functions for better debugging
void onCurrentModePressedWrapper(AFButton& sender) {
    AFSliderKeyboard* keyboard = static_cast<AFSliderKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onCurrentModePressed(sender);
    }
}



void onNextModePressedWrapper(AFButton& sender) {
    AFSliderKeyboard* keyboard = static_cast<AFSliderKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onNextModePressed(sender);
    }
}



void onSKBackspacePressedWrapper(AFButton& sender) {
    AFSliderKeyboard* keyboard = static_cast<AFSliderKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onSKBackspacePressed(sender);
    }
}



void onSliderReleasedWrapper(AFSlider& sender, int value) {
    AFSliderKeyboard* keyboard = static_cast<AFSliderKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onSliderReleased(sender, value);
    }
}



void onSliderMovedWrapper(AFSlider& sender, int value) {
    static int lastMovedValue = -1;
    AFSliderKeyboard* keyboard = static_cast<AFSliderKeyboard*>(sender.getParent());
    if (keyboard && value != lastMovedValue) {
        keyboard->onSliderMoved(sender, value);
    }
    lastMovedValue = value;
}



// Character sets for each mode
static const char* kLowerCaseChars = " abcdefghijklmnopqrstuvwxyz ";
static const char* kUpperCaseChars = " ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
static const char* kNumericChars = ".0123456789+-*/=";
static const char* kPunctuationChars = "`~@#$%^&*()-_;:'\",.?!=+[]{}\\|/<>";

// Mode transition indicators
static const char kModeIndicators[] = {'a', 'A', 'A', '2', '#'};
static const char* kModeTransitions[] = {"A2#", "2#a", "2#a", "#aA", "aA2"};



// Constructor
//
AFSliderKeyboard::AFSliderKeyboard(int16_t x, int16_t y, int16_t w, int16_t h, ID_TYPE id)
    : AFPanel(x, y, w, h, id)
    , m_characterMode(AFCharacterMode::kLowerCaseMode)
    , m_currentCharacter('a')
    , m_onSKCharacterCallback(nullptr)
    , m_onSKBackspaceCallback(nullptr)
{
    // Create child widgets in order from left to right
    
    // Current mode button (1 char width) - use local coordinates
    m_currentModeButton = new AFButton(kMargin, h - kControlHeight - kMargin, 
                                       kModeButtonWidth, kControlHeight, 
                                       makeID("SKCM"));
    m_currentModeButton->setOnClickCallback(onCurrentModePressedWrapper);
    addWidget(m_currentModeButton, true);
    
    // Next mode button (3 chars width) - use local coordinates
    m_nextModeButton = new AFButton(kMargin + kModeButtonWidth + kMargin, h - kControlHeight - kMargin,
                                    kNextModeButtonWidth, kControlHeight,
                                    makeID("SKNM"));
    m_nextModeButton->setOnClickCallback(onNextModePressedWrapper);
    addWidget(m_nextModeButton, true);
    
    // Back button (image button with backspace icon) - use local coordinates
    m_backButton = new AFImageButton(w - kBackButtonWidth - kMargin, h - kControlHeight - kMargin,
                                     &backspaceImage, makeID("SKBK"));
    m_backButton->setOnClickCallback(onSKBackspacePressedWrapper);
    addWidget(m_backButton, true);
    
    // Output character label (1 char width) - use local coordinates
    m_outputCharacterLabel = new AFLabel(w - kBackButtonWidth - kMargin - kCharacterLabelWidth - kMargin,
                                       h - kControlHeight - kMargin, kCharacterLabelWidth, kControlHeight,
                                       "", makeID("SKOL"));
    m_outputCharacterLabel->setJustification(AFJustificationCenter);
    addWidget(m_outputCharacterLabel, true);
    
    // Character slider (fills remaining space, same Y as other controls) - use local coordinates
    int sliderX = kMargin + kModeButtonWidth + kMargin + kNextModeButtonWidth + kMargin;
    int sliderWidth = w - sliderX - kBackButtonWidth - kCharacterLabelWidth - kMargin * 4;
    m_characterSlider = new AFSlider(sliderX, h - kControlHeight - kMargin,
                                     sliderWidth, kSliderHeight, makeID("SKSL"));
    m_characterSlider->setOnReleaseCallback(onSliderReleasedWrapper);
    m_characterSlider->setOnMoveCallback(onSliderMovedWrapper);
    addWidget(m_characterSlider, true);
    
    // Initialize display
    updateModeDisplay();
    updateCharacterDisplay();
    updateSliderRange();
}



// Set character mode and update display
//
void AFSliderKeyboard::setCharacterMode(AFCharacterMode mode) {
    m_characterMode = mode;
    updateModeDisplay();
    updateSliderRange();
    updateCharacterDisplay();
}



// Get current character based on slider position
//
char AFSliderKeyboard::getCurrentCharacter() const {
    return getCharacterForMode(m_characterMode, m_characterSlider->value());
}



// Handle events
//
void AFSliderKeyboard::handleEvent(const AFEvent& e) {
    // Let panel handle child widget events first
    AFPanel::handleEvent(e);
}



// Update mode button displays
//
void AFSliderKeyboard::updateModeDisplay() {
    // Update current mode button
    static char indicator[2] = {0};
    indicator[0] = kModeIndicators[static_cast<int>(m_characterMode)];
    m_currentModeButton->setLabel(indicator);
    
    // Update next mode button
    int modeIndex = static_cast<int>(m_characterMode);
    m_nextModeButton->setLabel(kModeTransitions[modeIndex]);
    
    // Configure current mode button appearance based on mode
    switch (m_characterMode) {
        case AFCharacterMode::kLowerCaseMode:
        case AFCharacterMode::kNumericMode:
        case AFCharacterMode::kPunctuationMode:
            // Inactive appearance - no border, disabled
            m_currentModeButton->setShowBorder(false);
            m_currentModeButton->setEnabled(false);
            break;
            
        case AFCharacterMode::kUpperCaseMode:
            // Active button with normal appearance
            m_currentModeButton->setShowBorder(true);
            m_currentModeButton->setEnabled(true);
            // Use normal colors
            break;
            
        case AFCharacterMode::kUpperCaseLockedMode:
            // Active button with inverted appearance
            m_currentModeButton->setShowBorder(true);
            m_currentModeButton->setEnabled(true);
            // Use inverted colors (swap bg and fg)
            const AFTheme& theme = AFWorld::instance()->getTheme();
            m_currentModeButton->setColors(theme.widgetFgColor, theme.widgetBgColor, theme.widgetBorderColor);
            break;
    }
}



// Update character display label
//
void AFSliderKeyboard::updateCharacterDisplay() {
    char currentChar = getCurrentCharacter();
    static char charStr[2] = {'\0'};
    charStr[0] = currentChar;
    m_outputCharacterLabel->setText(charStr);
}



// Update slider range based on current mode
//
void AFSliderKeyboard::updateSliderRange() {
    int charCount = getCharacterCountForMode(m_characterMode);
    m_characterSlider->setRange(0, charCount - 1);
    if (m_characterSlider->value() >= charCount) {
        m_characterSlider->setValue(0);
    }
}



// Get character for mode and index
//
char AFSliderKeyboard::getCharacterForMode(AFCharacterMode mode, int index) const {
    const char* charSet = nullptr;
    
    switch (mode) {
        case AFCharacterMode::kLowerCaseMode:
            charSet = kLowerCaseChars;
            break;
        case AFCharacterMode::kUpperCaseMode:
        case AFCharacterMode::kUpperCaseLockedMode:
            charSet = kUpperCaseChars;
            break;
        case AFCharacterMode::kNumericMode:
            charSet = kNumericChars;
            break;
        case AFCharacterMode::kPunctuationMode:
            charSet = kPunctuationChars;
            break;
    }
    
    if (charSet && index >= 0 && index < (int)strlen(charSet)) {
        return charSet[index];
    }
    
    return ' '; // Fallback
}



// Get character count for mode
//
int AFSliderKeyboard::getCharacterCountForMode(AFCharacterMode mode) const {
    switch (mode) {
        case AFCharacterMode::kLowerCaseMode:
            return strlen(kLowerCaseChars);
        case AFCharacterMode::kUpperCaseMode:
        case AFCharacterMode::kUpperCaseLockedMode:
            return strlen(kUpperCaseChars);
        case AFCharacterMode::kNumericMode:
            return strlen(kNumericChars);
        case AFCharacterMode::kPunctuationMode:
            return strlen(kPunctuationChars);
    }
    return 0;
}



// Handle slider release (character input)
//
void AFSliderKeyboard::onSliderReleased(AFSlider& sender, uint16_t value) {
    m_currentCharacter = getCurrentCharacter();
    
    // If in uppercase mode, switch to lowercase after input
    if (m_characterMode == AFCharacterMode::kUpperCaseMode) {
        setCharacterMode(AFCharacterMode::kLowerCaseMode);
    }
    
    // Update display
    updateCharacterDisplay();
    
    // Send character callback
    if (m_onSKCharacterCallback) {
        m_onSKCharacterCallback(*this, m_currentCharacter);
    }
    
    markDirty();
}



void AFSliderKeyboard::onSliderMoved(AFSlider& sender, uint16_t value) {
    m_currentCharacter = getCurrentCharacter();
    
    // Update display
    updateCharacterDisplay();
        
    markDirty();
}



// Handle backspace
//
void AFSliderKeyboard::onSKBackspacePressed(AFButton& sender) {
    if (m_onSKBackspaceCallback) {
        m_onSKBackspaceCallback(*this, '\b'); // Use backspace character
    }
    markDirty();
}



// Handle next mode button
//
void AFSliderKeyboard::onNextModePressed(AFButton& sender) {
    // Cycle to next available mode
    int currentMode = static_cast<int>(m_characterMode);
    int nextMode = (currentMode + 1) % 5; // 5 total modes
    setCharacterMode(static_cast<AFCharacterMode>(nextMode));
    markDirty();
}



// Handle current mode button (only active in uppercase modes)
//
void AFSliderKeyboard::onCurrentModePressed(AFButton& sender) {
    switch (m_characterMode) {
        case AFCharacterMode::kUpperCaseMode:
            // Switch to locked mode
            setCharacterMode(AFCharacterMode::kUpperCaseLockedMode);
            break;
            
        case AFCharacterMode::kUpperCaseLockedMode:
            // Switch to lowercase mode
            setCharacterMode(AFCharacterMode::kLowerCaseMode);
            break;
            
        default:
            // Should not be called in other modes
            break;
    }
    markDirty();
}
