#pragma once

//// AFEvent.h
//// Part of the AWFUI library
////
//// Event types and structures for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>



// The event types kTouchUp, kTouchMove, kButton, kKey, and kTimer are reserved for future use by AFUI.  
// For other new types (including variants of the reserved ones) use kCustom with customType/customData relevant to the event.

enum class AFEventType { kNone, kTouchDown, kTouchUp, kTouchMove, kButton, kKey, kTimer, kCustom };



struct AFEvent {
      AFEventType type;
      int16_t     x;
      int16_t     y;
      uint8_t     buttonId;
      uint16_t    keycode;
      uint32_t    timestamp;
      uint16_t    customType;   // application-defined sub-type (e.g. gamepad button, encoder direction)
      int32_t     customData;   // application-defined payload
};