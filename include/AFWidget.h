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
        bool wasVisible = m_flags & FLAG_VISIBLE;
        if (wasVisible != v) {
            if (v) 
                m_flags |= FLAG_VISIBLE;
            else 
                m_flags &= ~FLAG_VISIBLE;
            markDirty();
        }
    }


    bool isVisible() const {
        return m_flags & FLAG_VISIBLE;
    }


    void setEnabled(bool e) {
        bool wasEnabled = m_flags & FLAG_ENABLED;
        if (wasEnabled != e) {
            if (e) 
                m_flags |= FLAG_ENABLED;
            else 
                m_flags &= ~FLAG_ENABLED;
            markDirty();
        }
    }


    bool isEnabled() const {
        return m_flags & FLAG_ENABLED;
    }


    virtual void markDirty() {
        m_flags |= FLAG_DIRTY;
        // Note: We don't cascade to parent because needsRedraw() already checks all children
    }


    virtual bool isDirty() const {
        return m_flags & FLAG_DIRTY;
    }


    void clearDirty() {
        m_flags &= ~FLAG_DIRTY;
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
#if AWFUI_DISABLE_IDS
            return AFUI_ID_NONE; // IDs are disabled, return none
#else
            return m_id;
#endif
    }


    AFScreen* getOwner() const {
        return m_owner;
    }


    void setOwner(AFScreen* owner) {
        m_owner = owner;
    }


    bool isContainer() const {
        return m_flags & FLAG_CONTAINER;
    }


    void setIsContainer(bool container) {
        if (container) 
            m_flags |= FLAG_CONTAINER;
        else 
            m_flags &= ~FLAG_CONTAINER;
    }


    bool isOwned() const {
        return m_flags & FLAG_OWNED;
    }


    void setOwned(bool owned) {
        if (owned) 
            m_flags |= FLAG_OWNED;
        else 
            m_flags &= ~FLAG_OWNED;
    }


    void setJustification(AFJustification j) {
            m_flags = (m_flags & ~FLAG_JUSTIFICATION_MASK) | (static_cast<uint8_t>(j) << FLAG_JUSTIFICATION_SHIFT);
            markDirty();
    }


    AFJustification getJustification() const {
            return static_cast<AFJustification>((m_flags & FLAG_JUSTIFICATION_MASK) >> FLAG_JUSTIFICATION_SHIFT);
    }


    AFWidget* getParent() const { return m_parent; }


    uint8_t getEventMask() const { return m_eventMask; }


    void erase(AFDisplayInterface& displayInterface, int16_t x, int16_t y, int16_t w, int16_t h);


protected:
    // Flag bits for packed boolean storage
    static constexpr uint8_t FLAG_ENABLED     = 0x01;
    static constexpr uint8_t FLAG_VISIBLE     = 0x02;
    static constexpr uint8_t FLAG_DIRTY       = 0x04;
    static constexpr uint8_t FLAG_OWNED       = 0x08;
    static constexpr uint8_t FLAG_CONTAINER   = 0x10;
    // Justification uses bits 5-6 (2 bits for 3 values)
    static constexpr uint8_t FLAG_JUSTIFICATION_MASK   = 0x60;  // 0110 0000
    static constexpr uint8_t FLAG_JUSTIFICATION_SHIFT  = 5;

    int16_t   m_x, m_y;
    int16_t   m_width, m_height;

     // Start enabled, visible, dirty, centered
    uint8_t   m_flags = FLAG_ENABLED | FLAG_VISIBLE | FLAG_DIRTY | (AFJustificationCenter << FLAG_JUSTIFICATION_SHIFT);

    uint32_t  m_id = 0;

    uint8_t   m_eventMask = kEventTouch;  // default touch only
    AFWidget* m_parent  = nullptr;
    AFScreen* m_owner   = nullptr;  // Screen that manages this widget

    friend class AFPanel;
    friend class AFScreen;
};