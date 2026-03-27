// main.cpp

// Tell SDL we handle our own main() entry point
#ifdef AWFUI_USE_SDL
#define SDL_MAIN_HANDLED
#endif


#ifdef AWFUI_USE_SDL
#include "AFDisplaySDL.h"
#include "AFTouchSDL.h"
#else
#include "AFDisplayAdafruitGFX.h"
#include <Adafruit_ILI9341.h>
#include "AFFt6206Touch.h"
#endif

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



#ifdef AWFUI_USE_SDL
// SDL desktop simulation - 320x240 display at 2x scale
AFDisplaySDL      display(240, 320);
AFTouchSDL   touch(2);
#else
// Example display - STM32L475 IoT01 board uses no-arg constructor
// (pins are configured via BSP/HAL)
Adafruit_ILI9341 tft;
AFFt6206Touch    touch;

AFDisplayAdafruitGFX display(tft);
#endif


AFWorld*       world;
DemoScreen*    mainScreen;
AFEventQueue   eventQueue;
AFModalDialog* dialog;

void handleShowButtonClick(AFButton& sender) {
      dialog->show(*mainScreen);
}

void handleDialogOkClick(AFButton& sender) {
      dialog->dismiss();
}




void setup() {
#ifdef AWFUI_USE_SDL
      SDL_SetMainReady();
#endif
      display.begin();

      display.setRotation(1);

      touch.begin();
#ifdef AWFUI_USE_SDL
      AFWorld::init(display, &touch, &eventQueue);
#else
      touch.setRotation(1);
      AFWorld::init(display, &touch, &eventQueue);
#endif
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
      openBtn->setOnClickCallback(handleShowButtonClick);
      mainScreen->addWidget(openBtn);

      // ------------------------------------------------------------
      // Modal dialog
      // ------------------------------------------------------------
      dialog = new AFModalDialog(20, 40, 200, 140, makeID("HDlg"));

      AFLabel* lbl = new AFLabel(30, 60, "Hello from AWFUI!", makeID("Helo"));
      dialog->addWidget(lbl);

      AFButton* okBtn = new AFButton(50, 100, 100, 40, makeID("OKBt"), "OK");
      okBtn->setOnClickCallback(handleDialogOkClick);
      dialog->addWidget(okBtn);
}



const int kFps                    = 60;
const int kButtonIntervalFrames   = 5 * kFps;       // fire every 5 seconds
int       eventCountdown          = kButtonIntervalFrames;

void loop() {
#ifdef AWFUI_USE_SDL
      // Process SDL events and feed touch input
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                  exit(0);
            }
            touch.handleEvent(e);
      }
#endif

      // Simulate a hardware button press every 5 seconds
      if (--eventCountdown <= 0) {
            AFEvent buttonEvent{};
            buttonEvent.type     = AFEventType::kButton;
            buttonEvent.buttonId = 1;
            eventQueue.postEvent(buttonEvent);
            eventCountdown = kButtonIntervalFrames;
      }

      world->loop();

#ifdef AWFUI_USE_SDL
      // Update the SDL window
      display.present();
      SDL_Delay(16);  // ~60 FPS
#endif
}


 
 // Entry point for Windows/Linux builds
 // On actual STM32, the Arduino framework or your startup code calls setup()/loop()
#if defined(AWFUI_USE_SDL)
int main() {
      setup();
      while (true) {
            loop();
      }
      return 0;
}
#endif