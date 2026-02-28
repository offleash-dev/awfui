#pragma once

//// AFPanel.h
//// Part of the AWFUI library
////
//// Lightweight panel widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <etl/vector.h>

#include "AFBase.h"
#include "AFWidget.h"



class AFPanel : public AFWidget {
public:
      AFPanel(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);
      virtual ~AFPanel();

      // attaches and the panel takes ownership if indicated.
      // panel owned objects are deleted when the panel is deleted
      // caller owned objects are not deleted when the panel is deleted
      bool addWidget(AFWidget* w, bool owned = false); // returns if successfully added (false indicates full)

      // detaches only. removes ownership by the panel and th e caller takes responsibility for deleting
      void removeWidget(AFWidget* w);

      AFWidget* widgetAt(int16_t px, int16_t py);

      virtual void draw(AFDisplayInterface& displayInterface) override;
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
      etl::vector<AFWidget*, MAX_WIDGETS_PER_PANEL> m_widgets;

      bool m_opaque = true;
      int  m_zOrder = 0; // unused as yet
      AFWidget* m_pressedWidget = nullptr;  // implicit capture for drag
};