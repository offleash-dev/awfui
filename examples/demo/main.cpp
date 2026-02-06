// main.cpp

// Tell SDL we handle our own main() entry point
#ifdef AFUI_USE_SDL
#define SDL_MAIN_HANDLED
#endif

#include "AFAdafruitCompat.h"



#ifdef AFUI_USE_SDL
#include "gfx_sdl.hpp"
#include "AFTouchSDL.h"
#else
#include <Adafruit_ILI9341.h>
#endif

#include "AFButton.h"
#include "AFLabel.h"
#include "AFModalDialog.h"
#include "AFScreen.h"
#include "AFWorld.h"



#ifdef AFUI_USE_SDL
// SDL desktop simulation - 320x240 display at 2x scale
GFX_SDL      tft(240, 320);
AFTouchSDL   touch(2);
#else
// Example display - STM32L475 IoT01 board uses no-arg constructor
// (pins are configured via BSP/HAL)
Adafruit_ILI9341 tft;
#endif


AFWorld*       world;
AFScreen*      mainScreen;
AFModalDialog* dialog;

void setup() {
#ifdef AFUI_USE_SDL
      SDL_SetMainReady();
#endif
      tft.begin();

      tft.setRotation(1);

#ifdef AFUI_USE_SDL
      AFWorld::init(tft, &touch);
#else
      AFWorld::init(tft);
#endif
      world      = AFWorld::instance();
      mainScreen = world->createScreen(true);

      // ------------------------------------------------------------
      // Main screen button
      // ------------------------------------------------------------
      AFButton* openBtn = new AFButton(40, 40, 160, 50, makeID("Open"), "Open Dialog");
      openBtn->setOnClickCallback([]() { dialog->show(*mainScreen); });
      mainScreen->addWidget(openBtn);

      // ------------------------------------------------------------
      // Modal dialog
      // ------------------------------------------------------------
      dialog = new AFModalDialog(20, 40, 200, 140, makeID("HDlg"));

      AFLabel* lbl = new AFLabel(30, 60, "Hello from AFUI!", makeID("Helo"));
      dialog->addChild(lbl);

      AFButton* okBtn = new AFButton(50, 100, 100, 40, makeID("OKBt"), "OK");
      okBtn->setOnClickCallback([]() { dialog->dismiss(); });
      dialog->addChild(okBtn);
}



void loop() {
#ifdef AFUI_USE_SDL
      // Process SDL events and feed touch input
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                  exit(0);
            }
            touch.handleEvent(e);
      }
#endif

      world->loop();

#ifdef AFUI_USE_SDL
      // Update the SDL window
      tft.present();
      SDL_Delay(16);  // ~60 FPS
#endif
}

// Entry point for Windows/Linux builds
// On actual STM32, the Arduino framework or your startup code calls setup()/loop()
#if defined(_WIN32) || defined(__linux__)
int main() {
      setup();
      while (true) {
            loop();
      }
      return 0;
}
#endif
