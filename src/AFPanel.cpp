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



// Destructor
//
AFPanel::~AFPanel() {
      for (auto* w : m_widgets) {
            if (w->m_owned) delete w;
      }
}



// Add a child widget to the panel
//
bool AFPanel::addWidget(AFWidget* w, bool owned) {
      bool success = false;

      if (!m_widgets.full()) {
            m_widgets.push_back(w);
            w->m_parent = this;
            w->m_owned  = owned;
            w->markDirty();  // Mark as dirty so it gets drawn
            success     = true;
      }

      return success;
}



// Remove a child widget from the panel 
//
void AFPanel::removeWidget(AFWidget* w) {
      for (size_t i = 0; i < m_widgets.size(); ++i) {
            if (m_widgets[i] == w) {
                  m_widgets.erase(m_widgets.begin() + i);
                  w->m_parent = nullptr;
                  w->m_owned  = false;
                  return;
            }
      }
}



// Find child at a screen point
//
AFWidget* AFPanel::widgetAt(int16_t px, int16_t py) {
      for (int i = static_cast<int>(m_widgets.size()) - 1; i >= 0; --i) {
            AFWidget* w = m_widgets[i];
            if (w->isVisible() && w->hitTest(px, py)) {
                  return w;
            }
      }
      return nullptr;
}



// Draw panel and child widgets
//
void AFPanel::draw(AFDisplayInterface& displayInterface) {
      if (!m_visible)
            return;

      if (m_opaque) {
            const AFTheme& theme = AFWorld::instance()->getTheme();
            
            // Use DMA-accelerated fill for large panel backgrounds (dialogs, etc.)
            if (displayInterface.isDMAAvailable()) {
                  displayInterface.fastFillRectDMA(m_x, m_y, m_width, m_height, theme.widgetBgColor);
            } else {
                  displayInterface.fillRect(m_x, m_y, m_width, m_height, theme.widgetBgColor);
            }
            
            displayInterface.drawRect(m_x, m_y, m_width, m_height, theme.widgetBorderColor);
      }

      // Draw child widgets
      for (auto* w : m_widgets) {
            if (w->isVisible() && isDirty()) {
                  w->draw(displayInterface);
                  w->clearDirty();
            }
      }
      
      // Clear our own dirty flag after drawing children
      clearDirty();
}



bool AFPanel::isDirty() const {
      if (m_dirty)
            return true;

      for (auto* w : m_widgets) {
            if (w->isVisible() && w->isDirty()) {
                  return true;
            }
      }
      return false;
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

      // kTouchDown: hit-test child widgets in reverse order (topmost first)
      for (int i = static_cast<int>(m_widgets.size()) - 1; i >= 0; --i) {
            AFWidget* w = m_widgets[i];

            if (!(w->m_eventMask & eventMaskForType(e.type))) continue;

            if (w->isVisible() && w->hitTest(e.x, e.y)) {
                  m_pressedWidget = w;
                  w->onPress(e);
                  return;
            }
      }
}