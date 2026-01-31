#pragma once

// AFEvent.h
// Event types for the AWFUI library

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
