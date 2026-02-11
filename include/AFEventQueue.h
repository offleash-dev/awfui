#pragma once

//// AFEventQueue.h
//// Part of the AWFUI library
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
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <etl/queue.h>

#include "AFEvent.h"



constexpr size_t AF_EVENT_QUEUE_SIZE = 8;



class AFEventQueue {
public:
    bool hasEvent() const {
        return !m_queue.empty();
    }


    AFEvent nextEvent() {
        AFEvent e = m_queue.front();
        m_queue.pop();
        return e;
    }


    bool postEvent(const AFEvent& e) {
        if (m_queue.full()) {
            return false;
        }
        m_queue.push(e);
        return true;
    }


private:
    etl::queue<AFEvent, AF_EVENT_QUEUE_SIZE> m_queue;
};
