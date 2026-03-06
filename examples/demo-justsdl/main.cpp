// main.cpp — AWFUI demo using pure-SDL backend (no Adafruit_GFX)

// Tell SDL we handle our own main() entry point
#define SDL_MAIN_HANDLED

#include "AFDisplaySDL.h"
#include "AFTouchSDL.h"



#include <stdio.h>



#include "AFButton.h"
#include "AFLabel.h"
#include "AFModalDialog.h"
#include "AFScreen.h"
#include "AFWorld.h"



// ------------------------------------------------------------
// DemoScreen — subclass AFScreen to handle external events
// ------------------------------------------------------------
class DemoScreen : public AFScreen {
public:
      DemoScreen(AFDisplayInterface& display, bool useCanvas)
            : AFScreen(display, makeID("Main"), useCanvas) {}

      void setCounterLabel(AFLabel* lbl) { m_counterLabel = lbl; }

protected:
      void onExternalEvent(const AFEvent& e) override {
            if (e.type == AFEventType::kButton && e.buttonId == 1) {
                  m_count++;
                  snprintf(m_buf, sizeof(m_buf), "Button presses: %d", m_count);
                  m_counterLabel->setText(m_buf);
            }
      }

private:
      AFLabel* m_counterLabel = nullptr;
      int      m_count = 0;
      char     m_buf[32] = "Button presses: 0";
};



// Pure-SDL display — no Adafruit_GFX in the chain
AFDisplaySDL display(240, 320, 2);
AFTouchSDL   touch(2);

AFWorld*       world;
DemoScreen*    mainScreen;
AFEventQueue   eventQueue;
AFModalDialog* dialog;


void showMainScreenDialog(AFWidget& sender) {
      dialog->show(*mainScreen);
}


void dismissMainScreenDialog(AFWidget& sender) {
      dialog->show(*mainScreen);
}


void setup() {
      SDL_SetMainReady();

      display.begin();
      display.setRotation(1);

      touch.begin();

      AFWorld::init(display, &touch, &eventQueue);
      world = AFWorld::instance();

      // Create and register our custom screen
      mainScreen = new DemoScreen(display, true);
      world->registerScreen(mainScreen);
      world->setActiveScreen(mainScreen);

      // ------------------------------------------------------------
      // Counter label — updated by onExternalEvent
      // ------------------------------------------------------------
      AFLabel* counterLbl = new AFLabel(60, 10, 200, 20, "Button presses: 0", makeID("Cntr"));
      mainScreen->addWidget(counterLbl);
      mainScreen->setCounterLabel(counterLbl);

      // ------------------------------------------------------------
      // Main screen button
      // ------------------------------------------------------------
      AFButton* openBtn = new AFButton(120, 40, 160, 50, makeID("Open"), "Open Dialog");
      openBtn->setOnClickCallback(showMainScreenDialog);
      mainScreen->addWidget(openBtn);

      // ------------------------------------------------------------
      // Modal dialog
      // ------------------------------------------------------------
      dialog = new AFModalDialog(20, 40, 200, 140, makeID("HDlg"));

      AFLabel* lbl = new AFLabel(30, 60, "Hello from AWFUI!", makeID("Helo"));
      dialog->addWidget(lbl);

      AFButton* okBtn = new AFButton(50, 100, 100, 40, makeID("OKBt"), "OK");
      okBtn->setOnClickCallback(dismissMainScreenDialog);
      dialog->addWidget(okBtn);
}

const int kFps                    = 60;
const int kButtonIntervalFrames   = 5 * kFps;       // fire every 5 seconds
int       eventCountdown          = kButtonIntervalFrames;



void loop() {
      // Process SDL events and feed touch input
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                  exit(0);
            }
            touch.handleEvent(e);
      }

      // Simulate a hardware button press every 5 seconds
      if (--eventCountdown <= 0) {
            AFEvent buttonEvent{};
            buttonEvent.type     = AFEventType::kButton;
            buttonEvent.buttonId = 1;
            eventQueue.postEvent(buttonEvent);
            eventCountdown = kButtonIntervalFrames;
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