// AFScreen.cpp

#include "AFAdafruitCompat.h"

#include "AFDialog.h"
#include "AFModalDialog.h"
#include "AFScreen.h"



// Constructor
//
AFScreen::AFScreen(Adafruit_GFX& displayRef, uint32_t id, bool useCanvas) : m_display(displayRef), m_id(id) {
      if (useCanvas) {
            // Create an off-screen buffer matching the display size
            m_canvas = new GFXcanvas16(m_display.width(), m_display.height());
      }
}



// Destructor
//
AFScreen::~AFScreen() {
      if (m_canvas) {
            delete m_canvas;
            m_canvas = nullptr;
      }
}



// Add a normal widget (not a dialog)
//
void AFScreen::addWidget(AFWidget* w) {
      if (!m_widgets.full()) {
            m_widgets.push_back(w);
            w->m_parent = nullptr; // root-level widget
      }
}



// Add a non-modal dialog
//
void AFScreen::addDialog(AFDialog* d) {
      if (!m_dialogs.full()) {
            m_dialogs.push_back(d);
            d->m_parent = nullptr;
      }
}



// Show a modal dialog
//
void AFScreen::showModal(AFModalDialog* d) {
      m_activeModal = d;
}



// Dismiss a modal dialog
//
void AFScreen::dismissModal(AFModalDialog* d) {
      if (m_activeModal == d) {
            m_activeModal = nullptr;
            // Mark all widgets dirty so they redraw and cover the dismissed modal
            for (auto* w : m_widgets) {
                  w->markDirty();
            }
            for (auto* dlg : m_dialogs) {
                  dlg->markDirty();
            }

            m_needsScreenRedraw = true;
      }
}



// Event routing
//
void AFScreen::handleEvent(const AFEvent& e) {
      // Modal dialog gets exclusive input
      if (m_activeModal) {
            m_activeModal->handleEvent(e);
            return;
      }

      // First route to dialogs (topmost last)
      for (int i = m_dialogs.size() - 1; i >= 0; --i) {
            AFDialog* d = m_dialogs[i];
            if (d->isVisible() && d->hitTest(e.x, e.y)) {
                  d->handleEvent(e);
                  return;
            }
      }

      // Then route to root-level widgets 
      for (int i = m_widgets.size() - 1; i >= 0; --i) {
            AFWidget* w = m_widgets[i];
            if (w->isVisible() && w->hitTest(e.x, e.y)) {
                  switch (e.type) {
                        case AFEventType::kTouchDown:
                              w->onPress(e);
                              break;
                        case AFEventType::kTouchUp:
                              w->onRelease(e);
                              w->onClick(e);
                              break;
                        case AFEventType::kTouchMove:
                              // Optional: add hover/drag later
                              break;
                        default:
                              break;
                  }
                  return;
            }
      }
}



// Mark all widgets dirty to force a full redraw
//
void AFScreen::setNeedsFullRedraw() {
      for (auto* w : m_widgets) {
            w->markDirty();
      }
      for (auto* d : m_dialogs) {
            d->markDirty();
      }
      if (m_activeModal) {
            m_activeModal->markDirty();
      }
}



// Clear the screen
//
void AFScreen::clear(uint16_t color) {
      Adafruit_GFX* gfx = m_canvas ? (Adafruit_GFX*) m_canvas : &m_display;
      gfx->fillScreen(color);

      // If using canvas, push to display
      if (m_canvas) {
            m_display.drawRGBBitmap(0, 0, m_canvas->getBuffer(), m_display.width(), m_display.height());
      }
}



// Check if any widget needs redraw
//
bool AFScreen::needsRedraw() const {
    if (m_needsScreenRedraw)
        return true;
      ;
    for (auto* w : m_widgets) {
        if (w->isVisible() && w->isDirty()) {
            return true;
        }
    }
    for (auto* d : m_dialogs) {
        if (d->isVisible() && d->isDirty()) {
            return true;
        }
    }
    if (m_activeModal && m_activeModal->isVisible() && m_activeModal->isDirty()) {
        return true;
    }
    return false;
}



// Draw everything
//
void AFScreen::draw() {
    // Skip if nothing needs redraw
    if (!needsRedraw()) {
        return;
    }

    if (m_needsScreenRedraw) {
        clear();
        m_needsScreenRedraw = false;
    }

    Adafruit_GFX* gfx = m_canvas ? (Adafruit_GFX*) m_canvas : &m_display;

    // Draw root-level widgets (only dirty ones in non-canvas mode)
    for (auto* w : m_widgets) {
        if (w->isVisible() && w->isDirty()) {
            w->draw(*gfx);
            w->clearDirty();
        }
    }

    // Draw dialogs
    for (auto* d : m_dialogs) {
        if (d->isVisible() && d->isDirty()) {
            d->draw(*gfx);
            d->clearDirty();
        }
    }

    // Draw modal dialog last (on top)
    if (m_activeModal && m_activeModal->isVisible() && m_activeModal->isDirty()) {
        m_activeModal->draw(*gfx);
        m_activeModal->clearDirty();
    }

    // If using canvas, push to display
    if (m_canvas) {
        m_display.drawRGBBitmap(0, 0, m_canvas->getBuffer(), m_display.width(), m_display.height());
    }
}
