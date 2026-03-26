#pragma once

//// AFIdConfig.h
//// Part of the AWFUI library
////
//// ID configuration options and helpers for AWFUI framework.
//// Developers should choose ONE ID strategy based on their needs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


// ID STRATEGY CONFIGURATION
// IDs can be used for identifying widgets in event handling, debugging, and other operations. 
// Being uint32_t values per AWFUI object, they come at a cost, so, for memory tight 
// situations, IDs can be disabled; though, you have to use object addresses individual 
// callbacks or other means to identify widgets.  

// The awfui APIs accept IDs as uint32_t in either case, the ID value is just discarded
// and 0 is returned for any ID query.



// Set this to 1 to disable IDs
#define AWFUI_DISABLE_IDS 0

#if AWFUI_DISABLE_IDS
#warning "AWFUI: Widget IDs are disabled. All widgets will report AFUI_ID_NONE."
#endif

#define AFUI_ID_NONE 0u



// Helpers to create 4-character IDs used for widgets, screens, dialogs, etc.

// Example: uint32_t myID = MAKE_ID('T','E','S','T');
#define MAKE_ID(a,b,c,d) \
    ( ((uint32_t)(a) << 24) | \
      ((uint32_t)(b) << 16) | \
      ((uint32_t)(c) << 8)  | \
      ((uint32_t)(d) << 0) )



// Example: uint32_t myID = MAKE_ID_FROM_STR("TEST");
#define MAKE_ID_FROM_STR(str) \
      ((uint32_t)(str[0])<<24 | \
       (uint32_t)(str[1])<<16 | \
       (uint32_t)(str[2])<<8 | \
       (uint32_t)(str[3]))



// Example: char idOut5[5];
//         getIdAsChars(MAKE_ID_FROM_STR("TEST"), idOut5);
// will fill idOut5 with {'T', 'E', 'S', 'T', '\0'}
inline void getIdAsChars(uint32_t id, char* out5) {
#if AWFUI_DISABLE_IDS
    out5[0] = '\0'; // All IDs are disabled, return empty string
#else
    out5[0] = (char)((id >> 24) & 0xFF);
    out5[1] = (char)((id >> 16) & 0xFF);
    out5[2] = (char)((id >> 8)  & 0xFF);
    out5[3] = (char)((id >> 0)  & 0xFF);
    out5[4] = '\0';
#endif
}



// Example:
//         getIdByte(MAKE_ID_FROM_STR("TEST"), 2);
// will return the ASCII code for 'S' 
inline uint8_t getIdByte(uint32_t id, int n) {
    return (uint8_t)((id >> (24 - (n * 8))) & 0xFF);
}



// Example: uint32_t myID = MAKE_ID_FROM_STR("TEST");
//          setIdByte(myID, 2, 'X');
// will change myID to represent "TEXT"
inline void setIdByte(uint32_t& id, int n, uint8_t value) {
    id &= ~(0xFFu << (24 - (n * 8)));
    id |= ((uint32_t)value << (24 - (n * 8)));
}
