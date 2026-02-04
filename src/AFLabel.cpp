// AFLabel.cpp

#include "AFLabel.h"



// Constructor
//
AFLabel::AFLabel(int16_t x_, int16_t y_, const char* text_, uint32_t id) : AFWidget(x_, y_, 0, 0, id), m_text(text_) {
      // width/height are not used for hit-testing unless you want them to be
}



// Draw the label
//
void AFLabel::draw(Adafruit_GFX& gfx) {
      if (!m_visible || m_text == nullptr) {
            return;
      }

      gfx.setTextColor(m_color);
      gfx.setCursor(m_x, m_y);
      gfx.print(m_text);
}