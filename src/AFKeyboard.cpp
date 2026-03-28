//// AFKeyboard.cpp
//// Implementation of the AWFUI button keyboard widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <string>
#include <stdio.h>

#include "AFKeyboard.h"
#include "AFWorld.h"
#include "AFTheme.h"



// Character sets for each mode (row-based layout like original)
static const char* kLowerCaseCharsTopRow = "qwertyuiop";
static const char* kLowerCaseCharsMainRow = "asdfghjkl";
static const char* kLowerCaseCharsBottomRow = "zxcvbnm";

static const char* kUpperCaseCharsTopRow = "QWERTYUIOP";
static const char* kUpperCaseCharsMainRow = "ASDFGHJKL";
static const char* kUpperCaseCharsBottomRow = "ZXCVBNM";

static const char* kExtraChars1TopRow = "1234567890";
static const char* kExtraChars1MainRow = ".!?_@#$,&";
static const char* kExtraChars1BottomRow = "-+*/=()";

static const char* kExtraChars2TopRow = "1234567890";
static const char* kExtraChars2MainRow = "/~;:'\"%^|";
static const char* kExtraChars2BottomRow = "[]{}\\<>";

char keyLabels[26][2] = {0};   // Pre-allocated labels for buttons (1 char + null terminator)
char keyIds[26][5] = {0};   // Pre-allocated IDs for buttons (1 char + null terminator)

 

static const char* theButtonKeys[12]= {  // Indexed by AFCharacterMode, then row  
    kLowerCaseCharsTopRow, kLowerCaseCharsMainRow, kLowerCaseCharsBottomRow,
    kUpperCaseCharsTopRow, kUpperCaseCharsMainRow, kUpperCaseCharsBottomRow,
    kExtraChars1TopRow,   kExtraChars1MainRow,   kExtraChars1BottomRow,
    kExtraChars2TopRow,   kExtraChars2MainRow,   kExtraChars2BottomRow
};



// Callback wrappers
void onKeyPressedWrapper(AFButton& sender) {
    AFKeyboard* keyboard = static_cast<AFKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onKeyPressed(sender);
    }
}



void onShiftPressedWrapper(AFButton& sender) {
    AFKeyboard* keyboard = static_cast<AFKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onShiftPressed(sender);
    }
}



void onSpacePressedWrapper(AFButton& sender) {
    AFKeyboard* keyboard = static_cast<AFKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onSpacePressed(sender);
    }
}



void onBackspacePressedWrapper(AFButton& sender) {
    AFKeyboard* keyboard = static_cast<AFKeyboard*>(sender.getParent());
    if (keyboard) {
        keyboard->onBackspacePressed(sender);
    }
}



// Constructor
AFKeyboard::AFKeyboard(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id)
    : AFPanel(x, y, w, h, id)
    , m_characterMode(AFCharacterMode::kLowerCaseMode)
    , m_currentCharacter('a')
    , m_onCharacterCallback(nullptr)
    , m_onBackspaceCallback(nullptr)
{
    // Initialize button pointers
    for (int i = 0; i < 26; i++) {
        m_keyButtons[i] = nullptr;
    }
    m_shiftButton = nullptr;
    m_spaceButton = nullptr;
    m_backspaceButton = nullptr;
    m_modeButton = nullptr;

    // Create keyboard layout
    createKeyboardLayout();
    
    // Initialize display
    updateKeyLabels();
    updateShiftButton();
}



