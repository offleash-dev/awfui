//// AFWidget.cpp
//// Implementation of the AWFUI base widget class.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFWorld.h"
#include "AFWidget.h"



// Constructor
//
AFWidget::AFWidget(int16_t x_, int16_t y_, int16_t w_, int16_t h_, ID_TYPE id_) : m_x(x_), m_y(y_), m_width(w_), m_height(h_), m_id(storeID(id_)) {
      // Flags already initialized in header: enabled, visible, dirty
      m_parent  = nullptr;
}



// Draw with screen offset (for child widgets)
//
void AFWidget::draw(AFDisplayInterface& displayInterface, int16_t screenOffsetX, int16_t screenOffsetY) {
      // Save original position
      int16_t originalX = m_x;
      int16_t originalY = m_y;
      
      // Temporarily set screen position
      m_x += screenOffsetX;
      m_y += screenOffsetY;
      
      // Draw using screen coordinates
      draw(displayInterface);
      
      // Restore original local position
      m_x = originalX;
      m_y = originalY;
}



// Hit test: simple bounding box
//
bool AFWidget::hitTest(int16_t px, int16_t py) const {
      if (!isVisible() || !isEnabled()) {
            return false;
      }

      return (px >= m_x) && (py >= m_y) && (px < m_x + m_width) && (py < m_y + m_height);
}



void AFWidget::erase(AFDisplayInterface& displayInterface, int16_t x, int16_t y, int16_t w, int16_t h) {
      uint16_t fillColor = AFWorld::instance()->getTheme().widgetBgColor;
      if (!isEnabled())
            fillColor = AFWorld::instance()->getTheme().widgetDisabledBgColor;
      displayInterface.fillRect(x, y, w, h, fillColor);
}
    


void AFWidget::erase(AFDisplayInterface& displayInterface) {
      if (m_width != 0 && m_height != 0) {
            erase(displayInterface, m_x, m_y, m_width, m_height);
      }
}


void AFWidget::update() {
        if (isDirty()) {
              draw(AFWorld::instance()->getDisplay());
              clearDirty();
        }
}