#pragma once

//// AFFullscreenDialog.h
//// Part of the AWFUI library
////
//// Fullscreen dialog widget for embedded UIs.
//// Inherits from AFModalDialog and is sized to the screen with no border/chrome by default.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

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
