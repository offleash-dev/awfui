// AFFullscreenDialog.cpp

#include "AFAdafruitCompat.h"

#include "AFFullscreenDialog.h"
#include "AFScreen.h"

// Constructor: full‑screen modal dialog
//
AFFullscreenDialog::AFFullscreenDialog(int16_t screenW, int16_t screenH) : AFModalDialog(0, 0, screenW, screenH) {
      // Fullscreen dialogs are visible by default
      m_visible = true;
}

// Draw: fullscreen, no border, simple background
//
void AFFullscreenDialog::draw(Adafruit_GFX& gfx) {
      if (!m_visible)
            return;

      // Fill entire screen area
      gfx.fillRect(m_x, m_y, m_width, m_height, 0x0000); // default black

      // Draw children
      for (auto* w : m_children) {
            if (w->isVisible()) {
                  w->draw(gfx);
            }
      }
}
