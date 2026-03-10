#pragma once

//// AFModalDialog.h
//// Part of the AWFUI library
////
//// Modal dialog widget for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFPanel.h"



class AFScreen; // forward declaration



class AFModalDialog : public AFPanel {
public:
      AFModalDialog(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);

      friend class AFScreen;  // Allow AFScreen to set m_owner

      virtual void show(AFScreen& screen);
      void show();  // Convenience - shows on active screen
      void dismiss();

      
      bool isModal() const {
            return true;
      }

      
private:
      AFScreen* m_owner = nullptr;
};