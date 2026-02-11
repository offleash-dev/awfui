# AWFUI Widgets

This document covers the core components and widgets in AWFUI. It describes purpose and behavior rather than exhaustive API — the headers are the definitive reference.


## Core Components

### AFWorld

Singleton that owns the runtime. Initialize once with `AFWorld::init()`, then access via `AFWorld::instance()`.

**Responsibilities:**

- Owns the global `AFTheme`
- Owns the `AFScreenList`
- Holds references to the display (`AFDisplayInterface`) and touch (`AFTouchInterface`)
- Runs the event loop (`loop()`)
- Polls hardware, builds `AFEvent` structs, and dispatches them to the active screen

**Key API:**

```cpp
static bool init(AFDisplayInterface& display, AFTouchInterface* touch = nullptr);
static AFWorld* instance();
void setTheme(const AFTheme& t);
AFScreen* createScreen(bool useCanvas = false);
void setActiveScreen(AFScreen* screen);
void loop();
```

`createScreen()` is a factory — AFWorld allocates and owns the screen internally.


### AFScreenList

Manages the collection of screens and tracks which one is active. Used internally by AFWorld.

**Key API:**

```cpp
void add(AFScreen* screen);
void remove(AFScreen* screen);
bool setActive(AFScreen* screen);
bool setActive(uint32_t screenId);
AFScreen* getActive() const;
```


### AFScreen

Top-level container representing one "page" of your UI. Each screen owns its root-level widgets, panels, and an optional offscreen canvas.

**Responsibilities:**

- Owns root-level widgets and panels
- Owns an optional full-screen canvas for double-buffered drawing
- Hit-tests and dispatches events
- Manages modal dialog blocking
- Draws itself and all children

**Key API:**

```cpp
void addWidget(AFWidget* w);
void addPanel(AFPanel* p);
void showModal(AFModalDialog* d);
void dismissModal(AFModalDialog* d);
void handleEvent(const AFEvent& e);
void draw();
```


### AFWidget

Abstract base class for all visible UI elements. Every widget has position, size, visibility, enabled state, and a dirty flag.

**Geometry:** upper-left origin. All coordinates are `int16_t`.

**Virtual API:**

```cpp
virtual void draw(AFDisplayInterface& gfx) = 0;
virtual bool hitTest(int16_t px, int16_t py) const;
virtual void onPress(const AFEvent& e);
virtual void onRelease(const AFEvent& e);
virtual void onClick(const AFEvent& e);
virtual void onKey(const AFEvent& e);
virtual void onButton(const AFEvent& e);
```

**State:** `setVisible()`, `setEnabled()`, and `markDirty()` all trigger a redraw on the next pass. Disabled widgets still draw (using theme disabled colors) but do not respond to input.


## Containers

### AFPanel

A container widget that holds child widgets. Draws an optional opaque background with border, then draws its children. Routes touch events to children via hit-testing.

Inherits from AFWidget.

**Key API:**

```cpp
bool addChild(AFWidget* w);
void removeChild(AFWidget* w);
AFWidget* childAt(int16_t px, int16_t py);
void handleEvent(const AFEvent& e);
```

**Note:** child widget coordinates are currently in screen space, not local to the panel.


### AFModalDialog

A panel that captures all input while active. The owning screen routes every event to the modal exclusively until it is dismissed.

Inherits from AFPanel.

**Key API:**

```cpp
void show(AFScreen& screen);
void show();       // convenience — shows on the active screen
void dismiss();
```

`show()` and `dismiss()` call `onShow()` and `onHide()` virtual hooks for setup/teardown.


### AFFullscreenDialog

A modal that automatically sizes itself to fill the entire screen when shown. No border or chrome by default. Ideal for menus, wizards, or alert overlays.

Inherits from AFModalDialog.


## Widgets

### AFLabel

Displays a text string. Comes in two forms:

- **Positioned label** — just x, y, and text. Width/height are implicit from the text.
- **Bounded label** — x, y, w, h, and text. Supports justification (left, center, right).

Text color defaults to the theme's `textColor` but can be overridden with `setColor()`.


### AFButton

A rectangular push button with a text label. Draws with theme colors and swaps to pressed colors on touch-down. Supports an `onClick` callback via `setOnClickCallback()`.

**Behavior:**

- `onPress()` — visual press state
- `onRelease()` — visual release
- `onClick()` — fires the callback

Colors can be customized per-button with `setColors()` and `setPressedColors()`, or left at theme defaults.

Disabled buttons draw with `disabledFgColor`/`disabledBgColor` and ignore input.


### AFCheckbox

A square checkbox with an optional text label. Toggles on click. Fires an `onChange` callback with the new checked state.

**Key API:**

```cpp
void setChecked(bool checked);
bool isChecked() const;
void toggle();
void setOnChangeCallback(AFCheckboxCallback cb);  // void (*)(bool checked)
```

Box size defaults from the theme if not specified.


### AFRadioButton / AFRadioButtonGroup

Radio buttons provide mutually exclusive selection within a group.

**AFRadioButton** draws a circle with a filled dot when selected. Each radio button is added to an **AFRadioButtonGroup**, which enforces exclusivity — selecting one deselects the others.

```cpp
AFRadioButtonGroup group;
group.addButton(&radio1);
group.addButton(&radio2);
group.setOnChangeCallback(onRadioChanged);  // void (*)(uint32_t id)
```

The callback receives the `id` of the newly selected button.


### AFImageWidget

Displays an `AFImage` as a non-interactive widget. Supports 1-bit and RGB565 image formats.

- **1-bit images** draw using the theme's `fgColor` (or `disabledFgColor` when disabled).
- **RGB565 images** draw as-is.

Width and height are taken from the image.


### AFImageButton

A button whose face is an image instead of text. Inherits from AFButton, so it gets press/release behavior and click callbacks for free.

- **1-bit images** swap to `disabledFgColor` when disabled.
- **RGB565 images** draw unchanged regardless of enabled state.


### AFImage

Not a widget — a plain data class that wraps image pixel data. Constructed from a byte array with a small header (width, height, format) followed by raw pixel data. See `AFImage.h` for the header format and an example.

```cpp
enum AFImageFormat { kAFImageFormatRGB565 = 0, kAFImageFormat1bit = 1 };
```

AFImages are drawn from memory.  No additional memory is used.



