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



// Forward declaration
class AFScreen;



class AFWidget {
public:
    AFWidget() = default;  // Default constructor for stack objects
    AFWidget(int16_t x, int16_t y, int16_t w, int16_t h, uint32_t id = 0);
    virtual ~AFWidget() = default;

    virtual void draw(AFDisplayInterface& displayInterface) = 0;
    virtual void draw(AFDisplayInterface& displayInterface, int16_t screenOffsetX, int16_t screenOffsetY);
    virtual bool hitTest(int16_t px, int16_t py) const;
    virtual void erase(AFDisplayInterface& displayInterface);

    virtual void handleEvent(const AFEvent& e) {
          unused(e);
    }


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


    virtual void setVisible(bool v) {
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


    virtual void markDirty() {
        m_dirty = true;
        // Note: We don't cascade to parent because needsRedraw() already checks all children
    }


    virtual bool isDirty() const {
        return m_dirty;
    }


    void clearDirty() {
        m_dirty = false;
    }


    // Force immediate redraw - useful for progress bars, sliders, etc.
    void update();


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


    // Check if this widget intersects with a rectangle
    bool intersects(int16_t rx, int16_t ry, int16_t rw, int16_t rh) const {
        return !(m_x >= rx + rw || m_x + m_width <= rx ||
                 m_y >= ry + rh || m_y + m_height <= ry);
    }


    uint32_t getId() const {
        return m_id_uint32;
    }

    const char* getIdString() const {
        return m_id_chars;
    }

    void setIdString(const char* id) {
        m_id_chars = const_cast<char*>(id);
    }

    // Owner management
    AFScreen* getOwner() const {
        return m_owner;
    }

    void setOwner(AFScreen* owner) {
        m_owner = owner;
    }


    bool isContainer() const {
        return m_isContainer;
    }


    void setJustification(AFJustification j) {
            m_justification = j;
            markDirty();
    }


    AFJustification getJustification() const {
            return m_justification;
    }


    AFWidget* getParent() const { return m_parent; }


    uint8_t getEventMask() const { return m_eventMask; }


    void erase(AFDisplayInterface& displayInterface, int16_t x, int16_t y, int16_t w, int16_t h);


protected:
    int16_t   m_x, m_y;
    int16_t   m_width, m_height;
    bool      m_enabled = true;
    bool      m_visible = true;
    bool      m_dirty   = true;  // Start dirty so initial draw happens
    bool      m_owned   = false; // If true, container will delete this widget
    bool      m_isContainer = false; // If true, widget can contain child widgets (e.g. panels)
    union {
        uint32_t m_id_uint32;
        char*    m_id_chars;
    };
    uint8_t   m_eventMask = kEventTouch;  // default touch only
    AFWidget* m_parent  = nullptr;
    AFScreen* m_owner   = nullptr;  // Screen that manages this widget
    AFJustification m_justification = AFJustificationCenter;

    friend class AFPanel;
    friend class AFScreen;
};