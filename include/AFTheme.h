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

      uint16_t widgetTextColor;
      uint16_t widgetBgColor;
      uint16_t widgetFgColor;
      uint16_t widgetDisabledFgColor;
      uint16_t widgetDisabledBgColor;
      uint16_t widgetAccentColor;
      uint16_t widgetBorderColor;
      uint8_t  widgetPadding;
      uint8_t  widgetCornerRadius;

      uint16_t dialogBorderColor;
      uint8_t  dialogCornerRadius;
};