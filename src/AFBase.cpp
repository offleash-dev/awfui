#include "AFBase.h"

//// AFBase.cpp
//// Part of the AWFUI library
////
//// A utility collection for AWFUI
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <stdint.h>

#include "AFBase.h"
#include "AFWidget.h"
#include "AFPanel.h"


static char* tempIdBuffer5 = new char[5];
char* getAsChars(int32_t id) {
      getAsChars(id, tempIdBuffer5);
      return tempIdBuffer5;
}



void markIntersectingDirty(etl::vector<AFWidget*, MAX_WIDGETS_PER_SCREEN>& widgets, 
                          int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    for (auto* widget : widgets) {
        if (widget && widget->intersects(rx, ry, rw, rh)) {
            widget->markDirty();
        }
    }
}



void markIntersectingDirty(etl::vector<AFPanel*, MAX_PANELS_PER_SCREEN>& panels, 
                          int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    for (auto* panel : panels) {
        if (panel && panel->intersects(rx, ry, rw, rh)) {
            panel->markDirty();
        }
    }
}