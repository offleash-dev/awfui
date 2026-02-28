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
      for (auto* w : m_widgets) {
            if (w->m_owned) delete w;
      }
      for (auto* p : m_panels) {
            if (p->m_owned) delete p;
      }
      if (m_canvas) {
            delete m_canvas;
            m_canvas = nullptr;
      }
}



// Add a normal widget (not a dialog)
//
bool AFScreen::addWidget(AFWidget* w, bool owned) {
      bool success = false;

      if (!m_widgets.full()) {
            m_widgets.push_back(w);
            w->m_parent = nullptr; // root-level widget
            w->m_owned  = owned;

            success = true;           
      }

      return success;
}



// Add a panel
//
bool AFScreen::addPanel(AFPanel* p, bool owned) {
      bool success = false;

      if (!m_panels.full()) {
            m_panels.push_back(p);
            p->m_parent = nullptr;
            p->m_owned  = owned;

            success = true;
      }

      return success;
}



// Remove a widget from the screen
//
void AFScreen::removeWidget(AFWidget* w) {
      for (size_t i = 0; i < m_widgets.size(); ++i) {
            if (m_widgets[i] == w) {
                  m_widgets.erase(m_widgets.begin() + i);
                  w->m_parent = nullptr;
                  w->m_owned  = false;
                  return;
            }
      }
}



// Remove a panel from the screen
//

void AFScreen::removePanel(AFPanel* p) {
      for (size_t i = 0; i < m_panels.size(); ++i) {
            if (m_panels[i] == p) {
                  m_panels.erase(m_panels.begin() + i);
                  p->m_parent = nullptr;
                  p->m_owned  = false;
                  return;
            }
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

      // --- Touch events: route with implicit capture ---
      if (e.type == AFEventType::kTouchDown ||
          e.type == AFEventType::kTouchUp   ||
          e.type == AFEventType::kTouchMove) {

            // Move/Up go to captured widget (no hit-test)
            if (e.type == AFEventType::kTouchMove && m_pressedWidget) {
                  m_pressedWidget->onMove(e);
                  return;
            }
            if (e.type == AFEventType::kTouchUp && m_pressedWidget) {
                  AFWidget* w = m_pressedWidget;
                  m_pressedWidget = nullptr;
                  w->onRelease(e);
                  if (w->hitTest(e.x, e.y)) {
                        w->onClick(e);
                  }
                  return;
            }

            // kTouchDown: hit-test to find and capture a widget

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
                  
                  if (!(w->m_eventMask & eventMaskForType(e.type))) continue;

                  if (w->isVisible() && w->hitTest(e.x, e.y)) {
                        m_pressedWidget = w;
                        w->onPress(e);
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
      m_needsScreenRedraw = true;

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
      AFDisplayInterface* displayInterface = m_canvas ? m_canvas : &m_display;
      displayInterface->fillScreen(color);

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

    AFDisplayInterface* displayInterface = m_canvas ? m_canvas : &m_display;

    // Let subclass paint a custom background (game, camera, map, etc.)
    onDrawBackground(*displayInterface);

    // Draw root-level widgets (only dirty ones in non-canvas mode)
    for (auto* w : m_widgets) {
        if (w->isVisible() && w->isDirty()) {
            w->draw(*displayInterface);
            w->clearDirty();
        }
    }

    // Draw panels
    for (auto* p : m_panels) {
        if (p->isVisible() && p->isDirty()) {
            p->draw(*displayInterface);
            p->clearDirty();
        }
    }

    // Draw modal dialog last (on top)
    if (m_activeModal && m_activeModal->isVisible() && m_activeModal->isDirty()) {
        m_activeModal->draw(*displayInterface);
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