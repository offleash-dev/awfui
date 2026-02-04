#pragma once

// AFFullscreenDialog.h
// Fullscreen Dialog widget for the AWFUI library
// Inherits from AFModalDialog and is sized to the screen with no border/chrome by default

#include "AFModalDialog.h"



class AFFullscreenDialog : public AFModalDialog {
public:
      // No dimensions needed - automatically fills the screen when shown
      AFFullscreenDialog(uint32_t id = 0);

      // Override show to resize to screen dimensions
      void show(AFScreen& screen) override;

      // No border/chrome by default
      virtual void draw(Adafruit_GFX& gfx) override;
};
