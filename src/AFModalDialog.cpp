//// AFModalDialog.cpp
//// Implementation of the AWFUI modal dialog widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFModalDialog.h"
#include "AFScreen.h"
#include "AFWorld.h"



// Constructor
//
AFModalDialog::AFModalDialog(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id) : AFPanel(x, y, w, h, id) {
      // Modal dialogs are visible by default
      m_visible = true;
      m_opaque = true;
}



// Show the modal dialog on a screen
//
void AFModalDialog::show(AFScreen& screen) {
      if (m_owner)
            return; // already visible

      m_owner = &screen;
      setVisible(true);
      markDirty();

      // Mark all child widgets dirty so they draw with the dialog
      for (auto* w : m_widgets) {
            w->markDirty();
      }

      onShow();
      screen.showModal(this);
}



// Convenience: show on the active screen
//
void AFModalDialog::show() {
      AFWorld* world = AFWorld::instance();
      if (!world) return;
      
      AFScreen* active = world->getActiveScreen();
      if (active) {
            show(*active);  // calls virtual show(AFScreen&)
      }
}



// Dismiss the modal dialog
//
void AFModalDialog::dismiss() {
      if (m_owner) {
            onHide();
            setVisible(false);
            m_owner->dismissModal(this);

            m_owner = nullptr;
      }
}