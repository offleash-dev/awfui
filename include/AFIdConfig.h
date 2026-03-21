#pragma once

//// AFConfig.h
//// Part of the AWFUI library
////
//// Configuration options for AWFUI framework.
//// Developers should choose ONE ID strategy based on their needs.
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


// ID STRATEGY CONFIGURATION
// IDs are used for identifying widgets in event handling and other operations. 
//The choice of ID strategy can impact memory usage and ease of development.
// 
// Choose ONE of the following ID strategies:
//
// USE_NO_IDS:        No IDs at all - saves 4 bytes per widget
// USE_UINT32_IDS:    4-byte unsigned int IDs - current default approach
// USE_C_STRING_IDS:  C-string pointer IDs - human-readable but incurs string cost
//
// by uncommenting ONE of these or, better, defining AWFUI_ID_STRATEGY in your build system 
// (e.g. -DAWFUI_ID_STRATEGY=0) to avoid hardcoding in the header file.:

//#define USE_NO_IDS        0         // Most memory efficient - no ID storage
//#define USE_UINT32_IDS    1         // 4-byte unsigned int IDs - fast numeric
//#define USE_C_STRING_IDS  2         // C-string pointers - human-readable

// COMPILATION VALIDATION
//
#if !defined(USE_NO_IDS) && !defined(USE_UINT32_IDS) && !defined(USE_C_STRING_IDS)
#define AWFUI_ID_STRATEGY 2  // Default to C-string IDs if none defined
#endif

#if (AWFUI_ID_STRATEGY == 0)
    #define USE_NO_IDS
#elif (AWFUI_ID_STRATEGY == 1)
     #define USE_UINT32_IDS
#elif (AWFUI_ID_STRATEGY == 2)
     #define USE_C_STRING_IDS
#else
    #define USE_C_STRING_IDS  // Default fallback
#endif



// MEMORY USAGE IMPACT
// Strategy            | Memory per Widget | Use Case
// --------------------|-------------------|-------------------
// USE_NO_IDS          | 0 bytes           | Simple UIs, no widget identification needed
// USE_UINT32_IDS      | 4 bytes           | Performance-critical UIs, numeric IDs
// USE_C_STRING_IDS    | 4 bytes + strings | Development/debugging, readable names



// UNIFIED ID TYPE DEFINITION
//
#if defined(USE_NO_IDS)
    typedef int ID_TYPE;              // int but parameter is ignored
    #define ID_TYPE_NAME "none"
#elif defined(USE_UINT32_IDS)
    typedef uint32_t ID_TYPE;
    #define ID_TYPE_NAME "uint32"
#elif defined(USE_C_STRING_IDS)
    typedef const char* ID_TYPE;
    #define ID_TYPE_NAME "cstring"
#endif



// HELPER MACROS
// 
#if defined(USE_NO_IDS)
    #define storeID(id) (0)            // Don't store the ID
#elif defined(USE_UINT32_IDS)
    #include <string>
    static uint32_t g_nextWidgetID = 1000;  // Start from 1000 to avoid conflicts
    #define storeID(id) (id)             // Store the ID as-is
#elif defined(USE_C_STRING_IDS)
    #define storeID(id) (id)             // Store the ID as-is
#endif



// Helpers to create 4-character IDs used for widgets, screens, dialogs, etc.
// Example: constexpr uint32_t myID = MAKE_ID('T','E','S','T');
#define MAKE_UINT32_ID(a,b,c,d) \ ((uint32_t)(a)<<24 | (uint32_t)(b)<<16 | (uint32_t)(c)<<8 | (uint32_t)(d))



// Example: constexpr uint32_t myID = makeID("TEST");
#if defined(USE_NO_IDS) || defined(USE_UINT32_IDS)
constexpr uint32_t makeID(const char (&s)[5]) {
#if defined(USE_NO_IDS)
    return 0;
#elif defined(USE_UINT32_IDS)
    return (uint32_t(s[0]) << 24) |
           (uint32_t(s[1]) << 16) |
           (uint32_t(s[2]) << 8)  |
           (uint32_t(s[3]));
#endif
}
#elif defined(USE_C_STRING_IDS)
constexpr const char* makeID(const char* s) {
    return s;
}
#else
#endif



// Example: char idOut5[5];
//         getAsChars(makeID("TEST"), idOut5);
// will fill idOut5 with {'T', 'E', 'S', 'T', '\0'}
inline void getAsChars(int32_t id, char* out5) {
#if defined(USE_NO_IDS)
    out5[0] = 0;
    out5[1] = 0;
    out5[2] = 0;
    out5[3] = 0;
    out5[4] = '\0';
#elif defined(USE_UINT32_IDS)
    out5[0] = (char)((id >> 24) & 0xFF);
    out5[1] = (char)((id >> 16) & 0xFF);
    out5[2] = (char)((id >> 8)  & 0xFF);
    out5[3] = (char)((id >> 0)  & 0xFF);
    out5[4] = '\0';
#elif defined(USE_C_STRING_IDS)
    // Already a string, do nothing
#endif
}
