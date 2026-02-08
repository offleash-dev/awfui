#pragma once

//// AFScreenList.h
//// Part of the AWFUI library
////
//// Manages a list of AFScreen objects for the AFWorld.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.

#include <etl/vector.h>

#include "AFBase.h"



class AFScreen;

class AFScreenList {
public:
      bool canAdd() const {
            return !m_screens.full();
      }


      void add(AFScreen* screen);
      void remove(AFScreen* screen);

      bool      setActive(int16_t screenIndex);
      bool      setActive(AFScreen* screen);
      bool      setActive(uint32_t screenId);
      AFScreen* getActive() const;


private:
      etl::vector<AFScreen*, MAX_SCREENS_PER_WORLD> m_screens;
      AFScreen*                                     m_active = nullptr;
};