#pragma once

// AFFullscreenDialog.h
// Fullscreen Dialog widget for the AWFUI library
// Inherits from AFModalDialog and is sized to the screen with no border/chrome by default

#include "AFModalDialog.h"



class AFFullscreenDialog : public AFModalDialog {
    public:
      AFFullscreenDialog(int16_t screenW, int16_t screenH);

      // No border/chrome by default
      virtual void draw(Adafruit_GFX& gfx) override;
};
