//// AFContainer.cpp
//// Part of the AWFUI library
////
//// Implementation of widget container interface
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFContainer.h"
#include "AFWidget.h"
#include "AFPanel.h"



// Helper implementation for widgets
//
void AFContainer::markIntersectingDirty(AFVector<AFWidget*, MAX_WIDGETS_PER_PANEL>& widgets, 
                                       int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    for (auto* widget : widgets) {
        if (widget && widget->intersects(rx, ry, rw, rh)) {
            widget->markDirty();
        }
    }
}



// Helper implementation for panels
//
void AFContainer::markIntersectingDirty(AFVector<AFPanel*, MAX_PANELS_PER_SCREEN>& panels, 
                                       int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    for (auto* panel : panels) {
        if (panel && panel->intersects(rx, ry, rw, rh)) {
            panel->markDirty();
        }
    }
}
