#pragma once

//// AFLabel.h
//// Part of the AWFUI library
////
//// Lightweight label widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFAdafruitCompat.h"

#include "AFWidget.h"



class AFLabel : public AFWidget {
public:
      // A simple positioned label
      AFLabel(int16_t x, int16_t y, const char* text, uint32_t id = 0);

      // A bordered label with width/height and justification
      AFLabel(int16_t x, int16_t y, int16_t w, int16_t h, const char* text, uint32_t id = 0);

      void setText(const char* t) {
            m_text = t;
            markDirty();
      }


      const char* getText() const {
            return m_text;
      }


      void setColor(uint16_t c) {
            m_color = c;
            markDirty();
      }


      int32_t getColor() const {
            return m_color;
      }
      

      virtual void draw(Adafruit_GFX& gfx) override;

      
private:
      const char*          m_text;
      int32_t              m_color         = -1;    // -1 = use theme textColor
};
