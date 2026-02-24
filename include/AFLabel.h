#pragma once

//// AFLabel.h
//// Part of the AWFUI library
////
//// Lightweight label widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


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


      void setTextSize(uint8_t size) {
            m_textSize = size;
            markDirty();
      }


      uint8_t getTextSize() const {
            return m_textSize;
      }
      

      virtual void draw(AFDisplayInterface& displayInterface) override;
      virtual void erase(AFDisplayInterface& displayInterface) override;

      
private:
      const char*          m_text;
      int32_t              m_color         = -1;    // -1 = use theme textColor
      uint8_t              m_textSize      = 1;
      int16_t              m_lastDrawX     = 0;     // last drawn text origin (for erase)
      int16_t              m_lastDrawY     = 0;
      uint16_t             m_lastDrawW     = 0;     // last drawn text extent
      uint16_t             m_lastDrawH     = 0;
};