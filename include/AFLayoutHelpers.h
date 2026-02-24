#pragma once

//// AFLayout.h
//// Part of the AWFUI library
////
//// Lightweight layout helpers for positioning widgets.
//// All functions are static and stateless — pure coordinate math.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>

#include "AFBase.h"

class AFWidget;
class AFScreen;
class AFPanel;



class AFLayoutHelpers {
public:

    // -----------------------------------------------------------------
    // Grid helpers
    // -----------------------------------------------------------------

    // Position of a cell in a uniform grid
    static AFRect gridCell(int16_t originX, int16_t originY,
                           int col, int row,
                           int16_t cellW, int16_t cellH,
                           int16_t gapX = 0, int16_t gapY = 0) {
        return {
            (int16_t)(originX + col * (cellW + gapX)),
            (int16_t)(originY + row * (cellH + gapY)),
            cellW,
            cellH
        };
    }


    // Origin X that centers a grid of (cols) cells within a container width
    static int16_t gridCenterX(int16_t containerW, int cols,
                               int16_t cellW, int16_t gapX = 0) {
        int16_t totalW = (int16_t)(cols * cellW + (cols - 1) * gapX);
        return (int16_t)((containerW - totalW) / 2);
    }


    // -----------------------------------------------------------------
    // Relative placement (widget-to-widget)
    // -----------------------------------------------------------------

    // X position to the right of a widget, with a gap
    static int16_t rightOf(const AFWidget& w, int16_t gap = 0);

    // X position to the left of a widget, given the new widget's width
    static int16_t leftOf(const AFWidget& w, int16_t newW, int16_t gap = 0);

    // Y position below a widget, with a gap
    static int16_t below(const AFWidget& w, int16_t gap = 0);

    // Y position above a widget, given the new widget's height
    static int16_t above(const AFWidget& w, int16_t newH, int16_t gap = 0);


    // -----------------------------------------------------------------
    // Placement within
    // -----------------------------------------------------------------

    // X position inside a panel, left-aligned with a gap
    static int16_t leftIn(const AFPanel& p, int16_t gap = 0);

    // X position inside a panel, right-aligned with a gap
    static int16_t rightIn(const AFPanel& p, int16_t newW, int16_t gap = 0);

    // Center a child horizontally within a container
    static int16_t centerX(int16_t containerX, int16_t containerW,
                           int16_t childW) {
        return (int16_t)(containerX + (containerW - childW) / 2);
    }

    // Center a child vertically within a container
    static int16_t centerY(int16_t containerY, int16_t containerH,
                           int16_t childH) {
        return (int16_t)(containerY + (containerH - childH) / 2);
    }


    // -----------------------------------------------------------------
    // Screen region helpers
    // -----------------------------------------------------------------

    // Top bar region (full width, anchored to top)
    static AFRect topBar(const AFScreen& screen, int16_t barH);

    // Bottom bar region (full width, anchored to bottom)
    static AFRect bottomBar(const AFScreen& screen, int16_t barH);

    // Content area between top and bottom bars
    static AFRect contentArea(const AFScreen& screen,
                              int16_t topBarH = 0, int16_t bottomBarH = 0);


    // -----------------------------------------------------------------
    // Dialog/Panel region helpers
    // -----------------------------------------------------------------

    // Top bar within a dialog/panel
    static AFRect topBar(const AFPanel& panel, int16_t barH);

    // Bottom bar within a dialog/panel
    static AFRect bottomBar(const AFPanel& panel, int16_t barH);

    // Content area within a dialog/panel between top and bottom bars
    static AFRect contentArea(const AFPanel& panel,
                              int16_t topBarH = 0, int16_t bottomBarH = 0);

    // Center a dialog on screen
    static AFRect centerDialog(const AFScreen& screen,
                               int16_t dialogW, int16_t dialogH);
};
