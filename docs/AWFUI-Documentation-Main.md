## Welcome!

AWFUI (AwFui, pronounce awww fooey) is a lightweight, object oriented, embedded‑friendly UI framework originally built on top of **Adafruit_GFX**, which I was given for a class.  I quickly discovered the Adafruit library was pretty minimal, basically drawing primitives.  Having spent many years working in proprietary and open frameworks, I was dismayed.  Web searches offered nothing better.  So, AWFUI started as a framework with a button and a label and a plan to make it more complete.  

But not too complete - it is intended for embedded, so all additions are made with an eye toward keeping it small.  In the first release, it provides a consistent widget model, a screen/state system, dialogs and modal overlays, an event model with C callbacks, and images.  Pretty good.  It's a static library, so a good compiler will trim what you don't use, always thinking small.  There are abstractions for the screen and touch so that other hardware (TFT_eSPI?) could be supported (the idea is that any drawPixel() based libraries could be used).  It has a SDL based simulator to simplify development.

The goal is simple: a UI framework that feels familiar, stays tiny, avoids heavy dependencies. There are prettier frameworks out there, but they are larger and useful for build something amazing with a big step up in hardware.  I look at them with a little envy, but AWFUI is intend for a different environment.  The framework avoids heavy dependencies and is suitable for STM32, RP2040, ESP32, and similar MCUs. It is designed for RTOS environments but works without one.

Version 1 is for click and react widgets and intentionally lacks concepts like layered UI and widget focus. See the [Futures](AWFUI%20Documentation%20-%20Futures.md) document for what might come next.  References to these ideas in the headers is just early thinking out loud anticipating the future.

For now, enjoy.

**The Name**

AWFUI could stand for 'a widget-set for user interfaces', but that is forcing an acronym on some letters.  Because of the Adafruit connection it started as AFUI, but with the hardware abstraction layer, AWFUI is more appropriate.  Really, though, there is  just some humor in pronouncing it aw...fooey. 



## Overview

AWFUI is a collection of UI objects arranged in a familiar hierarchy.  If you've used any UI library, you will have a pretty good idea of how it works.  The key concepts are:

#### AWFUI Foundations and Containers

