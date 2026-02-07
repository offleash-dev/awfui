//// AFWorld.cpp
//// Implementation of the AWFUI world singleton.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFScreen.h"
#include "AFWorld.h"



// Singleton instance
AFWorld* AFWorld::s_instance = nullptr;



// Initialize the singleton
// Returns false if already initialized or allocation fails
//
bool AFWorld::init(AFDisplayInterface& display, AFTouchInterface* touch) {
    if (s_instance != nullptr) {
        return false;  // Already initialized
    }
    s_instance = new AFWorld(display, touch);
    return (s_instance != nullptr);
}



// Get singleton instance
// Returns nullptr if not initialized
//
AFWorld* AFWorld::instance() {
    return s_instance;
}



// Private constructor
//
AFWorld::AFWorld(AFDisplayInterface& displayRef, AFTouchInterface* touch)
    : m_display(displayRef)
    , m_touch(touch)
{
    // Default theme initialization
    m_theme.screenBgColor = 0x0000; // black
    m_theme.textColor     = 0xFFFF; // white
    m_theme.bgColor       = 0x0000; // black
    m_theme.fgColor       = 0xFFFF; // white
    m_theme.disabledFgColor = 0x8410; // dark gray
    m_theme.disabledBgColor = 0x4208; // medium-dark gray
    m_theme.accentColor   = 0xF800; // red
    m_theme.borderColor   = 0xFFFF; // white
    m_theme.padding       = 4;
    m_theme.cornerRadius  = 4;
}



// Create a new screen and register it with the screen list
// return nullptr if allocation fails or max screens reached
//
AFScreen* AFWorld::createScreen(bool useCanvas) {
      if (!m_screenList.canAdd()) {
            // Return nullptr if max screens reached
            // Note: caller must handle this case
            return (AFScreen*) nullptr;
      }

      // Allocate a new screen
      AFScreen* screen = new AFScreen(m_display, useCanvas);

      // Register with screen list
      m_screenList.add(screen);

      return screen;
}



// Set active screen
//
void AFWorld::setActiveScreen(AFScreen* screen) {
      if (screen != nullptr) {
            m_screenList.setActive(screen);
      }
}



// Get active screen
//
AFScreen* AFWorld::getActiveScreen() {
      return m_screenList.getActive();
}



// Transform touch coordinates from physical screen space to rotated display space
// This is the inverse of Adafruit_GFX's rotation transform
//
void AFWorld::transformTouchCoordinates(int16_t& x, int16_t& y) {
    uint8_t rotation = m_display.getRotation();
    int16_t w = m_display.width();   // rotated width
    int16_t h = m_display.height();  // rotated height
    int16_t tx, ty;
    
    switch (rotation) {
        case 0:
            // No transformation needed
            break;
        case 1:
            // Physical to rotated: x' = y, y' = width - 1 - x
            tx = y;
            ty = w - 1 - x;
            x = tx;
            y = ty;
            break;
        case 2:
            // Physical to rotated: x' = width - 1 - x, y' = height - 1 - y
            x = w - 1 - x;
            y = h - 1 - y;
            break;
        case 3:
            // Physical to rotated: x' = height - 1 - y, y' = x
            tx = h - 1 - y;
            ty = x;
            x = tx;
            y = ty;
            break;
    }
}



// Poll hardware and fill AFEvent
//
void AFWorld::pollHardware(AFEvent& outEvent) {
    // Default: no event
    outEvent.type      = AFEventType::kNone;
    outEvent.x         = 0;
    outEvent.y         = 0;
    outEvent.buttonId  = 0;
    outEvent.keycode   = 0;
    outEvent.timestamp = 0;

    // Poll touch if available
    if (m_touch) {
        AFTouchPoint pt = m_touch->getPoint();

        if (pt.touched || m_wasTouched) {
              // Touch libraries report raw physical panel coordinates
              // Widgets appear to be in physical space too (screen button works without transform)
              
              if (pt.touched && !m_wasTouched) {
                    // Touch just started
                    outEvent.type = AFEventType::kTouchDown;
                    outEvent.x    = pt.x;
                    outEvent.y    = pt.y;
                    m_lastTouchX  = pt.x;
                    m_lastTouchY  = pt.y;
              } else if (!pt.touched && m_wasTouched) {
                    // Touch just ended - use last known position
                    outEvent.type = AFEventType::kTouchUp;
                    outEvent.x    = m_lastTouchX;
                    outEvent.y    = m_lastTouchY;
              } else if (pt.touched) {
                    // Ongoing touch - move/drag
                    outEvent.type = AFEventType::kTouchMove;
                    outEvent.x    = pt.x;
                    outEvent.y    = pt.y;
                    m_lastTouchX  = pt.x;
                    m_lastTouchY  = pt.y;
              }
        }
        
        m_wasTouched = pt.touched;
    }

    // TODO: integrate hardware buttons
    // TODO: integrate timers
}



// Main UI loop
//
void AFWorld::loop() {
      AFScreen* active = m_screenList.getActive();
      if (!active) {
            return;
      }

      // 1. Poll hardware → build event
      AFEvent e;
      pollHardware(e);

      // 2. Dispatch event to active screen
      active->handleEvent(e);

      // 3. Draw active screen
      active->draw();
}
