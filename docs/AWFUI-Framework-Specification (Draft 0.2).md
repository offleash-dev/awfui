# AWFUI Framework Specification (Draft 0.2)



## 1. Purpose

AWFUI (AwFui, awww fooey) is a lightweight, embedded‑friendly UI framework built on top of **Adafruit_GFX**. It provides a consistent widget model, a screen/state system, dialogs and modal overlays, a unified event model, and optional double‑buffering via a full‑screen canvas for a screen.  

Version 1 is for click and react widgets and intentionally lacks concepts like layered UI and widget focus.  Maybe version 2 adds those.

The framework avoids heavy dependencies, uses **no STL** (ETL only), avoids exceptions, and is suitable for STM32, RP2040, ESP32, and similar MCUs. It is designed for RTOS environments but works without one.



## 2. Design Principles

- **Clarity and minimalism** — no extra junk; think embedded.

- **No STL** — use ETL containers.

- **No exceptions** — return `bool`, `nullptr`, or an `AFResult` enum.

- **Consistent geometry** — all widgets use upper‑left origin.

- **Single‑canvas model** — at most one full‑screen canvas per screen.

- **Explicit event loop** — no hidden blocking.

- **Deterministic rendering** — full redraw initially; dirty‑rect later.

  ### Style Conventions

- **No namespaces** — contstructs simply prefixed with `AF`.

- **K&R braces**,

- **Identifiable Object Names** 

  - `m_` for members, `s_` for statics, `k*` for constants.

- **Readable spacing** — 3 blank lines between major definition blocks.

- **clang-format** + post‑processing script for spacing.



## 3. Architecture Overview

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



## 4. Core Components

### 4.1 AFWorld

Top‑level runtime and orchestrator.

**Responsibilities**

- Owns global `AFTheme`
- Owns `AFScreenList`
- Holds references to hardware interfaces (display, touch, buttons)
- Runs the event loop
- Polls hardware and builds `AFEvent`
- Dispatches events to active screen
- Triggers redraws

**Key API**

```
void setTheme(const AFTheme& theme);
bool addScreen(AFScreen* screen);
void setActiveScreen(AFScreen* screen);
void loop();   // called from main or RTOS task
```



### 4.2 AFScreenList

Used by AFWorld to manage application screens.

**Responsibilities**

- Stores screens
- Tracks active screen
- Handles screen switching
- Planned transition support (fade, slide, etc.)

**Key API**

```
bool add(AFScreen* screen);
void remove(AFScreen* screen);
void setActive(AFScreen* screen);
AFScreen* getActive() const;
```



### 4.3 AFScreen

Represents a top‑level UI container.

**Responsibilities**

- Owns root level widgets and dialogs
- Owns optional full‑screen canvas (`GFXcanvas16`)
- Handles hit‑testing and event dispatch
- Handles modal blocking
- Draws itself and children

**Properties**

```
Adafruit_GFX& display;
GFXcanvas16* canvas;     // nullable
etl::vector<AFWidget*> widgets;
etl::vector<AFDialog*> dialogs;
AFModalDialog* activeModal;  // nullable
```

**Key API**

```
void addWidget(AFWidget* w);
void addDialog(AFDialog* d);
void showModal(AFModalDialog* d);
void dismissModal(AFModalDialog* d);
void handleEvent(const AFEvent& e);
void draw();
```



### 4.4 AFWidget (base class)

Base class for all visible UI elements.

**Geometry**

```
int16_t x, y;
int16_t width, height;
bool visible;
AFWidget* parent;
```

**Responsibilities**

- Draw itself
- Hit‑test
- Receive events
- Optionally request redraw

**Virtual API**

```
virtual void draw(Adafruit_GFX& gfx) = 0;
virtual bool hitTest(int16_t px, int16_t py) const;
virtual void onPress(const AFEvent& e);
virtual void onRelease(const AFEvent& e);
virtual void onClick(const AFEvent& e);
virtual void onKey(const AFEvent& e);
virtual void onButton(const AFEvent& e);
```



### 4.5 AFDialog (inherits AFWidget)

Container widget.

**Responsibilities**

- Owns child widgets
- Draws background/border
- Routes events to children

**Properties**

Code

```
etl::vector<AFWidget*> children;
```

**API**

```
void addChild(AFWidget* w);
void draw(Adafruit_GFX& gfx) override;
void handleEvent(const AFEvent& e);
```



### 4.6 AFModalDialog (inherits AFDialog)

A dialog that captures all input.

**Responsibilities**

- Blocks underlying widgets
- Must be explicitly dismissed

**API**

```
void show(AFScreen& screen);
void dismiss();
bool isModal() const;
```



### 4.7 AFFullscreenDialog (inherits AFModalDialog)

A modal overlay that fills the entire screen.

**Behavior**

- `x = 0, y = 0, width = screenWidth, height = screenHeight`
- No border/chrome by default
- Drawn into the screen’s canvas (no own canvas)
- Blocks input while active
- Ideal for menus, wizards, alerts



## 5. Event Model

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



## 6. Rendering Model



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

Initial version:

- Full redraw each frame

Future:

- Per‑widget `needsRedraw`
- Dirty‑rect tracking



## 7. RTOS Integration

- `AFWorld::loop()` runs in a dedicated UI task
- Hardware input may be polled or delivered via queue from ISR
- No blocking calls inside widgets
- Screen transitions and dialog operations are synchronous within UI task



## 8. Theme System

`AFTheme` defines:

- Colors
- Fonts
- Padding
- Corner radius
- Default widget styles

Stored in AFWorld and inherited by screens/widgets unless overridden.



## 9. Memory Model

- Only one full‑screen canvas active at a time (owned by AFScreen)
- Dialogs and widgets do not allocate canvases
- AFFullscreenDialog reuses the screen’s canvas
- Screens are long‑lived; dialogs/widgets may be dynamic
- Screen transitions, if required, must be requested at startup to guaranteed use of an extra buffering, pre-allocated screen.   

## 10. Widgets

The initial widgets are:

- AFButton, AFLabel
-  AFCheckbox, AFRadioButton
- AFImageWidget
- AFImageButton
- Additional widgets possible with the initial architecture:   AFSpinButton,, AFMenu, , AFProgressBar



## 11. Future Extensions (from noodling)

These are not part of the 1.0 effort but are anticipated:

- AFPanel (layout container)

- AFLayout / AFPixelLayout / AFAutoLayout

- Screen transitions (slide, fade, zoom)

- Background dimming for dialogs

- Dirty‑rect rendering

- Zero‑heap mode

  

Future versions would have:

- Focus management, to support
- AFTextEdit, implying the need for
- AFWidgetList
- AFContainerList  (for layered AFDialogs)

But first we need to get to 1.0