//// AFScreenList.cpp
//// Implementation of the AWFUI screen list management.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include "AFScreenList.h"
#include "AFScreen.h"



// Add a screen to the list
//
void AFScreenList::add(AFScreen* screen) {
      if (!m_screens.full()) {
            m_screens.push_back(screen);

            // If this is the first screen added, make it active
            if (m_active == nullptr) {
                  m_active = screen;
            }
      }
      // If full, silently ignore or assert depending on your style
}



// Remove a screen from the list
//
void AFScreenList::remove(AFScreen* screen) {
      for (size_t i = 0; i < m_screens.size(); ++i) {
            if (m_screens[i] == screen) {
                  m_screens.erase(m_screens.begin() + i);

                  // If we removed the active screen, clear active
                  if (m_active == screen) {
                        m_active = nullptr;

                        // Optionally: promote first remaining screen
                        if (!m_screens.empty()) {
                              m_active = m_screens.front();
                        }
                  }
                  return;
            }
      }
}



// Set active screen by pointer
//
bool AFScreenList::setActive(AFScreen* screen) {
      bool found = false;

      // Only allow active screens that are in the list
      for (auto* s : m_screens) {
            if (s == screen) {
                  m_active = screen;
                  found    = true;
                  break;
            }
      }

      return found;
}



// Set active screen by id
//
bool AFScreenList::setActive(uint32_t screenId) {
      bool found = false;

      // Only allow active screens that are in the list
      for (auto* s : m_screens) {
            if (s->getId() == screenId) {
                  m_active = s;
                  found    = true;
                  break;
            }
      }

      return found;
}



// Set active screen by index
//
bool AFScreenList::setActive(int16_t screenIndex) {
      bool found = false;

      if (screenIndex < 0 || screenIndex >= static_cast<int>(m_screens.size())) {
            return false; // invalid index
      }

      // Only allow active screens that are in the list

      AFScreen* screen = m_screens[screenIndex];
      for (auto* s : m_screens) {
            if (s == screen) {
                  m_active = screen;
                  found    = true;
                  break;
            }
      }

      return found;
}



// Get active screen
//
AFScreen* AFScreenList::getActive() const {
      return m_active;
}