#pragma once

//// AFSliderKeyboard.h
//// Part of the AWFUI library
////
//// Compound widget for character input via slider interface.
//// Provides modes for lowercase, uppercase, numeric, and punctuation input.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFPanel.h"
#include "AFButton.h"
#include "AFImageButton.h"
#include "AFSlider.h"
#include "AFLabel.h"
#include "AFEvent.h"

// Character input modes
enum class AFCharacterMode {
    kLowerCaseMode,     // 'a' indicator, a-z input
    kUpperCaseMode,     // 'A' indicator, A-Z input (temporary)
    kUpperCaseLockedMode, // 'A' indicator, A-Z input (locked)
    kNumericMode,       // '2' indicator, 0-9 and symbols
    kPunctuationMode    // '#' indicator, punctuation symbols
};


// Forward declaration
class AFSliderKeyboard;

// Callback type for character input
using AFSliderKeyboardCallback = void (*)(AFSliderKeyboard& sender, char character);



class AFSliderKeyboard : public AFPanel {
public:
    AFSliderKeyboard(int16_t x, int16_t y, int16_t w, int16_t h, ID_TYPE id = 0);
    
    // Friend functions for callback wrappers
    friend void onCurrentModePressedWrapper(AFButton& sender);
    friend void onNextModePressedWrapper(AFButton& sender);
    friend void onSKBackspacePressedWrapper(AFButton& sender);
    friend void onSliderReleasedWrapper(AFSlider& sender, int value);
    friend void onSliderMovedWrapper(AFSlider& sender, int value);
    
    // Mode management
    void setCharacterMode(AFCharacterMode mode);
    AFCharacterMode getCharacterMode() const { return m_characterMode; }
    
    // Character input callback
    void setOnCharacterCallback(AFSliderKeyboardCallback cb) { m_onSKCharacterCallback = cb; }
    
    // Backspace handling
    void setOnBackspaceCallback(AFSliderKeyboardCallback cb) { m_onSKBackspaceCallback = cb; }
    
    // Get current character (based on slider position)
    char getCurrentCharacter() const;
    
    // Event handling
    void handleEvent(const AFEvent& e) override;


protected:
    void updateModeDisplay();
    void updateCharacterDisplay();
    void updateSliderRange();
    char getCharacterForMode(AFCharacterMode mode, int index) const;
    int getCharacterCountForMode(AFCharacterMode mode) const;
    void onSliderReleased(AFSlider& sender, uint16_t value);
    void onSliderMoved(AFSlider& sender, uint16_t value);
    void onSKBackspacePressed(AFButton& sender);
    void onNextModePressed(AFButton& sender);
    void onCurrentModePressed(AFButton& sender);
    
    
private:
    // Child widgets
    AFButton* m_currentModeButton;
    AFButton* m_nextModeButton;
    AFSlider* m_characterSlider;
    AFImageButton* m_backButton;
    AFLabel* m_outputCharacterLabel;
    
    // State
    AFCharacterMode m_characterMode;
    char m_currentCharacter;
    
    // Callbacks
    AFSliderKeyboardCallback m_onSKCharacterCallback;
    AFSliderKeyboardCallback m_onSKBackspaceCallback;
    
    // Constants
    static constexpr int kModeButtonWidth = 20;      // 1 character width
    static constexpr int kNextModeButtonWidth = 60;  // 3 characters width
    static constexpr int kBackButtonWidth = 35;
    static constexpr int kBackButtonHeight = 24;
    static constexpr int kCharacterLabelWidth = 20;  // 1 character width
    static constexpr int kControlHeight = 30;
    static constexpr int kSliderHeight = 30;
    static constexpr int kMargin = 5;
};
