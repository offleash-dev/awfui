#pragma once

//// AFDialog.h
//// Part of the AWFUI library
////
//// Lightweight dialog widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <etl/vector.h>

#include "AFBase.h"
#include "AFWidget.h"



class AFDialog : public AFWidget {
public:
      AFDialog(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);
      virtual ~AFDialog() = default;

      bool addChild(AFWidget* w); // returns if successfully added (false indicates full)
      void removeChild(AFWidget* w);

      AFWidget* childAt(int16_t px, int16_t py);

      virtual void draw(AFDisplayInterface& gfx) override;
      virtual void handleEvent(const AFEvent& e);

      
      int16_t toScreenX(int16_t localX) const {
            return m_x + localX;
      }


      int16_t toScreenY(int16_t localY) const {
            return m_y + localY;
      }


      virtual void onShow() {
      }


      virtual void onHide() {
      }


protected:
      etl::vector<AFWidget*, MAX_WIDGETS_PER_DIALOG> m_children;

      bool m_opaque = true;
      int  m_zOrder = 0; // unused as yet
};
