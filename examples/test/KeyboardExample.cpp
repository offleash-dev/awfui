
#include "stdio.h"
#include "string.h"


#include "AFLabel.h"
#include "AFScreen.h"
#include "AFWorld.h"
#include "AFSlider.h"
#include "AFButton.h"
#include "AFKeyboard.h"



void createTextInputPanel(AFScreen& screen, AFLabel* keyOutputLabel);



char textBuffer[64] = {0}; // Buffer to hold the current text for display
AFButton* submitButton;
AFKeyboard* keyboard = nullptr;
AFLabel* keyboardOutputLabel = nullptr;

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



void onSubmitButtonRelease(AFButton& sender) {
      textBuffer[0] = 0; // Clear the text buffer
      if (keyboardOutputLabel) {
            keyboardOutputLabel->setText("");

      }
      keyboard->setVisible(false);
      submitButton->setVisible(true);
}




void createTextInputPanel(AFScreen& screen, AFLabel* keyOutputLabel) {
    keyboardOutputLabel = keyOutputLabel;

    if (keyboard == nullptr) {
        int16_t submitButtonX = keyOutputLabel->getX() + keyOutputLabel->getWidth() + 5;
        int16_t submitButtonY = keyOutputLabel->getY();

        submitButton = new AFButton(submitButtonX, submitButtonY, 50, 30, "Submit", MAKE_ID_FROM_STR("S1Sb"));
        submitButton->setOnClickCallback(onSubmitButtonRelease);
        screen.addWidget(submitButton, true);

        keyboard = new AFKeyboard(0, screen.getDisplay().height() - 70, screen.getDisplay().width(), 70, MAKE_ID_FROM_STR("SKbd"));
        keyboard->setOnCharacterCallback(onKeyboardCharacterCallback);
        keyboard->setOnBackspaceCallback(onKeyboardBackspaceCallBack);
        screen.addPanel(keyboard, true);
    }

    submitButton->setVisible(true);
    keyboard->setVisible(true);
}
