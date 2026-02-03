// AFModalDialog.cpp

#include "AFModalDialog.h"
#include "AFScreen.h"

// Constructor
//
AFModalDialog::AFModalDialog(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id) : AFDialog(x, y, w, h, id) {
      // Modal dialogs are visible by default
      m_visible = true;
}

// Show the modal dialog on a screen
//
void AFModalDialog::show(AFScreen& screen) {
      if (m_owner)
            return; // already visible

      m_owner = &screen;
      setVisible(true);
      markDirty();
      onShow();
      screen.showModal(this);
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
