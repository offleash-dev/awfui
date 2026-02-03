// AFWidget.cpp

#include "AFWidget.h"

// Constructor
//

AFWidget::AFWidget(int16_t x_, int16_t y_, int16_t w_, int16_t h_, uint32_t id_) : m_x(x_), m_y(y_), m_width(w_), m_height(h_), m_id(id_) {
      m_visible = true;
      m_parent  = nullptr;
}



// Hit test: simple bounding box
//
bool AFWidget::hitTest(int16_t px, int16_t py) const {
      if (!m_visible) {
            return false;
      }

      return (px >= m_x) && (py >= m_y) && (px < m_x + m_width) && (py < m_y + m_height);
}
