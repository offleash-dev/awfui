#pragma once

//// AFKeyboard.h
//// Part of the AWFUI library
////
//// Compound widget for character input via a button interface.
//// Provides modes for lowercase, uppercase, numeric, and punctuation input.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFPanel.h"
#include "AFButton.h"
#include "AFImageButton.h"
#include "AFEvent.h"



// Character input modes
enum class AFCharacterMode {
    kLowerCaseMode,     // 'a' indicator, a-z input
    kUpperCaseMode,     // 'A' indicator, A-Z input (temporary)
    kUpperCaseLockedMode, // 'A' indicator, A-Z input (locked)
    kNumericMode,       // '1' indicator, 0-9 and symbols
    kPunctuationMode    // '#' indicator, punctuation symbols
};  



// Forward declaration
class AFKeyboard;
class AFButton;


// Callback type for character input
using AFKeyboardCallback = void (*)(AFKeyboard& sender, char character);



class AFKeyboard : public AFPanel {
public:
    AFKeyboard(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);
    
    // Friend functions for callback wrappers
    friend void onKeyPressedWrapper(AFButton& sender);
    friend void onShiftPressedWrapper(AFButton& sender);
    friend void onSpacePressedWrapper(AFButton& sender);
    friend void onBackspacePressedWrapper(AFButton& sender);
    
    // Mode management
    void setCharacterMode(AFCharacterMode mode);
    AFCharacterMode getCharacterMode() const { return m_characterMode; }
    
    // Character input callback
    void setOnCharacterCallback(AFKeyboardCallback cb) { m_onCharacterCallback = cb; }
    
    // Backspace handling
    void setOnBackspaceCallback(AFKeyboardCallback cb) { m_onBackspaceCallback = cb; }
    
    // Optimized event handling for grid layout
    void handleEvent(const AFEvent& e) override;

protected:
    void createKeyboardLayout();
    void updateKeyLabels();
    void updateShiftButton();
    char getCharacterForButton(int buttonIndex) const;
    void onKeyPressed(AFButton& sender);
    void onShiftPressed(AFButton& sender);
    void onSpacePressed(AFButton& sender);
    void onBackspacePressed(AFButton& sender);

    
private:
    // Child widgets
    AFButton* m_keyButtons[26];  // 26 letter buttons
    AFButton* m_shiftButton;
    AFButton* m_spaceButton;
    AFButton* m_backspaceButton;
    AFButton* m_modeButton;     // For numbers/symbols mode
    
    // State
    AFCharacterMode m_characterMode;
    char m_currentCharacter;
    
    // Callbacks
    AFKeyboardCallback m_onCharacterCallback;
    AFKeyboardCallback m_onBackspaceCallback;
    
    // Constants
    static constexpr int kKeyWidth = 20;
    static constexpr int kKeyHeight = 20;
    static constexpr int kKeyMargin = 3;
    static constexpr int kShiftWidth = 20;  // 1x key width
    static constexpr int kSpaceWidth = 40;   // 2x key width
    static constexpr int kControlHeight = 96;
};
