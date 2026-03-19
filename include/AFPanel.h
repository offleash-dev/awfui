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
#include "AFContainer.h"



class AFPanel : public AFWidget, public AFContainer {
public:
      AFPanel() = default;  // Default constructor for stack objects
      AFPanel(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);
      virtual ~AFPanel();

      // attaches and the panel takes ownership if indicated.
      // panel owned objects are deleted when the panel is deleted
      // caller owned objects are not deleted when the panel is deleted
      bool addWidget(AFWidget* w, bool owned = false); // returns if successfully added (false indicates full)

      // detaches only. removes ownership by the panel and th e caller takes responsibility for deleting
      void removeWidget(AFWidget* w);

      // Panel management
      bool addPanel(AFPanel* p, bool owned = false); // returns if successfully added (false indicates full)
      void removePanel(AFPanel* p);

      AFWidget* widgetAt(int16_t px, int16_t py);

      virtual void draw(AFDisplayInterface& displayInterface) override;

      // Event handling, panels/dialog widgets handled here
      void handleEvent(const AFEvent& e) override;
     
      virtual bool isDirty() const override;
      virtual void setVisible(bool v) override;
      
      // Container interface implementation
      void markIntersectingWidgetsDirty(int16_t rx, int16_t ry, int16_t rw, int16_t rh) override;
      
      
      int16_t toScreenX(int16_t localX) const {
            return m_x + localX;
      }


      int16_t toScreenY(int16_t localY) const {
            return m_y + localY;
      }


      int16_t toLocalX(int16_t screenX) const {
            return screenX - m_x;
      }

      
      int16_t toLocalY(int16_t screenY) const {
            return screenY - m_y;
      }


      void localToScreen(int16_t& x, int16_t& y) const {
            x += m_x;
            y += m_y;
      }


      void screenToLocal(int16_t& x, int16_t& y) const {
            x -= m_x;
            y -= m_y;
      }


      virtual void onShow() {
      }


      virtual void onHide() {
      }


      virtual void markDirty() override;


protected:

      bool m_opaque = true;
      AFWidget* m_pressedWidget = nullptr;  // implicit capture for drag

      void fillBackgroundRect(AFDisplayInterface& displayInterface);
};