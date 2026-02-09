## Core Components

### AFWorld

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



### AFScreenList

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



### AFWidget (base class)

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



### AFDialog (inherits AFWidget)

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



### AFModalDialog (inherits AFDialog)

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



### AFFullscreenDialog (inherits AFModalDialog)

A modal overlay that fills the entire screen.

**Behavior**

- `x = 0, y = 0, width = screenWidth, height = screenHeight`
- No border/chrome by default
- Drawn into the screen’s canvas (no own canvas)
- Blocks input while active
- Ideal for menus, wizards, alerts



Widgets

AFLabel

AFPushButton

AFCheckbox

AFRadioButton/AFRadioButtonGroup

AFImageButton

AFImageWidget