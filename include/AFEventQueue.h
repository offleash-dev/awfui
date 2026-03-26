#pragma once

//// AFEventQueue.h
//// Part of the AWFUI library
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.
////
//// A fixed-size event queue that external sources (GPIO ISRs, RTOS tasks,
//// gamepads, encoders, etc.) can post AFEvents into.  AFWorld drains the
//// queue each loop iteration.
////
//// Usage from an STM32 GPIO callback or button-polling task:
////
////     AFEventQueue eventQueue;
////
////     // In GPIO callback:
////     AFEvent e{};
////     e.type       = AFEventType::kButton;
////     e.buttonId   = 1;
////     eventQueue.postEvent(e);
////
////     // At init:
////     AFWorld::init(display, &touch, &eventQueue);
////
//// For RTOS, wrap postEvent/nextEvent with a mutex in a subclass.

#include <etl/queue.h>

#include "AFEvent.h"



constexpr size_t AF_EVENT_QUEUE_SIZE = 8;



class AFEventQueue {
public:
    virtual ~AFEventQueue() = default;

    bool hasEvent() {
        lock();
        bool result = !m_queue.empty();
        unlock();
        return result;
    }


    AFEvent nextEvent() {
        lock();
        AFEvent e = m_queue.front();
        m_queue.pop();
        unlock();
        return e;
    }


    bool postEvent(const AFEvent& e) {
        lock();
        if (m_queue.full()) {
            unlock();
            return false;
        }
        m_queue.push(e);
        unlock();
        return true;
    }


protected:
    // Override in RTOS subclass to provide mutual exclusion
    virtual void lock() {}
    virtual void unlock() {}


private:
    etl::queue<AFEvent, AF_EVENT_QUEUE_SIZE> m_queue;
};