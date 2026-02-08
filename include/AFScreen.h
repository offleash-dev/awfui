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
#include "AFDialog.h"
#include "AFEvent.h"
#include "AFModalDialog.h"
#include "AFWidget.h"



class AFScreen {
public:
      AFScreen(AFDisplayInterface& display, uint32_t id = 0, bool useCanvas = false);
      virtual ~AFScreen();

      void addWidget(AFWidget* w);
      void addDialog(AFDialog* d);

      void showModal(AFModalDialog* d);
      void dismissModal(AFModalDialog* d);

      void handleEvent(const AFEvent& e);
      void draw();

      // Check if any widget needs redraw
      bool needsRedraw() const;

      // Clear the screen to the specified color (default from theme)
      void clear(uint16_t color = 0xFFFF);

      // Mark all widgets dirty to force a full redraw
      void setNeedsFullRedraw();

      AFDisplayInterface& getDisplay() {
            return m_display;
      }


      uint32_t getId() const {
            return m_id;
      }

      
private:
      AFDisplayInterface& m_display;
      AFDisplayInterface*  m_canvas = nullptr;
      bool          m_needsScreenRedraw = false;
      uint32_t      m_id;

      etl::vector<AFWidget*, MAX_WIDGETS_PER_DIALOG> m_widgets;
      etl::vector<AFDialog*, MAX_DIALOGS_PER_SCREEN> m_dialogs;
      AFModalDialog*                                 m_activeModal = nullptr;
};