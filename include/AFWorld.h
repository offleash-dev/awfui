#pragma once

// AFWorld.h
// Singleton representing the entire UI world, managing screens, themes, and events.
// Initialize once with init(), then access via instance().

#include <etl/vector.h>

#include "AFEvent.h"
#include "AFScreenList.h"
#include "AFTheme.h"
#include "AFTouchInterface.h"



class AFWorld {
public:
    // Singleton access
    // touch is optional - pass nullptr for display-only applications
    static bool init(Adafruit_GFX& display, AFTouchInterface* touch = nullptr);
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


    Adafruit_GFX& getDisplay() {
        return m_display;
    }


    AFTouchInterface* getTouch() {
        return m_touch;
    }


    // Factory: creates, stores, and returns a new screen
    // returns nullptr if allocation fails or max screens reached
    AFScreen* createScreen(bool useCanvas = false);

    // Convenience wrapper around screenList
    void setActiveScreen(AFScreen* screen);

    // Main UI loop
    void loop();

    
private:
    AFWorld(Adafruit_GFX& display, AFTouchInterface* touch);

    static AFWorld* s_instance;

    Adafruit_GFX&      m_display;
    AFTouchInterface*  m_touch;
    AFTheme            m_theme;

    // Touch state tracking for press/release detection
    bool    m_wasTouched = false;
    int16_t m_lastTouchX = 0;
    int16_t m_lastTouchY = 0;

    // Single source of truth for screens
    AFScreenList m_screenList;

    void pollHardware(AFEvent& outEvent);
};
