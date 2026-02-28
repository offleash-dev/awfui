#pragma once

//// AFWidget.h
//// Part of the AWFUI library
////
//// Base widget class for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>

#include "AFDisplayInterface.h"

#include "AFBase.h"
#include "AFEvent.h"



class AFWidget {
public:
    AFWidget(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);
    virtual ~AFWidget() = default;

    virtual void draw(AFDisplayInterface& displayInterface) = 0;
    virtual bool hitTest(int16_t px, int16_t py) const;
    virtual void erase(AFDisplayInterface& displayInterface);


    virtual void onPress(const AFEvent& e) {
          unused(e);
    }


    virtual void onMove(const AFEvent& e) {
          unused(e);
    }


    virtual void onRelease(const AFEvent& e) {
          unused(e);
    }


    virtual void onClick(const AFEvent& e) {
          unused(e);
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


    void setEnabled(bool e) {
        if (m_enabled != e) {
            m_enabled = e;
            markDirty();
        }
    }


    bool isEnabled() const {
        return m_enabled;
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


    void erase(AFDisplayInterface& displayInterface, int16_t x, int16_t y, int16_t w, int16_t h);


protected:
    int16_t   m_x, m_y;
    int16_t   m_width, m_height;
    bool      m_enabled = true;
    bool      m_visible = true;
    bool      m_dirty   = true;  // Start dirty so initial draw happens
    bool      m_owned   = false; // If true, container will delete this widget
    uint32_t   m_id;
    uint8_t   m_eventMask = kEventTouch;  // default touch only
    AFWidget* m_parent  = nullptr;
    AFJustification m_justification = AFJustificationCenter;

    friend class AFPanel;
    friend class AFScreen;
};