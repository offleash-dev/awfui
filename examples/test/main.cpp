// main.cpp — AWFUI test application
//
// Screen 1: Click test — corner buttons, status label, checkbox to enable/disable
// Screen 2: Radio test — radio buttons, checkboxes to enable/disable each other
// Screen 3: Panel test — top panel with buttons, bottom panel with image buttons, dialog

// Tell SDL we handle our own main() entry point


#ifndef AWFUI_USE_SDL
#define AWFUI_USE_SDL
#endif

#ifdef AWFUI_USE_SDL
#define SDL_MAIN_HANDLED
#endif


#ifdef AWFUI_USE_SDL

#include "AFDisplaySDL.h"
#include "AFTouchSDL.h"

#else

#include "AFDisplayAdafruitGFX.h"
#include <Adafruit_ILI9341.h>

#endif

#include <stdio.h>
#include "AFWorld.h"
#include "testUi.h"



// ============================================================
// Hardware setup
// ============================================================

#ifdef AWFUI_USE_SDL
AFDisplaySDL    display(240, 320);
AFTouchSDL   touch(2);
#else
Adafruit_ILI9341 tft;
AFDisplayAdafruitGFX display(tft);
#endif


AFWorld*  world;

// ============================================================
// Setup and loop
// ============================================================
#define THE_BIG_TEST 1
#define STACKED_MODAL_TEST 0
extern void testAFVector();
extern void testAFQueue();

void setup() {
#ifdef AWFUI_USE_SDL
      SDL_SetMainReady();
      display.begin();
      display.setRotation(1);
#else
      tft.begin();
      tft.setRotation(1);
#endif

#ifdef AWFUI_USE_SDL
      AFWorld::init(display, &touch);
#else
      AFWorld::init(display);
#endif

      testAFVector();
      testAFQueue();

      world = AFWorld::instance();

      int16_t w = display.width();
      int16_t h = display.height();
#if THE_BIG_TEST
      setupUI(w, h);
#endif
#if STACKED_MODAL_TEST
      setupStackedModalTestUI(w, h);
#endif
}



void loop() {
#ifdef AWFUI_USE_SDL
      SDL_Event e;
      while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                  exit(0);
            }
            touch.handleEvent(e);
      }
#endif

      world->loop();

#ifdef AWFUI_USE_SDL
      display.present();
      SDL_Delay(16);  // ~60 FPS
#endif
}



// Entry point
int main() {
      setup();
      while (true) {
            loop();
      }
      return 0;
}
