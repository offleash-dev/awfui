#pragma once

//// AFTheme.h
//// Part of the AWFUI library
////
//// Theme structure and options for embedded UIs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <stdint.h>



struct AFTheme {
      uint16_t screenBgColor;
      uint16_t textColor;
      uint16_t bgColor;
      uint16_t fgColor;
      uint16_t disabledFgColor;
      uint16_t disabledBgColor;
      uint16_t accentColor;
      uint16_t borderColor;
      uint8_t  padding;
      uint8_t  cornerRadius;
};
