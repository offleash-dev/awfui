#pragma once

// AFScreenList.h
// Manages a list of AFScreen objects for the AFWorld of the AWFUI library

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
