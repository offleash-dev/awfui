//// AFPanel.cpp
//// Implementation of the AWFUI panel widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include "AFWorld.h"



#include "AFPanel.h"
#include "AFWidget.h"



// Constructor
//
AFPanel::AFPanel(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id) : AFWidget(x, y, w, h, id) {
      // Panels default to visible
      m_visible = true;
}



// Add a child widget to the panel
//
bool AFPanel::addChild(AFWidget* w) {
      bool success = false;

      if (!m_children.full()) {
            m_children.push_back(w);
            w->m_parent = this;
            success     = true;
      }

      return success;
}



// Remove a child widget from the panel 
//
void AFPanel::removeChild(AFWidget* w) {
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
AFWidget* AFPanel::childAt(int16_t px, int16_t py) {
      for (int i = static_cast<int>(m_children.size()) - 1; i >= 0; --i) {
            AFWidget* w = m_children[i];
            if (w->isVisible() && w->hitTest(px, py)) {
                  return w;
            }
      }
      return nullptr;
}



// Draw panel and children
//
void AFPanel::draw(AFDisplayInterface& gfx) {
      if (!m_visible)
            return;

      if (m_opaque) {
            const AFTheme& theme = AFWorld::instance()->getTheme();
            gfx.fillRect(m_x, m_y, m_width, m_height, theme.widgetBgColor);
            gfx.drawRect(m_x, m_y, m_width, m_height, theme.widgetBorderColor);
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
void AFPanel::handleEvent(const AFEvent& e) {
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
      for (int i = static_cast<int>(m_children.size()) - 1; i >= 0; --i) {
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
