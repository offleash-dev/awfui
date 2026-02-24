//// AFLayout.cpp
//// Part of the AWFUI library
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include "AFLayoutHelpers.h"
#include "AFWidget.h"
#include "AFScreen.h"
#include "AFPanel.h"



// -----------------------------------------------------------------
// Relative placement
// -----------------------------------------------------------------

int16_t AFLayoutHelpers::rightOf(const AFWidget& w, int16_t gap) {
    return (int16_t)(w.getX() + w.getWidth() + gap);
}



int16_t AFLayoutHelpers::leftOf(const AFWidget& w, int16_t newW, int16_t gap) {
    return (int16_t)(w.getX() - newW - gap);
}



int16_t AFLayoutHelpers::below(const AFWidget& w, int16_t gap) {
    return (int16_t)(w.getY() + w.getHeight() + gap);
}



int16_t AFLayoutHelpers::above(const AFWidget& w, int16_t newH, int16_t gap) {
    return (int16_t)(w.getY() - newH - gap);
}



// -----------------------------------------------------------------
// Placement within
// -----------------------------------------------------------------

int16_t AFLayoutHelpers::leftIn(const AFPanel& p, int16_t gap) {
    return (int16_t)(p.getX() + gap);
}



int16_t AFLayoutHelpers::rightIn(const AFPanel& p, int16_t newW, int16_t gap) {
    return (int16_t)(p.getX() + p.getWidth() - newW - gap);
}



// -----------------------------------------------------------------
// Screen region helpers
// -----------------------------------------------------------------

AFRect AFLayoutHelpers::topBar(const AFScreen& screen, int16_t barH) {
    return { 0, 0, screen.getDisplay().width(), barH };
}

AFRect AFLayoutHelpers::bottomBar(const AFScreen& screen, int16_t barH) {
    int16_t screenH = screen.getDisplay().height();
    return { 0, (int16_t)(screenH - barH), screen.getDisplay().width(), barH };
}

AFRect AFLayoutHelpers::contentArea(const AFScreen& screen,
                             int16_t topBarH, int16_t bottomBarH) {
    int16_t screenW = screen.getDisplay().width();
    int16_t screenH = screen.getDisplay().height();
    return { 0, topBarH, screenW, (int16_t)(screenH - topBarH - bottomBarH) };
}



// -----------------------------------------------------------------
// Dialog/Panel region helpers
// -----------------------------------------------------------------

AFRect AFLayoutHelpers::topBar(const AFPanel& panel, int16_t barH) {
    return { panel.getX(), panel.getY(), panel.getWidth(), barH };
}

AFRect AFLayoutHelpers::bottomBar(const AFPanel& panel, int16_t barH) {
    return {
        panel.getX(),
        (int16_t)(panel.getY() + panel.getHeight() - barH),
        panel.getWidth(),
        barH
    };
}

AFRect AFLayoutHelpers::contentArea(const AFPanel& panel,
                             int16_t topBarH, int16_t bottomBarH) {
    return {
        panel.getX(),
        (int16_t)(panel.getY() + topBarH),
        panel.getWidth(),
        (int16_t)(panel.getHeight() - topBarH - bottomBarH)
    };
}

AFRect AFLayoutHelpers::centerDialog(const AFScreen& screen,
                              int16_t dialogW, int16_t dialogH) {
    int16_t screenW = screen.getDisplay().width();
    int16_t screenH = screen.getDisplay().height();
    return {
        (int16_t)((screenW - dialogW) / 2),
        (int16_t)((screenH - dialogH) / 2),
        dialogW,
        dialogH
    };
}
