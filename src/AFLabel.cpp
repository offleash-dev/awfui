//// AFLabel.cpp
//// Implementation of the AWFUI label widget.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFWorld.h"
#include "AFLabel.h"



// Constructor
//
AFLabel::AFLabel(int16_t x_, int16_t y_, const char* text_, uint32_t id) : AFWidget(x_, y_, 0, 0, id), m_text(text_) {
      // width/height are not usable for hit-testing
      m_width  = 0;
      m_height = 0;
      m_justification = AFJustificationLeft;
      m_textSize = AFWorld::instance()->getTheme().widgetTextSize;
}



AFLabel::AFLabel(int16_t x, int16_t y, int16_t w, int16_t h, const char* text, uint32_t id) : AFWidget(x, y, w, h, id), m_text(text) {
      // width/height are usable for hit-testing
      m_width  = w;
      m_height = h;
      m_justification = AFJustificationLeft;
      m_textSize = AFWorld::instance()->getTheme().widgetTextSize;
}



// Draw the label
//
void AFLabel::draw(AFDisplayInterface& displayInterface) {
      if (!m_visible || m_text == nullptr) {
            return;
      }

      // Erase previous text region before redrawing
      if (m_lastDrawW > 0 && m_lastDrawH > 0) {
            AFWidget::erase(displayInterface, m_lastDrawX, m_lastDrawY, m_lastDrawW, m_lastDrawH);
      }

      displayInterface.setTextSize(m_textSize);

      uint16_t color = (m_color < 0) ? AFWorld::instance()->getTheme().widgetTextColor : static_cast<uint16_t>(m_color);
      displayInterface.setTextColor(color);

      if (!m_enabled) {
            color = AFWorld::instance()->getTheme().widgetDisabledFgColor;
            displayInterface.setTextColor(color);
      }

      if (m_width == 0 && m_height == 0) {
            // this just a positioned text label
            int16_t  x1, y1;
            uint16_t w, h;
            displayInterface.getTextBounds(m_text, m_x, m_y, &x1, &y1, &w, &h);

            displayInterface.setCursor(m_x, m_y);
            displayInterface.print(m_text);

            // Save drawn bounds for next erase
            m_lastDrawX = x1;
            m_lastDrawY = y1;
            m_lastDrawW = w;
            m_lastDrawH = h;
      } else {
            // this is a bordered text label that supports justification
            int16_t  x1, y1;
            uint16_t w, h;
            displayInterface.getTextBounds(m_text, 0, 0, &x1, &y1, &w, &h);

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

            displayInterface.setCursor(tx, ty);
            displayInterface.print(m_text);

            // Save drawn bounds for next erase
            m_lastDrawX = m_x;
            m_lastDrawY = m_y;
            m_lastDrawW = m_width;
            m_lastDrawH = m_height;
      }
}



// Erase the area of the text so it can be rewritten (or hidden or whatever)
//
void AFLabel::erase(AFDisplayInterface& displayInterface) {
      if (m_width != 0 && m_height != 0) {
            AFWidget::erase(displayInterface);
      } else {
            // need to calculate our bounding box to erase it.
            int16_t  x1, y1;
            uint16_t w, h;
            displayInterface.getTextBounds(m_text, m_x, m_y, &x1, &y1, &w, &h);
            AFWidget::erase(displayInterface, x1, y1, w, h);
      }
}