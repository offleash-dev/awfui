//// AFPanel.cpp
//// Implementation of the AWFUI panel widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.



#include "AFWorld.h"
#include "AFPanel.h"
#include "AFScreen.h"
#include "AFWidget.h"
#include "AFBase.h"



// Constructor
//
AFPanel::AFPanel(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id)
    : AFWidget(x, y, w, h, id) {
      // Panels default to visible
      AFWidget::setVisible(true);
      AFWidget::setIsContainer(true); // Panels can contain child widgets
}



// Destructor
//
AFPanel::~AFPanel() {
      for (auto* w : m_widgets) {
            if (w->isOwned()) delete w;
      }
}



// Add a child widget to the panel
//
bool AFPanel::addWidget(AFWidget* w, bool owned) {
      bool success = false;

      if (!m_widgets.full()) {
            m_widgets.push_back(w);
            w->m_parent = this;
            w->m_owner  = m_owner;  // Inherit owner from panel
            w->setOwned(owned);
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
                  w->setOwned(false);
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



void AFPanel::fillBackgroundRect(AFDisplayInterface& displayInterface) {
      const AFTheme& theme = AFWorld::instance()->getTheme();
            
      // Use DMA-accelerated fill for large panel backgrounds (dialogs, etc.)
      if (displayInterface.isDMAAvailable()) {
            displayInterface.fastFillRectDMA(m_x, m_y, m_width, m_height, theme.widgetBgColor);
      } else {
            displayInterface.fillRect(m_x, m_y, m_width, m_height, theme.widgetBgColor);
      }
}




// Draw panel and child widgets
//
void AFPanel::draw(AFDisplayInterface& displayInterface) {
      if (!AFWidget::isVisible())
            return;

      bool didRedraw = false;
      if (m_opaque && AFWidget::isDirty()) { 
            const AFTheme& theme = AFWorld::instance()->getTheme();
            fillBackgroundRect(displayInterface);

            displayInterface.drawRect(m_x, m_y, m_width, m_height, theme.widgetBorderColor);
            didRedraw = true;
      }

      // Draw child widgets with screen offset
      for (auto* w : m_widgets) {
            if ((w->isVisible() && w->isDirty()) || didRedraw) {
                  w->draw(displayInterface, m_x, m_y);  // Convert local to screen
                  w->clearDirty();
            }
      }
      
      // Clear our own dirty flag after drawing children
      AFWidget::clearDirty();
 }



void AFPanel::setVisible(bool v) {
      bool wasVisible = AFWidget::isVisible();
      if (wasVisible != v) {
            if (!v && wasVisible) {
                  // When hiding, mark intersecting widgets dirty so they get redrawn
                  if (m_owner) {
                        fillBackgroundRect(m_owner->getDisplay());

                        // Convert local coordinates to screen coordinates
                        int16_t screenX = m_x;
                        int16_t screenY = m_y;
                        
                        // If parent is a panel, convert to screen coordinates
                        if (m_parent && m_parent->isContainer()) {
                            AFPanel* parentPanel = static_cast<AFPanel*>(m_parent);
                            screenX = parentPanel->toScreenX(m_x);
                            screenY = parentPanel->toScreenY(m_y);
                        }
                        
                        // Mark underlying widgets/panels dirty 
                        m_owner->markIntersectingWidgetsDirty(screenX, screenY, m_width, m_height);
                  }
            }
            
            AFWidget::setVisible(v);
            markDirty();
      }
}



bool AFPanel::isDirty() const {
      if (AFWidget::isDirty())
            return true;

      for (auto* w : m_widgets) {
            if (w->isVisible() && w->isDirty()) {
                  return true;
            }
      }
      return false;
}



void AFPanel::markDirty() {
      AFWidget::markDirty();
      for (auto* w : m_widgets) {
            w->markDirty();
      }
}



// Event routing
//
void AFPanel::handleEvent(const AFEvent& e) {
//    printf("AFPanel::handleEvent type=%d, x=%d, y=%d, panelID=%s\n", (int)e.type, e.x, e.y, getAsChars(getId()));
      
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
            capturedWidgetMove(e);
            return;
      }
      if (e.type == AFEventType::kTouchUp && m_pressedWidget) {
            capturedWidgetRelease(e);
            return;
      }

      // kTouchDown: hit-test child widgets in reverse order (topmost first)
      for (int i = static_cast<int>(m_widgets.size()) - 1; i >= 0; --i) {
            AFWidget* w = m_widgets[i];
            
            // Convert screen coordinates to panel-local coordinates for hit testing
            int16_t localX = e.x - m_x;
            int16_t localY = e.y - m_y;
            

            if (!(w->m_eventMask & eventMaskForType(e.type))) continue;

            if (w->isVisible() && w->hitTest(localX, localY)) {
//                  printf("  Widget %d hit! Capturing\n", i);
//                  printf("Widget ID: %s\n", getAsChars(w->getId()));
                  
                  // If this is a panel, let it handle its own children
                  if (w->isContainer()) {
                        // Convert screen coordinates to panel's local coordinates
                        static_cast<AFPanel*>(w)->handleEvent(e);
                        return;
                  }
                  
                  // Otherwise, capture this widget
                  captureWidget(w, e);
                  
                  return;
            }
      }
}



void AFPanel::captureWidget(AFWidget* w, const AFEvent& e) {
      m_pressedWidget = w;
      w->onPress(e);
}



void AFPanel::capturedWidgetMove(const AFEvent& e) {
      m_pressedWidget->onMove(e);
}



void AFPanel::capturedWidgetRelease(const AFEvent& e) {
      AFWidget* w = m_pressedWidget;
      m_pressedWidget = nullptr;
      
      w->onRelease(e);
      int16_t localX = e.x - m_x;  // Convert screen to panel-local
      int16_t localY = e.y - m_y;
      if (w->hitTest(localX, localY)) {
            w->onClick(e);
      }
}



// Add panel to container
//
bool AFPanel::addPanel(AFPanel* p, bool owned) {
    if (!m_panels.full()) {
        m_panels.push_back(p);
        p->m_parent = this;
        p->m_owner = m_owner;  // Inherit owner from parent panel
        p->setOwned(owned);
        p->markDirty();  // Mark as dirty so it gets drawn
        return true;
    }
    return false;
}

// Remove panel from container
//
void AFPanel::removePanel(AFPanel* p) {
    for (size_t i = 0; i < m_panels.size(); ++i) {
        if (m_panels[i] == p) {
            m_panels.erase(m_panels.begin() + i);
            p->m_parent = nullptr;
            p->setOwned(false);
            return;
        }
    }
}

// Mark intersecting widgets dirty
//
void AFPanel::markIntersectingWidgetsDirty(int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    markIntersectingDirty(m_widgets, rx, ry, rw, rh);
    markIntersectingDirty(m_panels, rx, ry, rw, rh);
}