//// AFLabel.cpp
//// Implementation of the AWFUI label widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFLabel.h"
#include "AFWorld.h"



// Constructor
//
AFLabel::AFLabel(int16_t x_, int16_t y_, const char* text_, uint32_t id) : AFWidget(x_, y_, 0, 0, id), m_text(text_) {
      // width/height are not usable for hit-testing
      m_width  = 0;
      m_height = 0;
      m_justification = AFJustificationLeft;
}



AFLabel::AFLabel(int16_t x, int16_t y, int16_t w, int16_t h, const char* text, uint32_t id) : AFWidget(x, y, w, h, id), m_text(text) {
      // width/height are usable for hit-testing
      m_width  = w;
      m_height = h;
      m_justification = AFJustificationLeft;
}



// Draw the label
//
void AFLabel::draw(AFDisplayInterface& gfx) {
      if (!m_visible || m_text == nullptr) {
            return;
      }

      uint16_t color = (m_color < 0) ? AFWorld::instance()->getTheme().textColor : static_cast<uint16_t>(m_color);
      gfx.setTextColor(color);

      if (m_width == 0 && m_height == 0) {
            // this just a positioned text label
            gfx.setCursor(m_x, m_y);
            gfx.print(m_text);
      } else {
            // this is a bordered text label that supports justification
            int16_t  x1, y1;
            uint16_t w, h;
            gfx.getTextBounds(m_text, 0, 0, &x1, &y1, &w, &h);

            int16_t tx, ty;

            // Horizontal justification
            switch (m_justification) {
                  case AFJustificationCenter:
                        tx = m_x + (m_width - w) / 2;
                        break;
                  case AFJustificationRight:
                        tx = m_x + m_width - w;
                        break;
                  case AFJustificationLeft:
                  default:
                        tx = m_x;
                        break;
            }

            // Vertical centering
            ty = m_y + (m_height - h) / 2;

            gfx.setCursor(tx, ty);
            gfx.print(m_text);
      }
}
