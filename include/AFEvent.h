#pragma once

//// AFEvent.h
//// Part of the AWFUI library
////
//// Event types and structures for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>



enum class AFEventType { kNone, kTouchDown, kTouchUp, kTouchMove, kButton, kKey, kTimer };



struct AFEvent {
      AFEventType type;
      int16_t     x;
      int16_t     y;
      uint8_t     buttonId;
      uint16_t    keycode;
      uint32_t    timestamp;
};