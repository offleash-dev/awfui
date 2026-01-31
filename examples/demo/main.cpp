// main.cpp

#include "AFAdafruitCompat.h"
#include <Adafruit_ILI9341.h>

#include "AFButton.h"
#include "AFLabel.h"
#include "AFModalDialog.h"
#include "AFScreen.h"
#include "AFWorld.h"



// Example display - STM32L475 IoT01 board uses no-arg constructor
// (pins are configured via BSP/HAL)
Adafruit_ILI9341 tft;


AFWorld*       world;
AFScreen*      mainScreen;
AFModalDialog* dialog;

void setup() {
      tft.begin();
      tft.setRotation(1);

      world      = new AFWorld(tft);
      mainScreen = world->createScreen(true);

      // ------------------------------------------------------------
      // Main screen button
      // ------------------------------------------------------------
      AFButton* openBtn = new AFButton(40, 40, 160, 50, "Open Dialog");
      openBtn->setOnClickCallback([]() { dialog->show(*mainScreen); });
      mainScreen->addWidget(openBtn);

      // ------------------------------------------------------------
      // Modal dialog
      // ------------------------------------------------------------
      dialog = new AFModalDialog(20, 40, 200, 140);

      AFLabel* lbl = new AFLabel(30, 60, "Hello from AFUI!");
      dialog->addChild(lbl);

      AFButton* okBtn = new AFButton(50, 100, 100, 40, "OK");
      okBtn->setOnClickCallback([]() { dialog->dismiss(); });
      dialog->addChild(okBtn);
}



void loop() {
      world->loop();
}

// Entry point for Windows build testing
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