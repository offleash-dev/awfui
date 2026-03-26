// main.cpp — AWFUI keypad demo using pure-SDL backend (no Adafruit_GFX)

// Tell SDL we handle our own main() entry point
#define SDL_MAIN_HANDLED

#include "AFDisplaySDL.h"
#include "AFTouchSDL.h"

#include <stdio.h>
#include <string.h>

#include "AFButton.h"
#include "AFLabel.h"
#include "AFLayoutHelpers.h"
#include "AFScreen.h"
#include "AFWorld.h"



// ------------------------------------------------------------
// Maximum digits the user can enter
// ------------------------------------------------------------
static const int kMaxDigits = 16;



// ------------------------------------------------------------
// KeypadScreen — phone keypad with digit entry
// ------------------------------------------------------------
class KeypadScreen : public AFScreen {
public:
      KeypadScreen(AFDisplayInterface& display, bool useCanvas)
            : AFScreen(display, makeID("KPad"), useCanvas) {
            m_numberBuf[0] = '\0';
      }

      void setNumberLabel(AFLabel* lbl) { m_numberLabel = lbl; }

      void appendDigit(AFWidget& sender) {
            int len = (int)strlen(m_numberBuf);
            char ch      = sender.getId() & 0xFF; // low byte of button ID is the char
            if (len < kMaxDigits) {
                  m_numberBuf[len]     = ch;
                  m_numberBuf[len + 1] = '\0';
                  m_numberLabel->setText(m_numberBuf);
            }
      }

      void backspace(AFWidget& sender) {
            int len = (int)strlen(m_numberBuf);
            if (len > 0) {
                  m_numberBuf[len - 1] = '\0';
                  m_numberLabel->setText(m_numberBuf);
            }
      }

      void dial(AFWidget& sender) {
            // Placeholder — just clear the display
            m_numberBuf[0] = '\0';
            m_numberLabel->setText(m_numberBuf);
      }

private:
      AFLabel* m_numberLabel = nullptr;
      char     m_numberBuf[kMaxDigits + 1] = {};
};




// Pure-SDL display — 240x320 portrait (rotation 0)
AFDisplaySDL   display(240, 320, 2);
AFTouchSDL     touch(2);

AFWorld*        world;
KeypadScreen*   keypadScreen;
AFEventQueue    eventQueue;


void dialCallback(AFButton& sender) {
      // Placeholder — just clear the display
      keypadScreen->dial(sender);
}

void backspaceCallback(AFButton& sender) {
      keypadScreen->backspace(sender);
}

void appendDigitCallback(AFButton& sender) {
      keypadScreen->appendDigit(sender);
}




// ------------------------------------------------------------
// Layout constants
// ------------------------------------------------------------
static const int16_t kScreenW       = 240;
static const int16_t kLabelY        = 15;
static const int16_t kLabelH        = 30;
static const int16_t kLabelW        = 200;
static const int16_t kBtnW          = 40;
static const int16_t kBtnH          = 40;
static const int16_t kGapX          = 14;
static const int16_t kGapY          = 14;
static const int16_t kGridOriginX   = AFLayoutHelpers::gridCenterX(kScreenW, 3, kBtnW, kGapX);
static const int16_t kGridOriginY   = (int16_t)(kLabelY + kLabelH + kGapY);
static const int16_t kBottomRowH    = 30;



void setup() {
      SDL_SetMainReady();

      display.begin();
      display.setRotation(0);

      touch.begin();

      AFWorld::init(display, &touch, &eventQueue);
      world = AFWorld::instance();

      // Create and register keypad screen
      keypadScreen = new KeypadScreen(display, true);
      world->registerScreen(keypadScreen);
      world->setActiveScreen(keypadScreen);

      // ------------------------------------------------------------
      // Entered-number label — centered
      // ------------------------------------------------------------
      int16_t lblX = AFLayoutHelpers::centerX(0, kScreenW, kLabelW);
      AFLabel* numberLbl = new AFLabel(lblX, kLabelY, kLabelW, kLabelH, "", makeID("Numb"));
      keypadScreen->addWidget(numberLbl);
      keypadScreen->setNumberLabel(numberLbl);

      // ------------------------------------------------------------
      // Keypad grid:  4 rows x 3 cols
      //   [1] [2] [3]
      //   [4] [5] [6]
      //   [7] [8] [9]
      //   [*] [0] [#]
      // ------------------------------------------------------------
      struct KeyDef { const char* label; uint32_t id; };
      static const KeyDef keys[4][3] = {
            { {"1",makeID("Key1")}, {"2",makeID("Key2")}, {"3",makeID("Key3")} },
            { {"4",makeID("Key4")}, {"5",makeID("Key5")}, {"6",makeID("Key6")} },
            { {"7",makeID("Key7")}, {"8",makeID("Key8")}, {"9",makeID("Key9")} },
            { {"*",makeID("KeyS")}, {"0",makeID("Key0")}, {"#",makeID("KeyH")} },
      };


      for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 3; col++) {
                  AFRect r = AFLayoutHelpers::gridCell(kGridOriginX, kGridOriginY,
                                                col, row, kBtnW, kBtnH, kGapX, kGapY);
                  const KeyDef& k = keys[row][col];
                  AFButton* btn = new AFButton(r.x, r.y, r.w, r.h,
                                               k.id, k.label);
                  btn->setOnClickCallback(appendDigitCallback);
                  keypadScreen->addWidget(btn);
            }
      }

      // ------------------------------------------------------------
      // Bottom row:  [Call]  [<-]
      // ------------------------------------------------------------
      // Place below the last grid row
      AFRect lastCell = AFLayoutHelpers::gridCell(kGridOriginX, kGridOriginY,
                                           0, 3, kBtnW, kBtnH, kGapX, kGapY);
      int16_t bottomY = (int16_t)(lastCell.y + lastCell.h + kGapY);

      AFButton* callBtn = new AFButton(30, bottomY, 100, kBottomRowH, makeID("Call"), "Call");
      callBtn->setOnClickCallback(dialCallback);
      keypadScreen->addWidget(callBtn);

      AFButton* bkspBtn = new AFButton(
            AFLayoutHelpers::rightOf(*callBtn, kGapX), bottomY,
            30, kBottomRowH, makeID("BkSp"), "<");
      bkspBtn->setOnClickCallback(backspaceCallback);
      keypadScreen->addWidget(bkspBtn);
}



void loop() {
      // Process SDL events and feed touch input
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                  exit(0);
            }
            touch.handleEvent(e);
      }

      world->loop();

      // Update the SDL window
      display.present();
      SDL_Delay(16);  // ~60 FPS
}



// Entry point for Windows/Linux/macOS builds
int main() {
      setup();
      while (true) {
            loop();
      }
      return 0;
}
