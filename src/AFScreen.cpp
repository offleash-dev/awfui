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
AFScreen::AFScreen(AFDisplayInterface& displayRef, ID_TYPE id, bool useCanvas) : m_display(displayRef), m_id(id) {
      if (useCanvas) {
            // Create an off-screen buffer matching the display size
            m_canvas = m_display.createCanvas();
      }
}



// Destructor
//
AFScreen::~AFScreen() {
      for (auto* w : m_widgets) {
            if (w->isOwned()) delete w;
      }
      for (auto* p : m_panels) {
            if (p->isOwned()) delete p;
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
            w->m_owner  = this;    // screen owns the widget
            w->setOwned(owned);

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
            p->m_owner  = this;    // screen owns the panel
            p->setOwned(owned);

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
                  w->setOwned(false);
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
                  p->setOwned(false);
                  return;
            }
      }
}



// Show a modal dialog
//
void AFScreen::showModal(AFModalDialog* d) {
      // Clear dirty flags on all widgets to prevent drawing over modal
      for (auto* w : m_widgets) {
            w->clearDirty();
      }
      for (auto* p : m_panels) {
            p->clearDirty();
      }           

      m_modalStack.push_back(d);
      // Set owner directly so dismiss() works (don't call show() - we're already showing)
      d->m_owner = this;
      
      // Only clear the dialog area, not the entire screen
      AFDisplayInterface* displayInterface = m_canvas ? m_canvas : &m_display;
      if (displayInterface->isDMAAvailable()) {
            displayInterface->fastFillRectDMA(d->getX(), d->getY(), d->getWidth(), d->getHeight(), 
                                              AFWorld::instance()->getTheme().screenBgColor);
      } else {
            displayInterface->fillRect(d->getX(), d->getY(), d->getWidth(), d->getHeight(), 
                                       AFWorld::instance()->getTheme().screenBgColor);
      }
}



// Dismiss a modal dialog
//
void AFScreen::dismissModal(AFModalDialog* d) {
      // Find and remove this dialog from the stack
      for (auto it = m_modalStack.begin(); it != m_modalStack.end(); ++it) {
            if (*it == d) {
            m_modalStack.erase(it);
                break;
            }
        }
            
            // Only clear the dialog area, not the entire screen
            AFDisplayInterface* displayInterface = m_canvas ? m_canvas : &m_display;
            if (displayInterface->isDMAAvailable()) {
                  displayInterface->fastFillRectDMA(d->getX(), d->getY(), d->getWidth(), d->getHeight(), 
                                                  AFWorld::instance()->getTheme().screenBgColor);
            } else {
                  displayInterface->fillRect(d->getX(), d->getY(), d->getWidth(), d->getHeight(), 
                                           AFWorld::instance()->getTheme().screenBgColor);
            }
            
            // Only mark widgets that were under the dialog as dirty
            for (auto* w : m_widgets) {
                  if (w->isVisible() && w->intersects(d->getX(), d->getY(), d->getWidth(), d->getHeight())) {
                        w->markDirty();
                  }
            }
            for (auto* p : m_panels) {
                  if (p->isVisible() && p->intersects(d->getX(), d->getY(), d->getWidth(), d->getHeight())) {
                        p->markDirty();
                  }
            }
            
            // Mark the new top modal as dirty (if there is one)
            if (!m_modalStack.empty()) {
                  m_modalStack.back()->markDirty();
      }
}



// Event routing
//
void AFScreen::handleEvent(const AFEvent& e) {
      // Top modal dialog gets exclusive input
      if (!m_modalStack.empty()) {
            m_modalStack.back()->handleEvent(e);
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
      for (auto* modal : m_modalStack) {
            modal->markDirty();
      }
}



// Clear the screen to the specified color (default from theme)
//
void AFScreen::clear(uint16_t color) {
      AFDisplayInterface* displayInterface = m_canvas ? m_canvas : &m_display;
      
      // Use DMA-accelerated fill if available for full screen clears
      if (displayInterface->isDMAAvailable()) {
            displayInterface->fastFillRectDMA(0, 0, displayInterface->width(), displayInterface->height(), color);
      } else {
            displayInterface->fillScreen(color);
      }

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

      for (auto* modal : m_modalStack) {
            if (modal->isVisible() && modal->isDirty()) {
                  return true;
            }
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

    // Draw modal dialogs last (on top)
    for (auto* modal : m_modalStack) {
        if (modal->isVisible() && modal->isDirty()) {
            modal->draw(*displayInterface);
            modal->clearDirty();
        }
    }

    // If using canvas, push to display
    if (m_canvas) {
        const uint16_t* buf = m_canvas->getCanvasBuffer();
        if (buf) {
            m_display.drawRGBBitmap(0, 0, buf, m_display.width(), m_display.height());
        }
    }
}

// Mark intersecting widgets dirty
//
void AFScreen::markIntersectingWidgetsDirty(int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    markIntersectingDirty(m_widgets, rx, ry, rw, rh);
    markIntersectingDirty(m_panels, rx, ry, rw, rh);
}