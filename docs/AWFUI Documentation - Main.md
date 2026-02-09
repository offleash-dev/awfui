## Welcome!

AWFUI (AwFui, pronounce awww fooey) is a lightweight, object oriented, embedded‑friendly UI framework originally built on top of **Adafruit_GFX**, which I was given for a class.  I soon discovered the Adafruit library was pretty minimal, basically drawing primitives.  I spent many years working in proprietary and open frameworks and was dismayed.  Web searches offered nothing better.  So, AWFUI started as a framework with a button and a label with a plan to make it more complete.  But not too complete - it is intended for embedded, so all additions are made with an eye toward keeping it small.  In the first release, it provides a consistent widget model, a screen/state system, dialogs and modal overlays, an event model with C callbacks, and images.  Pretty good.  It's a static library, so a good compiler will trim what you don't use, always thinking small.  There are abstractions for the screen and touch so that other hardware (TFT_eSPI?) could be supported (the idea is that any drawPixel() based libraries could be used).  

There are prettier frameworks out there, but they are larger and you would probably build something beautiful with them using a big step up in hardware.  I look at them with a little envy, but AWFUI is intend for a different environment.  The framework avoids heavy dependencies and is suitable for STM32, RP2040, ESP32, and similar MCUs. It is designed for RTOS environments but works without one. It will hopefully help you get started in embedded and RTOS, allowing you to have pleasant UI development experience.

Version 1 is for click and react widgets and intentionally lacks concepts like layered UI and widget focus.  Maybe version 2 adds those.  Same thing for double‑buffering via a full‑screen canvas for a screen, which could reduce flash.  It would be cool to have some UI transitions so that it could be used with a game.  You might see references to these ideas in the headers. That was me thinking out loud, planning (and hopefully accommodating) the future.

For now, enjoy.

**The Name**

AWFUI could, I guess, stand for 'a widget-set for user interfaces'.  Because of the Adafruit connection it started as AFUI, but with the hardware abstraction layer, AWFUI is more appropriate.  Really, though, I found some humor in pronouncing it aw...fooey. 

## Overview

AWFUI is a collection of UI objects.  If you've used any UI library, you will have a pretty good idea of how it works.  The key concepts are:

AWFUI Foundations and Widgets

​	Consistent coordinates: all widgets use upper‑left origin.  Rotation is supported.

	Foundations: 
		AFWorld: top‑level runtime and event loop

​	AFScreenList: manages screens and active screen selection

​	AFScreen: root of a widget tree for a given app state

Widgets

​	AFWidget: base class for all UI elements

​	AFDialog / AFModalDialog / AFFullscreenDialog: container widgets with different behavior

```
AFWorld
 └── Theme
 └── AFScreenList
       └── AFScreen
             ├── Canvas	
             └── Widgets
                   ├── Dialogs
                        └── Widgets
                   ├── Modal Dialog (optional)
                   └── Fullscreen Dialog (optional)
```

AFWorld owns the runtime, theme, and event loop. 

AFScreenList manages screens, which are containers for your UI.

AFScreen owns widgets, dialogs, and optional canvas. 

AFDialogs and AFWidgets form a tree of UI elements.

There is a clear separation of:

​	Application state (AFScreen)

​	Overlays (AFDialog, AFModalDialog, AFFullscreenDialog)

​	Elements (AFWidget and descendants)

## Memory

Strings in AWFUI 
String lifetime management is the application's responsibility.

AFButton, AFLabel, and similar widgets store const char* pointers. This means:
    The widget doesn't own the string
    The string must outlive the widget
    The caller is responsible for keeping the string valid

So, best practices are:
    Use string literals where possible
    Define constants at file/class scope: static const char* kButtonLabels[] = {"OK", "Cancel", "Apply"};
    For dynamic text (like sensor readings), maintain a buffer that outlives the widget
    Never pass temporary string objects

Again, string lifetime management is the application's responsibility.



AWFUI does not manage memory for widgets. Instead, widgets are typically created on the stack and registered with a screen or dialog. Screens are long‑lived objects, while dialogs are usually temporary.

- **Screens** persist for the lifetime of the application. Widgets added to a screen should therefore also be long‑lived (often static or global).
- **Dialogs** are temporary. Widgets created inside a function and added to a dialog will be destroyed automatically when the dialog goes out of scope.

