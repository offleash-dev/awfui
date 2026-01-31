#pragma once

// AFWidget.h
// Base widget class for the AWFUI library

#include <stdint.h>

#include "AFAdafruitCompat.h"

#include "AFEvent.h"



class AFWidget {
public:
    AFWidget(int16_t x, int16_t y, int16_t w, int16_t h);
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


    // Dirty flag for redraw optimization
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


protected:
    int16_t   m_x, m_y;
    int16_t   m_width, m_height;
    bool      m_visible = true;
    bool      m_dirty   = true;  // Start dirty so initial draw happens
    AFWidget* m_parent  = nullptr;

    friend class AFDialog;
    friend class AFScreen;
};