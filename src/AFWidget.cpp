//// AFWidget.cpp
//// Implementation of the AWFUI base widget class.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFWorld.h"
#include "AFWidget.h"



// Constructor
//
AFWidget::AFWidget(int16_t x_, int16_t y_, int16_t w_, int16_t h_, uint32_t id_) : m_x(x_), m_y(y_), m_width(w_), m_height(h_), m_id(id_) {
      m_visible = true;
      m_parent  = nullptr;
      m_dirty   = true;  // Start dirty so initial draw happens
}



// Hit test: simple bounding box
//
bool AFWidget::hitTest(int16_t px, int16_t py) const {
      if (!m_visible || !m_enabled) {
            return false;
      }

      return (px >= m_x) && (py >= m_y) && (px < m_x + m_width) && (py < m_y + m_height);
}



void AFWidget::erase(AFDisplayInterface& displayInterface, int16_t x, int16_t y, int16_t w, int16_t h) {
      uint16_t fillColor = AFWorld::instance()->getTheme().widgetBgColor;
      if (!m_enabled)
            fillColor = AFWorld::instance()->getTheme().widgetDisabledBgColor;
      displayInterface.fillRect(x, y, w, h, fillColor);
}
    


void AFWidget::erase(AFDisplayInterface& displayInterface) {
      if (m_width != 0 && m_height != 0) {
            erase(displayInterface, m_x, m_y, m_width, m_height);
      }
}