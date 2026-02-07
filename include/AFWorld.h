#pragma once

//// AFWorld.h
//// Part of the AWFUI library
////
//// Singleton representing the entire UI world, managing screens, themes, and events.
//// Initialize once with init(), then access via instance().
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <etl/vector.h>

#include "AFDisplayInterface.h"
#include "AFEvent.h"
#include "AFScreenList.h"
#include "AFTheme.h"
#include "AFTouchInterface.h"



class AFWorld {
public:
    // Singleton access
    // touch is optional - pass nullptr for display-only applications
    static bool init(AFDisplayInterface& display, AFTouchInterface* touch = nullptr);
    static AFWorld* instance();

    // No copy/move
    AFWorld(const AFWorld&) = delete;
    AFWorld& operator=(const AFWorld&) = delete;


    void setTheme(const AFTheme& t) {
        m_theme = t;
    }


    const AFTheme& getTheme() const {
        return m_theme;
    }


    AFDisplayInterface& getDisplay() {
        return m_display;
    }


    AFTouchInterface* getTouch() {
        return m_touch;
    }


    // Factory: creates, stores, and returns a new screen
    // returns nullptr if allocation fails or max screens reached
    AFScreen* createScreen(bool useCanvas = false);

    // Accessors for screenList
    void setActiveScreen(AFScreen* screen);
    AFScreen* getActiveScreen();

    // Main UI loop
    void loop();

    
private:
    AFWorld(AFDisplayInterface& display, AFTouchInterface* touch);

    static AFWorld* s_instance;

    AFDisplayInterface& m_display;
    AFTouchInterface*  m_touch;
    AFTheme            m_theme;

    // Touch state tracking for press/release detection
    bool    m_wasTouched = false;
    int16_t m_lastTouchX = 0;
    int16_t m_lastTouchY = 0;

    // Single source of truth for screens
    AFScreenList m_screenList;

    void pollHardware(AFEvent& outEvent);
    
    // Transform touch coordinates from physical screen space to rotated display space
    void transformTouchCoordinates(int16_t& x, int16_t& y);
};