void AFKeyboard::createKeyboardLayout() {
    int16_t startX = kKeyMargin+2;
    int16_t startY = kKeyMargin;

    int keyIndex = 0;

    // Row 0: q w e r t y u i o p (10 keys)
    for (int col = 0; col < 10; col++) {
            int buttonIndex = 0 * 10 + col; // Row 0, column col
            keyIds[keyIndex][0] = 'K';
            snprintf(keyIds[keyIndex] + 1, sizeof(keyIds[keyIndex]) - 1, "%02d", buttonIndex);
            keyIds[keyIndex][3] = '\0';

            AFButton* button = new AFButton(
                startX + col * (kKeyWidth + kKeyMargin),
                startY,
                kKeyWidth, kKeyHeight,
                "",
                MAKE_ID_FROM_STR(keyIds[keyIndex])
            );

            button->setOnClickCallback(onKeyPressedWrapper);
            addWidget(button, true);
            
            if (buttonIndex < 26) {
                m_keyButtons[buttonIndex] = button;
            }

            keyIndex++;
    }
    
    // Row 1: a s d f g h j k l + backspace
    int16_t row1Y = startY + kKeyHeight + kKeyMargin;
    for (int col = 0; col < 10; col++) {
        if (col < 9) {
            int buttonIndex = 1 * 10 + col; // Row 1, column col
            keyIds[keyIndex][0] = 'K';
            snprintf(keyIds[keyIndex] + 1, sizeof(keyIds[keyIndex]) - 1, "%02d", buttonIndex);
            keyIds[keyIndex][3] = '\0';
            
            AFButton* button = new AFButton(
                startX + col * (kKeyWidth + kKeyMargin),
                row1Y,
                kKeyWidth, kKeyHeight,
                "",
                MAKE_ID_FROM_STR(keyIds[keyIndex])
            );
            button->setOnClickCallback(onKeyPressedWrapper);
            addWidget(button, true);
            
            if (buttonIndex < 26) {
                m_keyButtons[buttonIndex] = button;
            }
            
            keyIndex++;  // Only increment when we actually use keyIds
        } else if (col == 9) {
            // Backspace button
            m_backspaceButton = new AFButton(
                startX + 9 * (kKeyWidth + kKeyMargin),
                row1Y,
                kKeyWidth, kKeyHeight, // Make it wider
                "",
                MAKE_ID_FROM_STR("BACK")
            );
            m_backspaceButton->setLabel("<-");
            m_backspaceButton->setOnClickCallback(onBackspacePressedWrapper);
            addWidget(m_backspaceButton, true);
        }
    }
    
    // Row 2: shift + z x c v b n m + space
    int16_t row2Y = row1Y + kKeyHeight + kKeyMargin;
    
    // Shift button (1x width)
    m_shiftButton = new AFButton(
        startX,
        row2Y,
        kShiftWidth, kKeyHeight,
        "",
        MAKE_ID_FROM_STR("SHFT")
    );
    m_shiftButton->setLabel("^^");
    m_shiftButton->setOnClickCallback(onShiftPressedWrapper);
    addWidget(m_shiftButton, true);
    
    // z x c v b n m (7 keys, offset by shift button)
    for (int col = 0; col < 7; col++) {
            int buttonIndex = 19 + col; // Row 2, column col (10 from row 0 + 9 from row 1 = 19)
            keyIds[keyIndex][0] = 'K';
            snprintf(keyIds[keyIndex] + 1, sizeof(keyIds[keyIndex]) - 1, "%02d", buttonIndex);
            keyIds[keyIndex][3] = '\0';

            AFButton* button = new AFButton(
                startX + kShiftWidth + kKeyMargin + (col) * (kKeyWidth + kKeyMargin),
                row2Y,
                kKeyWidth, kKeyHeight,
                "",
                MAKE_ID_FROM_STR(keyIds[keyIndex])
            );
            button->setOnClickCallback(onKeyPressedWrapper);
            addWidget(button, true);
            
            if (buttonIndex < 26) {
                m_keyButtons[buttonIndex] = button;
            }

            keyIndex++;
    }
    
    // Space button (2x width)
    int spaceX = startX + kShiftWidth + kKeyMargin + 7 * (kKeyWidth + kKeyMargin) + kKeyMargin;
    m_spaceButton = new AFButton(
        spaceX,
        row2Y,
        kSpaceWidth, kKeyHeight,
        "",
        MAKE_ID_FROM_STR("SPCE")
    );
    m_spaceButton->setLabel("___");
    m_spaceButton->setOnClickCallback(onSpacePressedWrapper);

    addWidget(m_spaceButton, true);
}



void AFKeyboard::setCharacterMode(AFCharacterMode mode) {
    m_characterMode = mode;
    updateKeyLabels();
    updateShiftButton();
}



void AFKeyboard::updateKeyLabels() {
    // Update each button based on its row and position
    for (int key = 0; key < 26; key++) {
            if (!m_keyButtons[key]) 
                continue;

            char ch = getCharacterForButton(key);
            keyLabels[key][0] = ch;
            keyLabels[key][1] = '\0';
            
            m_keyButtons[key]->setLabel(keyLabels[key]);
    }
    
    markDirty();
}



