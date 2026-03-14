#pragma once

//// AFScreen.h
//// Part of the AWFUI library
////
//// Screen management for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <etl/vector.h>

#include "AFDisplayInterface.h"

#include "AFBase.h"
#include "AFPanel.h"
#include "AFEvent.h"
#include "AFModalDialog.h"
#include "AFWidget.h"



class AFScreen {
public:
      AFScreen(AFDisplayInterface& display, uint32_t id = 0, bool useCanvas = false);
      virtual ~AFScreen();

      // if owned, the screen or panel will delete the widget when deleted. 
      // if not owned, caller is responsible for deleting the widget.
      bool addWidget(AFWidget* w, bool owned = false);  // returns false if max widgets reached
      bool addPanel(AFPanel* p, bool owned = false);    // returns false if max panels reached

      // detaches only. removes ownership by the panel and the caller takes responsibility for deleting
      void removeWidget(AFWidget* w);
      void removePanel(AFPanel* p);

      void showModal(AFModalDialog* d);
      void dismissModal(AFModalDialog* d);

      void handleEvent(const AFEvent& e);
      void draw();

      // Check if any child object needs redraw
      bool needsRedraw() const;

      // Clear the screen to the specified color (default from theme)
      void clear(uint16_t color = 0xFFFF);

      // Mark all widgets dirty to force a full redraw
      void setNeedsFullRedraw();

      AFDisplayInterface& getDisplay() {
            return m_display;
      }


      const AFDisplayInterface& getDisplay() const {
            return m_display;
      }


      uint32_t getId() const {
            return m_id;
      }


protected:
      // Override in subclass to handle non-positional events (kButton, kKey, kCustom)
      virtual void onExternalEvent(const AFEvent& e) {
            unused(e);
      }


      // Override in subclass to draw a custom background before widgets
      // (e.g. game frame, camera feed, map).  Called each frame before widget drawing.
      virtual void onDrawBackground(AFDisplayInterface& displayInterface) {
            unused(displayInterface);
      }

      
protected:
     AFDisplayInterface*  m_canvas = nullptr;
 
 
private:
      AFDisplayInterface& m_display;
      bool          m_needsScreenRedraw = false;
      uint32_t      m_id;

      etl::vector<AFWidget*, MAX_WIDGETS_PER_SCREEN>        m_widgets;
      etl::vector<AFPanel*, MAX_PANELS_PER_SCREEN>          m_panels;
      etl::vector<AFModalDialog*, MAX_DIALOGS_PER_SCREEN>   m_modalStack;  // Modal dialog stack
      AFWidget*                                             m_pressedWidget = nullptr;  // implicit capture for drag
};