//// AFFullscreenDialog.cpp
//// Implementation of the AWFUI fullscreen dialog widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFFullscreenDialog.h"
#include "AFScreen.h"
#include "AFWorld.h"



// Constructor: full‑screen modal dialog
// Dimensions are set when show() is called
//
AFFullscreenDialog::AFFullscreenDialog(uint32_t id) 
    : AFModalDialog(0, 0, 0, 0, id) 
{
      // Fullscreen dialogs are visible by default
      setVisible(true);
      m_opaque = true;
}



// Override show to resize to screen dimensions
//
void AFFullscreenDialog::show(AFScreen& screen) {
      // Get screen dimensions and resize to fill
      const AFDisplayInterface& display = screen.getDisplay();
      m_x = 0;
      m_y = 0;
      m_width = display.width();
      m_height = display.height();
      
      // Call parent show
      AFModalDialog::show(screen);
}



// Draw: fullscreen, no border, simple background
//
void AFFullscreenDialog::draw(AFDisplayInterface& displayInterface) {
      if (!isVisible())
            return;

      // Fill entire screen area
      if (AFWidget::isDirty()) {
            displayInterface.fillRect(m_x, m_y, m_width, m_height, AFWorld::instance()->getTheme().screenBgColor);
      }

      // Draw child widgets
      for (auto* w : m_widgets) {
            if (w->isVisible() && w->isDirty()) {
                  w->draw(displayInterface);
                  w->clearDirty();
            }
      }

      clearDirty();
}



void AFFullscreenDialog::dismiss() {
      AFModalDialog::dismiss();
      if (m_owner) {
            m_owner->setNeedsFullRedraw();
      }
}