AWFUI stores **non‑owning pointers** to widgets. It never allocates or frees widget memory. Lifetime is entirely controlled by normal C++ scope rules.

This design avoids dynamic allocation, garbage collection, and complex ownership models, making AWFUI predictable and safe for embedded systems.

## Events

	4.2 Flow
AFWorld polls hardware → builds AFEvent

AFWorld → activeScreen->handleEvent(e)

AFScreen:

If activeModal → send to modal

Else → hit‑test dialogs/widgets

AFWidget receives event via onPress/onRelease/onClick/...



## Themes

​	shared colors, fonts, spacing, etc.

## 5. Event Model

Explicit event loop** — no hidden blocking.

AFScreen fills the whole screen.
 └── recieves and dispatches from the event loop

 All events originate at the event loop and are dispatched to the current screen:
    Touch events
    Hardware button events
    Keyboard events (future)
    And the AFScreen decides:
        If a modal dialog is active → send events there
        Otherwise → hit-test widgets
        Otherwise → background behavior

        Hardware event
        ↓
        AFScreen (root)
        ↓
        Active modal dialog? → Yes → send to modal
                            → No  → hit-test children
        ↓
        AFWidget::handleEvent()
        ↓
        onPress / onRelease / onClick / onKey / onButton

### 5.1 AFEvent

Represents a hardware or logical event.

**Fields**

```
enum class Type { TouchDown, TouchUp, TouchMove, Button, Key, Timer };

Type type;
int16_t x, y;        // touch
uint8_t buttonId;    // hardware button
uint16_t keycode;    // keyboard (future)
uint32_t timestamp;
```



### 5.2 Event Flow

1. **AFWorld** polls hardware → builds `AFEvent`
2. AFWorld → `activeScreen->handleEvent(e)`
3. **AFScreen** dispatch:
   - If modal dialog active → send to modal
   - Else → hit‑test dialogs
   - Else → hit‑test widgets
4. **AFWidget** receives event via virtual handlers
5. host receives event via supplied (e. onClick)

## Drawing

AFScreen fills the whole screen. AFScreen has an optional AFCanvas accessible.  If available, double buffering happens.
 ├── owns widgets (AFButton, AFLabel, etc.) and/or a dialog  (has a AFWidgetList)
 ├── draws widgets into the canvas (or direct to screen)
 └── flushes canvas to the display

### 6.1 AFScreen::draw()

AFScreen has two  drawing modes:

**Canvas mode**

- An offscreen buffer for smooth rendering
- Clear canvas
- Draw widgets/dialogs into canvas
- Flush canvas to display

**Direct mode**

- Draw widgets/dialogs directly to display



### 6.2 Redraw Strategy

**Deterministic rendering** — full redraw initially; dirty‑rect later.

**Single‑canvas model** — at most one full‑screen canvas per screen.

Initial version:

- Full redraw each frame

Future:

- Per‑widget `needsRedraw`

- Dirty‑rect tracking

  

AFScreen::draw():

If canvas:

Clear canvas

Draw widgets/dialogs into canvas

Flush canvas to display

Else:

Draw widgets/dialogs directly to display

Optional:

Dirty‑rect tracking later

Simple “needsRedraw” flag per widget/screen initially



## Using AWFUI

C++ Use

AWFUI uses C++, but it does so in a most minimal way.  There are many valuable C++ language features, but they are not all compatible with generating a small embeddable binary.  You can choose to disregard AWFUI's choices in your own code, but you should be aware for compatibility that AWFUI follows these two rules to reduce compiled size

- **No STL** — use ETL containers.
- **No exceptions** — return `bool`, `nullptr`, or an `AFResult` enum.


​	Memory Model

- Only one full‑screen canvas active at a time (owned by AFScreen)
- Dialogs and widgets do not allocate canvases
- AFFullscreenDialog reuses the screen’s canvas
- Screens are long‑lived; dialogs/widgets may be dynamic
- Screen transitions, if required, must be requested at startup to guaranteed use of an extra buffering, pre-allocated screen.

Examples

​	

With and RTOS

​		RTOS‑friendly: explicit event loop, no hidden blocking

6. RTOS considerations
AFWorld::loop() is called from a dedicated UI task

​	No blocking calls inside widgets

​	Hardware input can be:	

​		Polled in UI task, or

​		Posted into a queue that UI task drains into AFEvents

​		Screen transitions and dialog show/dismiss are synchronous from the UI task’s perspective
