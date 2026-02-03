#pragma once

// AFScreen.h
// Screen management for the AWFUI library

#include <etl/vector.h>

#include "AFAdafruitCompat.h"

#include "AFBase.h"
#include "AFDialog.h"
#include "AFEvent.h"
#include "AFModalDialog.h"
#include "AFWidget.h"



class AFScreen {
    public:
      AFScreen(Adafruit_GFX& display, uint32_t id = 0, bool useCanvas = false);
      virtual ~AFScreen();

      void addWidget(AFWidget* w);
      void addDialog(AFDialog* d);

      void showModal(AFModalDialog* d);
      void dismissModal(AFModalDialog* d);

      void handleEvent(const AFEvent& e);
      void draw();

      // Check if any widget needs redraw
      bool needsRedraw() const;

      // Clear the screen to the specified color (default black)
      void clear(uint16_t color = 0x0000);

      // Mark all widgets dirty to force a full redraw
      void setNeedsFullRedraw();

      Adafruit_GFX& getDisplay() {
            return m_display;
      }

      uint32_t getId() const {
            return m_id;
      }

      
    private:
      Adafruit_GFX& m_display;
      GFXcanvas16*  m_canvas = nullptr;
      bool          m_needsScreenRedraw = false;
      uint32_t      m_id;

      etl::vector<AFWidget*, MAX_WIDGETS_PER_DIALOG> m_widgets;
      etl::vector<AFDialog*, MAX_DIALOGS_PER_SCREEN> m_dialogs;
      AFModalDialog*                                 m_activeModal = nullptr;
};