- **AFWorld** — a singleton that owns the runtime (e.g., the screen and it's interfaces), theme, and event loop. 
- **AFScreenList** — manages screens, including tracking the active one.
- **AFScreen** — Root container for one "page" of your UI. Owns widgets, panels (dialogs), and optional canvas. 

- **AFPanel** — a container widget that holds child widgets, draws a background, and routes events to its children.
- **AFModalDialog** — a panel that captures all input until dismissed. Inherits from AFPanel.
- **AFFullscreenDialog** — a modal that auto-sizes to fill the screen. Inherits from AFModalDialog.

```
AFWorld
 └── Theme
 └── AFScreenList
       └── AFScreen
             ├── Canvas	
             ├── Widgets
             └── Panels/Dialogs
                        └── Widgets
```

#### Widgets

Widgets are individual elements of a visual user interface. The can be for display, like a label, or interactive, like a check box.

​	AFWidget: base class for all UI elements

​		Widget implementations (e.g. AFPushButton, AFImageWidget)

#### Separation of Responsibilities

\- Application state → AFScreen

\- Overlays → AFPanel, AFModalDialog, AFFullscreenDialog

\- Elements → AFWidget and descendants



## Using AWFUI

### Minimal Example

Integrating and using AWFUI is straight forward.  The hardware is handed to the AFWorld, a screen is created as a base container for widgets and then loops.  This example shows heap created widgets that that AWFUI manages.  Widgets can also be created on the stack and managed by code scope.  Button events are handled by callback functions, in this example, that is by hooking them up to dialog methods.

```cpp
// Create hardware instances
Adafruit_ILI9341 tft;
AFFt6206Touch    touch;
AFDisplayAdafruitGFX display(tft);

AFWorld*       world;
AFScreen*    mainScreen;
AFModalDialog* dialog;

void setup() {
      // initialize hardware
      tft.begin();
      tft.setRotation(1);
      touch.begin();

      // then use the hardware to initialize the AFWorld
      AFWorld::init(display, &touch, &eventQueue);
      world = AFWorld::instance();

      // Create a screen to control the UI
      // More than one screen can be created, with one being active
      mainScreen = world->createScreen();

      // Create a button and add it to the the main screen to show the dialog.
      // Widgets can be addded to a screen or a dialog/panel.
      // the callback is assigned directly to a dialog method, but any callback function can be used
      AFButton* openBtn = new AFButton(120, 40, 160, 50, makeID("Open"), "Open Dialog");
      openBtn->setOnClickCallback([]() { dialog->show(*mainScreen); });
      mainScreen->addWidget(openBtn, true);

      // Creaate a "hello world-ish" modal dialog to display
      dialog = new AFModalDialog(20, 40, 200, 140, makeID("HDlg"));

      // Add a label for the "hello"
      AFLabel* lbl = new AFLabel(30, 60, "Hello from AFUI!", makeID("Helo"));
      dialog->addWidget(lbl, true);

      // Add the button to dismiss the dialog
      // as above, the callback is assigned to a dialog method, but any callback function can be used
      AFButton* okBtn = new AFButton(50, 100, 100, 40, makeID("OKBt"), "OK");
      okBtn->setOnClickCallback([]() { dialog->dismiss(); });
      dialog->addWidget(okBtn, true);
}

void loop() {
      // run the UI loop.
      // which "shows" the screen and controls the event loop,
      // which will get and process the touch for the button,
      // showing the modal dialog and eventually closing it with another touch.
      world->loop();
}

// Entry point for a desktop app as a simple example
// On actual hardware or RTOS, your startup/task code calls setup() and controls the loop()
int main() {
      setup();
      while (true) {
            loop();
      }
      return 0;
}
```

In practice, you would create the specific display adapter and pass it to AFWorld.  Touch and Event adapters are optional and provided if a project needs them, otherwise, pass null.  

### Using SDL for Desktop Prototyping

For development purposes, an SDL adapter is provided so that AWFUI can be used on the desktop.  To use it, SDL version 2  should be a sibling directory to AWFUI.

```
modules
 └── awfui
 └── SDL2
```

This must be SDL version 2.x, not the latest version 3.  In the CMAKE file, set 

```
option(AFUI_USE_SDL "Build with SDL backend for desktop simulation" ON)
```



## Memory

AWFUI was designed to avoid dynamic allocation. By default widgets are created and owned by the application and registered with screens or dialogs for use. AWFUI stores non‑owning pointers and never frees widget memory. Since AWFUI is focused on being an embedded library, this stack based, un-owned widgets default is reasonable.  Many embedded apps are static UIs that live for the lifetime of the process.  

**Screens** and **Panels** exist for the lifetime of the application. In this case, widgets added to a screen should also be long‑lived (often static or global).  **Dialogs** are temporary. Widgets declared (not allocated) inside a function and added to a dialog will be destroyed automatically by the function when the dialog goes out of scope. 

**Widget Allocation and Ownership**
Some projects would prefer a heap allocation model for widgets and then ownership becomes a concern.  AWFUI allows for this by opt-in ownership rules when a widget is added to a containing screen or dialog/panel.  

```
bool addWidget(AFWidget* w, bool owned = false);
```

The rule is: ownership transfers into the container on addWidget(..., true), and transfers back out on removeWidget(). If you never remove it, the container cleans it up.

While the ownership rules are intentionally simple, if you prefer to implement a more dynamic system, AWFUI should be compatible.  The m_owned flag doesn't fight smart pointers — it just makes them optional.

### **Memory Allocated per Container**

Because AWFUI uses ETL and not STL, there is a preallocated ETL list for each container.  Bt default this is 32 widget pointers × 4 bytes = 128 bytes per screen/panel, regardless of how many widgets are actually used.  If you are running in tight memory system, change the defaults in AFBase.h

### Strings in AWFUI 

Widgets like AFButton and AFLabel store `const char*` pointers — they do not copy the string. The caller must keep the string valid for the widget's lifetime.

**Safe:**

```cpp
button.setLabel("OK");                          // string literal — lives forever
static const char* kLabels[] = {"OK", "Cancel"};
button.setLabel(kLabels[0]);                    // static storage — fine
```

**Unsafe:**

```cpp
button.setLabel(String("OK").c_str());          // temporary — dangling pointer
```

For dynamic text (sensor readings, etc.), maintain a buffer that outlives the widget.

So, best practices are:

- ​    Use string literals where possible
- ​    Define constants at file/class scope: static const char* kButtonLabels[] = {"OK", "Cancel", "Apply"};
- ​    For dynamic text (like sensor readings), maintain a buffer that outlives the widget
- ​    Never pass temporary string objects

### Images

Images are embedded in the application code - there is no formal resource manager.  The image pixels are drawn on the screen directly from the in coded image in memory.



## Events

AWFUI uses an explicit event loop with no hidden blocking.   The AFWorld dispatches UI events to widgets, but dispatches non-UI events (says a button press) to the active screen for handling.

### AFEvent

Represents a hardware or logical event.

```cpp
enum class AFEventType { kNone, kTouchDown, kTouchUp, kTouchMove, kButton, kKey, kTimer };

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
```

### Event Flow

1. `AFWorld::loop()` polls hardware and builds an `AFEvent`.
2. The event is sent to the active screen via `AFScreen::handleEvent()`.
3. The screen decides where it goes:
   - If a modal dialog is active → the modal gets the event exclusively.
     - Otherwise → hit-test panels, then root-level widgets
   - If the event is a non-UI event (defined or custom) it is passed to the active screen.
4. The widget receives the event through virtual handlers: `onPress()`, `onRelease()`, `onClick()`.
5. If a callback is registered (e.g., `setOnClickCallback()`), it fires.
6. The screen receives the event through the virtual handlers: `onButton()`, onEvent().
7. Text edit widgets (will one day) receive the text event through the virtual handler `onKey()`.

### Callbacks

callbacks are plain C function pointers. No `std::function`, no lambdas, no allocations.

```cpp
void onOkClicked() {
    // handle it
}

okButton.setOnClickCallback(onOkClicked);
```



## Themes

`AFTheme` is a simple struct of shared colors and metrics. Set it once on `AFWorld` and all widgets pick it up.

```cpp
struct AFTheme {
      uint16_t screenBgColor;

      uint16_t widgetTextColor;
      uint16_t widgetBgColor;
      uint16_t widgetFgColor;
      uint16_t widgetDisabledFgColor;
      uint16_t widgetDisabledBgColor;
      uint16_t widgetAccentColor;
      uint16_t widgetBorderColor;
      uint8_t  widgetTextSize;
      uint8_t  smallTextSize;
      uint8_t  widgetPadding;
      uint8_t  widgetCornerRadius;

      uint16_t dialogBorderColor;
      uint8_t  dialogCornerRadius;
};
```

Theme value purpose should be indicated by the name.  Widgets that are disabled draw using `disabledWidgetFgColor` and `disabledWidgetBgColor` automatically.

Not all theme values are implemented in every theme but are provide for custom widgets.  The smallTextSize by default is the same widgetTextSize but exists if long info text is used and needs a different value.	



## Drawing

#### Two Modes

- **Direct mode** — used by widgets to draw straight to the display. Simple, no extra RAM.  This is currently the only mode
- **Canvas mode** — For screens, a hook is provide to draw the canvas before the widgets  are drawn on top.
  operation. Smoother, but costs a full-screen buffer of RAM.  A custom screen that needs the canvas should inherit from AFScreen.

Canvas mode is enabled per-screen when you create it via `AFWorld::createScreen(true)`.

#### Dirty Tracking

Each widget has a dirty flag. `AFScreen::draw()` only redraws widgets that are marked dirty. Calling `markDirty()` on a widget (or changing its state via setters) schedules it for the next draw pass.

AFScreen has an optional AFCanvas accessible.  If available, double buffering happens.
 ├── draws widgets into the canvas
 └── flushes canvas to the display

#### Canvases

An offscreen buffer can take a big chunk of memory.  If using the canvas, be aware that:

- One full-screen canvas at most, owned by AFScreen.

- Panels and widgets do not allocate canvases.

- AFFullscreenDialog reuses the screen's canvas.

- Screen transitions, if required, must be requested at startup to guaranteed use of an extra buffering, pre-allocated screen.

  

## C++ Use

AWFUI uses C++, but it does so in a most minimal way.  There are many valuable C++ language features, but they are not all compatible with generating a small embeddable binary.  Y

- **No STL** — ETL containers are used instead.
- **No exceptions** — functions return `bool`, `nullptr`, or an `AFResult` enum.
- **No RTTI** — disabled to save space.

You are free to use whatever you like in your own code, but be aware of these choices for compatibility.



## Using with an RTOS

`AFWorld::loop()` is designed to be called from a dedicated UI task. Keep these rules in mind:

- No blocking calls inside widgets or callbacks.

- Hardware input can be polled in the UI task, or posted into a queue that the UI task drains into `AFEvent` structs.

- Screen transitions and modal show/dismiss are synchronous from the UI task's perspective.

- Hardware input can be:	

  - Polled in UI task, or


  - Posted into a queue that UI task drains into AFEvents


  - AFWorld can be created without a touch interface but use the event queue to pass events from an touch task.  


    - AFEventQueue is lockable using lock/unlock methods.
    - An inheriting, RTOS-based class overrides these methods with a mutex, enabling the safe access to the queue by AFWorld (consuming with nextEvent()) and the RTOS task (adding with postEvent())
    
    ```
    class RTOSEventQueue : public AFEventQueue {
    protected:
        void lock() override   { /* acquire mutex */ }
        void unlock() override { /* release mutex */ }
    };
    ```



## Related Documents

- [Widgets](AWFUI%20Documentation%20-%20Widgets.md) — class-by-class reference
- [Futures](AWFUI%20Documentation%20-%20Futures.md) — what's missing and what might come next
- [Developing](AWFUI%20Documentation%20-%20Developing.md) — conventions for contributors
