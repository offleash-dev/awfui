#pragma once

// AFLabel.h
// Label widget for the AWFUI library


#include "AFAdafruitCompat.h"

#include "AFWidget.h"



class AFLabel : public AFWidget {
public:
      AFLabel(int16_t x, int16_t y, const char* text, uint32_t id = 0);

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


      uint16_t getColor() const {
            return m_color;
      }


      virtual void draw(Adafruit_GFX& gfx) override;

      
private:
      const char* m_text;
      uint16_t    m_color = 0xFFFF; // default white
};
