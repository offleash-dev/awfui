#include "AFBase.h"

//// AFBase.cpp
//// Part of the AWFUI library
////
//// A utility collection for AWFUI
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <stdint.h>

#include "AFBase.h"
#include "AFWidget.h"
#include "AFPanel.h"


static char* tempIdBuffer5 = new char[5];

char* getIdAsCharPtr(uint32_t id) {
      getIdAsChars(id, tempIdBuffer5);
      return tempIdBuffer5;
}