void AFKeyboard::updateShiftButton() {
    const AFTheme& theme = AFWorld::instance()->getTheme();
    switch (m_characterMode) {
        case AFCharacterMode::kLowerCaseMode:
            m_shiftButton->setLabel("^^");
            m_shiftButton->setShowBorder(false);
            m_shiftButton->setEnabled(true);
            m_shiftButton->setColors(theme.widgetBgColor, theme.widgetFgColor, theme.widgetBorderColor);
            break;
            
        case AFCharacterMode::kUpperCaseMode:
            m_shiftButton->setLabel("^^");
            m_shiftButton->setShowBorder(true);
            m_shiftButton->setEnabled(true);
            // Highlight to show temporary shift
            m_shiftButton->setColors(theme.widgetAccentColor, theme.widgetFgColor, theme.widgetBorderColor);
            break;
            
        case AFCharacterMode::kUpperCaseLockedMode:
            m_shiftButton->setLabel("1");
            m_shiftButton->setShowBorder(true);
            m_shiftButton->setEnabled(true);
            // Inverted colors for caps lock
            m_shiftButton->setColors(theme.widgetFgColor, theme.widgetBgColor, theme.widgetBorderColor);
            break;
            
        case AFCharacterMode::kNumericMode:
            m_shiftButton->setLabel("#");
            m_shiftButton->setShowBorder(false);
            m_shiftButton->setEnabled(true);
            m_shiftButton->setColors(theme.widgetBgColor, theme.widgetFgColor, theme.widgetBorderColor);
            break;

        case AFCharacterMode::kPunctuationMode:
            m_shiftButton->setLabel("^^");
            m_shiftButton->setShowBorder(false);
            m_shiftButton->setEnabled(true);
            m_shiftButton->setColors(theme.widgetBgColor, theme.widgetFgColor, theme.widgetBorderColor);
            break;
    }
    
    markDirty();
}



char AFKeyboard::getCharacterForButton(int buttonIndex) const {
        char character = ' ';

        int row = 0;
        int col = 0;
        if (buttonIndex < 10) {
            row = 0;
            col = buttonIndex;
        } else if (buttonIndex < 19) {
            row = 1;
            col = buttonIndex - 10;
        } else {
            row = 2;
            col = buttonIndex - 19;
        }

        //  the  character set for this mode and row is theButtonKeys
        switch (m_characterMode) {
            case AFCharacterMode::kLowerCaseMode:
                break;
                
            case AFCharacterMode::kUpperCaseMode:
            case AFCharacterMode::kUpperCaseLockedMode:
                row += 3;
                break;
                
            case AFCharacterMode::kNumericMode:
                row += 6;
                break;
                
            case AFCharacterMode::kPunctuationMode:
                row += 9;
                break;
        }
        
        character = (char)(theButtonKeys[row][col]);
    
        return character;
}



void AFKeyboard::onKeyPressed(AFButton& sender) {
    // Extract button index from button ID
    uint32_t id = sender.getId();
    // button IDs are in the format "KXX" where XX is the button index (e.g. K00, K01, ..., K29)
    if (getIdByte(id, 0) != 'K') 
        return; // Not a key button
    
    // button index is char 1 and 2, convert to integer
    int buttonIndex = (getIdByte(id, 1) - '0') * 10 + (getIdByte(id, 2) - '0'); // Convert "KXX" to integer index
    if (buttonIndex < 0 || buttonIndex >= 26) 
        return; // We have 26 buttons
    
    m_currentCharacter = getCharacterForButton(buttonIndex);
    
    // If in temporary uppercase mode, revert to lowercase after input
    if (m_characterMode == AFCharacterMode::kUpperCaseMode) {
        setCharacterMode(AFCharacterMode::kLowerCaseMode);
    }
    
    // Send character callback
    if (m_onCharacterCallback) {
        m_onCharacterCallback(*this, m_currentCharacter);
    }
    
//    markDirty();
}



void AFKeyboard::onShiftPressed(AFButton& sender) {
    switch (m_characterMode) {
        case AFCharacterMode::kLowerCaseMode:
            // First press: temporary shift
            setCharacterMode(AFCharacterMode::kUpperCaseMode);
            break;
            
        case AFCharacterMode::kUpperCaseMode:
            // Second press: lock caps
            setCharacterMode(AFCharacterMode::kUpperCaseLockedMode);
            break;
            
        case AFCharacterMode::kUpperCaseLockedMode:
            // Third press: back to lowercase
            setCharacterMode(AFCharacterMode::kNumericMode);
            break;
            
        case AFCharacterMode::kNumericMode:
            // Switch to punctuation mode
            setCharacterMode(AFCharacterMode::kPunctuationMode);
            break;
            
        case AFCharacterMode::kPunctuationMode: 
            // Switch back to lowercase
            setCharacterMode(AFCharacterMode::kLowerCaseMode);
            break;
    }
    
    markDirty();
}



