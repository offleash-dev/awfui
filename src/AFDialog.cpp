// AFDialog.cpp

#include "AFAdafruitCompat.h"

#include "AFDialog.h"
#include "AFWidget.h"

// Constructor
//
AFDialog::AFDialog(int16_t x, int16_t y, int16_t w, int16_t h) : AFWidget(x, y, w, h) {
      // Dialogs default to visible
      m_visible = true;
}

// Add a child widget
//
bool AFDialog::addChild(AFWidget* w) {
      bool success = false;

      if (!m_children.full()) {
            m_children.push_back(w);
            w->m_parent = this;
            success     = true;
      }

      return success;
}

// Remove a child widget
//
void AFDialog::removeChild(AFWidget* w) {
      for (size_t i = 0; i < m_children.size(); ++i) {
            if (m_children[i] == w) {
                  m_children.erase(m_children.begin() + i);
                  w->m_parent = nullptr;
                  return;
            }
      }
}

// Find child at a screen point
//
AFWidget* AFDialog::childAt(int16_t px, int16_t py) {
      for (int i = m_children.size() - 1; i >= 0; --i) {
            AFWidget* w = m_children[i];
            if (w->isVisible() && w->hitTest(px, py)) {
                  return w;
            }
      }
      return nullptr;
}

// Draw dialog and children
//
void AFDialog::draw(Adafruit_GFX& gfx) {
      if (!m_visible)
            return;

      if (m_opaque) {
            // Draw background (default: light gray)
            gfx.fillRect(m_x, m_y, m_width, m_height, 0xC618);

            // Draw border (default: black)
            gfx.drawRect(m_x, m_y, m_width, m_height, 0x0000);
      }

      // Draw children
      for (auto* w : m_children) {
            if (w->isVisible()) {
                  w->draw(gfx);
            }
      }
}

// Event routing
//
void AFDialog::handleEvent(const AFEvent& e) {
      // Only route touch events
      switch (e.type) {
            case AFEventType::kTouchDown:
            case AFEventType::kTouchUp:
            case AFEventType::kTouchMove:
                  break;
            default:
                  return;
      }

      // Hit-test children in reverse order (topmost first)
      for (int i = m_children.size() - 1; i >= 0; --i) {
            AFWidget* w = m_children[i];

            if (w->isVisible() && w->hitTest(e.x, e.y)) {
                  // Dispatch based on event type
                  switch (e.type) {
                        case AFEventType::kTouchDown:
                              w->onPress(e);
                              break;
                        case AFEventType::kTouchUp:
                              w->onRelease(e);
                              w->onClick(e); // click = release inside bounds
                              break;
                        case AFEventType::kTouchMove:
                              // Optional: add hover/drag behavior later
                              break;
                        default:
                              break;
                  }
                  return; // Stop after first hit
            }
      }
}
