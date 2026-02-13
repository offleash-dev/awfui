//// AFScreen.cpp
//// Implementation of the AWFUI screen management.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFPanel.h"
#include "AFModalDialog.h"
#include "AFScreen.h"
#include "AFWorld.h"



// Constructor
//
AFScreen::AFScreen(AFDisplayInterface& displayRef, uint32_t id, bool useCanvas) : m_display(displayRef), m_id(id) {
      if (useCanvas) {
            // Create an off-screen buffer matching the display size
            m_canvas = m_display.createCanvas();
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



// Add a panel
//
void AFScreen::addPanel(AFPanel* p) {
      if (!m_panels.full()) {
            m_panels.push_back(p);
            p->m_parent = nullptr;
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
            for (auto* p : m_panels) {
                  p->markDirty();
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

      // --- Touch events: route by hit-test ---
      if (e.type == AFEventType::kTouchDown ||
          e.type == AFEventType::kTouchUp   ||
          e.type == AFEventType::kTouchMove) {

            // First route to panels (topmost last)
            for (int i = static_cast<int>(m_panels.size()) - 1; i >= 0; --i) {
                  AFPanel* p = m_panels[i];
                  if (p->isVisible() && p->hitTest(e.x, e.y)) {
                        p->handleEvent(e);
                        return;
                  }
            }

            // Then route to root-level widgets
            for (int i = static_cast<int>(m_widgets.size()) - 1; i >= 0; --i) {
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
            return;
      }

      // --- Non-positional events (kButton, kKey, kCustom): delegate to screen subclass ---
      onExternalEvent(e);
}



// Mark all widgets dirty to force a full redraw
//
void AFScreen::setNeedsFullRedraw() {
      for (auto* w : m_widgets) {
            w->markDirty();
      }
      for (auto* p : m_panels) {
            p->markDirty();
      }
      if (m_activeModal) {
            m_activeModal->markDirty();
      }
}



// Clear the screen to the specified color (default from theme)
//
void AFScreen::clear(uint16_t color) {
      AFDisplayInterface* gfx = m_canvas ? m_canvas : &m_display;
      gfx->fillScreen(color);

      // If using canvas, push to display
      if (m_canvas) {
            const uint16_t* buf = m_canvas->getCanvasBuffer();
            if (buf) {
                  m_display.drawRGBBitmap(0, 0, buf, m_display.width(), m_display.height());
            }
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
    for (auto* p : m_panels) {
        if (p->isVisible() && p->isDirty()) {
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
        clear(AFWorld::instance()->getTheme().screenBgColor);
        m_needsScreenRedraw = false;
    }

    AFDisplayInterface* gfx = m_canvas ? m_canvas : &m_display;

    // Let subclass paint a custom background (game, camera, map, etc.)
    onDrawBackground(*gfx);

    // Draw root-level widgets (only dirty ones in non-canvas mode)
    for (auto* w : m_widgets) {
        if (w->isVisible() && w->isDirty()) {
            w->draw(*gfx);
            w->clearDirty();
        }
    }

    // Draw panels
    for (auto* p : m_panels) {
        if (p->isVisible() && p->isDirty()) {
            p->draw(*gfx);
            p->clearDirty();
        }
    }

    // Draw modal dialog last (on top)
    if (m_activeModal && m_activeModal->isVisible() && m_activeModal->isDirty()) {
        m_activeModal->draw(*gfx);
        m_activeModal->clearDirty();
    }

    // If using canvas, push to display
    if (m_canvas) {
        const uint16_t* buf = m_canvas->getCanvasBuffer();
        if (buf) {
            m_display.drawRGBBitmap(0, 0, buf, m_display.width(), m_display.height());
        }
    }
}