void AFKeyboard::onSpacePressed(AFButton& sender) {
    if (m_onCharacterCallback) {
        m_onCharacterCallback(*this, ' ');
    }
}



// Optimized event handling using grid-based hit testing
void AFKeyboard::handleEvent(const AFEvent& e) {
    // Only handle touch events
    if (e.type != AFEventType::kTouchDown && 
        e.type != AFEventType::kTouchUp && 
        e.type != AFEventType::kTouchMove) {
        AFPanel::handleEvent(e);
        return;
    }
    
    // Move/Up go to captured widget (no hit-test)
    if (e.type == AFEventType::kTouchMove && hasCaputuredWidget()) {
            capturedWidgetMove(e);
            return;
    }
    if (e.type == AFEventType::kTouchUp && hasCaputuredWidget()) {
            capturedWidgetRelease(e);
            return;
    }

      // Convert to local coordinates
    int16_t localX = e.x - m_x;
    int16_t localY = e.y - m_y;
    
    // Quick bounds check
    if (localX < 0 || localY < 0 || localX >= m_width || localY >= m_height) {
        return;
    }
    
    // Grid-based hit testing for letter keys
    // Calculate which row/column was touched
    int16_t startX = kKeyMargin + 2;
    int16_t startY = kKeyMargin;
    
    AFWidget* foundButton = nullptr;
    // Check row 0 (10 keys: qwerty...)
    if (localY >= startY && localY < startY + kKeyHeight) {
        int col = (localX - startX) / (kKeyWidth + kKeyMargin);
        if (col >= 0 && col < 10) {
            int buttonIndex = col;
            if (m_keyButtons[buttonIndex]) {
                foundButton = m_keyButtons[buttonIndex];
            }
        }
    }
    
    // Check row 1 (9 keys + backspace)
    int16_t row1Y = startY + kKeyHeight + kKeyMargin;
    if (foundButton == nullptr && localY >= row1Y && localY < row1Y + kKeyHeight) {
        int col = (localX - startX) / (kKeyWidth + kKeyMargin);
        if (col >= 0 && col < 9) {
            int buttonIndex = 10 + col;
            if (m_keyButtons[buttonIndex]) {
                foundButton = m_keyButtons[buttonIndex];
            }
        } else if (col == 9 && m_backspaceButton) {
            foundButton = m_backspaceButton;
        }
    }
    
    // Check row 2 (shift + 7 keys + space)
    int16_t row2Y = row1Y + kKeyHeight + kKeyMargin;
    if (foundButton == nullptr && localY >= row2Y && localY < row2Y + kKeyHeight) {
        // Check shift button
        if (localX >= startX && localX < startX + kShiftWidth && m_shiftButton) {
            foundButton = m_shiftButton;
        }
        
        // Check letter keys (offset by shift button)
        int offsetX = localX - (startX + kShiftWidth + kKeyMargin);
        if (offsetX >= 0) {
            int col = offsetX / (kKeyWidth + kKeyMargin);
            if (col >= 0 && col < 7) {
                int buttonIndex = 19 + col;
                if (m_keyButtons[buttonIndex]) {
                    foundButton = m_keyButtons[buttonIndex];
                }
            }
            
            // Check space button
            int spaceX = startX + kShiftWidth + kKeyMargin + 7 * (kKeyWidth + kKeyMargin) + kKeyMargin;
            if (localX >= spaceX && localX < spaceX + kSpaceWidth && m_spaceButton) {
                foundButton = m_spaceButton;
            }
        }
    }
    
    if (foundButton) {
        captureWidget(foundButton, e);
        return;
    }

    // Fallback to default panel hit testing for any edge cases
    AFPanel::handleEvent(e);
}



void AFKeyboard::onBackspacePressed(AFButton& sender) {
    if (m_onBackspaceCallback) {
        m_onBackspaceCallback(*this, '\b'); // Use backspace character
    }
}
