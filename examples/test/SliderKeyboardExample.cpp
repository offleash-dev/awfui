#define USE_SLIDER_KEYBOARD_TEST 0
#define USE_BUTTON_KEYBOARD_TEST 1

#include "AFLabel.h"
#include "AFScreen.h"
#include "AFWorld.h"
#include "AFSlider.h"
#include "AFButton.h"

#if USE_SLIDER_KEYBOARD_TEST
#include "AFSliderKeyboard.h"
#endif
#if USE_BUTTON_KEYBOARD_TEST
#include "AFKeyboard.h"
#endif


void createTextInputPanel(AFScreen& screen, AFLabel* keyOutputLabel);


char textBuffer[64] = {0}; // Buffer to hold the current text for display
AFButton* submitButton;
AFLabel* keyboardOutputLabel = nullptr;

#if USE_SLIDER_KEYBOARD_TEST
AFSliderKeyboard* keyboard = nullptr;

void onKeyboardCharacterCallback(AFSliderKeyboard& sender, char character) {
    if (keyboardOutputLabel == nullptr) {
        return; 
    }

    // Append character to text label
    const char* current = keyboardOutputLabel->getText();
    if (strlen(current) == 0) {
        snprintf(textBuffer, sizeof(textBuffer)-1, "%c", character);
    } else {
        snprintf(textBuffer, sizeof(textBuffer)-1, "%s%c", current, character);
    }
    keyboardOutputLabel->setText(textBuffer);
}



void onKeyboardBackspaceCallBack(AFSliderKeyboard& sender, char character) {
    if (keyboardOutputLabel == nullptr) {
        return; 
    }

    int currentLength = strlen(textBuffer);
    if (currentLength > 0) {
        textBuffer[currentLength-1] = 0;
        keyboardOutputLabel->setText(textBuffer);
    }
}

#endif

#if USE_BUTTON_KEYBOARD_TEST
AFKeyboard* buttonKeyboardPanel = nullptr;

void onKeyboardCharacterCallback(AFKeyboard& sender, char character) {
    if (keyboardOutputLabel == nullptr) {
        return; 
    }

    // Append character to text label
    const char* current = keyboardOutputLabel->getText();
    if (strlen(current) == 0) {
        snprintf(textBuffer, sizeof(textBuffer)-1, "%c", character);
    } else {
        snprintf(textBuffer, sizeof(textBuffer)-1, "%s%c", current, character);
    }
    keyboardOutputLabel->setText(textBuffer);
}



void onKeyboardBackspaceCallBack(AFKeyboard& sender, char character) {
    if (keyboardOutputLabel == nullptr) {
        return; 
    }

    int currentLength = strlen(textBuffer);
    if (currentLength > 0) {
        textBuffer[currentLength-1] = 0;
        keyboardOutputLabel->setText(textBuffer);
    }
}

#endif




void onSubmitButtonRelease(AFButton& sender) {
      textBuffer[0] = 0; // Clear the text buffer
      if (keyboardOutputLabel) {
            keyboardOutputLabel->setText("");
      }
#if USE_SLIDER_KEYBOARD_TEST
      keyboard->setVisible(false);
#endif
#if USE_BUTTON_KEYBOARD_TEST
      buttonKeyboardPanel->setVisible(false);
#endif
      submitButton->setVisible(true);
}




void createTextInputPanel(AFScreen& screen, AFLabel* keyOutputLabel) {
    keyboardOutputLabel = keyOutputLabel;

#if USE_SLIDER_KEYBOARD_TEST
    if (keyboard == nullptr) {
#elif USE_BUTTON_KEYBOARD_TEST
    if (buttonKeyboardPanel == nullptr) {
#endif
        int16_t submitButtonX = keyOutputLabel->getX() + keyOutputLabel->getWidth() + 5;
        int16_t submitButtonY = keyOutputLabel->getY();

        submitButton = new AFButton(submitButtonX, submitButtonY, 50, 30, makeID("S1Sb"), "Submit");
        submitButton->setOnClickCallback(onSubmitButtonRelease);
        screen.addWidget(submitButton, true);

#if USE_SLIDER_KEYBOARD_TEST
        keyboard = new AFSliderKeyboard(0, screen.getDisplay().height() - 96, screen.getDisplay().width(), 96, makeID("SKbd"));
        keyboard->setOnCharacterCallback(onKeyboardCharacterCallback);
#endif

#if USE_BUTTON_KEYBOARD_TEST
        buttonKeyboardPanel = new AFKeyboard(0, screen.getDisplay().height() - 96, screen.getDisplay().width(), 96, makeID("BKbd"));
        buttonKeyboardPanel->setOnCharacterCallback(onKeyboardCharacterCallback);
        buttonKeyboardPanel->setOnBackspaceCallback(onKeyboardBackspaceCallBack);
        screen.addPanel(buttonKeyboardPanel, true);
#endif  
    }

    submitButton->setVisible(true);
#if USE_SLIDER_KEYBOARD_TEST
    keyboard->setVisible(true);
#endif
#if USE_BUTTON_KEYBOARD_TEST
    buttonKeyboardPanel->setVisible(true);
#endif
}
