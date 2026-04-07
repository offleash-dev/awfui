#pragma once

//// AFTouchInterface.h
//// Part of the AWFUI library
////
//// Abstract interface for touch input devices.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>



struct AFTouchPoint {
    int16_t x;
    int16_t y;
    bool    touched;
};



// Callback types for acccessing touch hardware on a shared hardware bus.
// Eg. these are used to manage locking of the SPI bus when multiple components need to access it.
// If defined, AWFUI will call the hold callback before touch operations and the release callback after, 
// to allow the backend to manage bus access appropriately.  
// They are optional if you manage the bus on your own or the dsplay is not shared with other components.
// Also note that these callbacks work for quick operations.  Long operation might be better handled by the host app directly,
// or maybe you need to prebuild a UI and keep it around instead of building it on the fly.

using AFSharedTouchCallback = void (*)();



class AFTouchInterface {
public:
    virtual ~AFTouchInterface() = default;
    virtual bool begin() = 0;
    virtual AFTouchPoint getPoint() = 0;


    void setSharedTouchCallbacks(AFSharedTouchCallback acquire, AFSharedTouchCallback release) {
        m_acquireTouchCallback = acquire;
        m_releaseTouchCallback = release;
    }


    void sharedTouchAcquire() {
        if (m_acquireTouchCallback) {
            m_acquireTouchCallback();
        }
    }


    void sharedTouchRelease() {
        if (m_releaseTouchCallback) {
            m_releaseTouchCallback();
        }
    }

protected:
    AFSharedTouchCallback m_acquireTouchCallback = nullptr;
    AFSharedTouchCallback m_releaseTouchCallback = nullptr;
};