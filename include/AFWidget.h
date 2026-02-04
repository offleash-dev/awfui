#pragma once

//// AFWidget.h
//// Part of the AWFUI library
////
//// Base widget class for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>

#include "AFAdafruitCompat.h"

#include "AFEvent.h"



class AFWidget {
public:
    AFWidget(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);
    virtual ~AFWidget() = default;

    virtual void draw(Adafruit_GFX& gfx) = 0;
    virtual bool hitTest(int16_t px, int16_t py) const;


    virtual void onPress(const AFEvent& e) {
    }


    virtual void onRelease(const AFEvent& e) {
    }


    virtual void onClick(const AFEvent& e) {
    }


    virtual void onKey(const AFEvent& e) {
    }


    virtual void onButton(const AFEvent& e) {
    }


    void setVisible(bool v) {
        if (m_visible != v) {
            m_visible = v;
            markDirty();
        }
    }


    bool isVisible() const {
        return m_visible;
    }


    void markDirty() {
        m_dirty = true;
    }


    bool isDirty() const {
        return m_dirty;
    }


    void clearDirty() {
        m_dirty = false;
    }


    int16_t getX() const {
        return m_x;
    }


    int16_t getY() const {
        return m_y;
    }


    int16_t getWidth() const {
        return m_width;
    }


    int16_t getHeight() const {
        return m_height;
    }


    uint32_t getId() const {
        return m_id;
    }


    void setJustification(AFJustification j) {
            m_justification = j;
            markDirty();
    }


    AFJustification getJustification() const {
            return m_justification;
    }

    
protected:
    int16_t   m_x, m_y;
    int16_t   m_width, m_height;
    bool      m_visible = true;
    bool      m_dirty   = true;  // Start dirty so initial draw happens
    uint32_t   m_id;
    AFWidget* m_parent  = nullptr;
    AFJustification m_justification = AFJustificationCenter;

    friend class AFDialog;
    friend class AFScreen;
};
