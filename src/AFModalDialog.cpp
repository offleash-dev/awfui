// AFModalDialog.cpp

#include "AFModalDialog.h"
#include "AFScreen.h"

// Constructor
//
AFModalDialog::AFModalDialog(int16_t x, int16_t y, int16_t w, int16_t h) : AFDialog(x, y, w, h) {
      // Modal dialogs are visible by default
      m_visible = true;
}

// Show the modal dialog on a screen
//
void AFModalDialog::show(AFScreen& screen) {
      if (m_owner)
            return; // already visible

      m_owner = &screen;
      onShow();
      screen.showModal(this);
}

// Dismiss the modal dialog
//
void AFModalDialog::dismiss() {
      if (m_owner) {
            onHide();
            m_owner->dismissModal(this);
            m_owner = nullptr;
      }
}
