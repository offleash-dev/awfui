- # AWFUI Futures

  Version 1 is click-and-react. This document acknowledges what's missing, sometimes why, and sketches what might come next. Additions will always be weighed against the lightweight, embedded-first goals of the framework.  They might be integrated or made to add-in as needed

  See also [Developing](AWFUI%20Documentation%20-%20Developing.md) for contributor guidance.


  ## Widgets That Could Be Added

  These fit within the current architecture without major changes:

  - **AFSpinButton** — up/down arrows for numeric input. Simple to build on AFButton.
  - **AFMenu** — a popup offering text choices. Likely a modal panel with a list of items.
  - **AFProgressBar** — visual feedback for long operations. Could be a determinate bar or an indeterminate animation (the "cylon eye" approach avoids the need for accurate percentages).
  - **Multiline / text-wrapping labels** — `AFLabel` is single-line only. Wrapping or multiline support would help for longer informational text.
  - **Checkbox checkmark** — currently draws an "X". A proper checkmark glyph would look more polished.
  - **AFList** — a scrollable list of items. Implies scroll tracking and a scrollbar widget.
  - **Scroll container** — no way to scroll lists or long content. Would require a clipping region and scroll tracking.


  ## Architectural Extensions

  These require deeper changes and are intentionally deferred:

  #### Focus and TextEdit

  - **Focus management** — Version 1 has no concept of an "active" widget that receives keyboard or button input. Adding focus means new events (`kGetFocus`, `kLoseFocus`), a focus chain, and tab-order tracking. It's a prerequisite for text editing and non-touch input.
  - **AFTextEdit** — text input needs focus, a cursor, and keyboard events. A significant addition.
  - **AFWidgetList** — if focus is added, containers will need a managed widget list to track focus order and state. Currently, containers use a simple `AFVector` of pointers.
  - **Encoder / D-pad navigation** — non-touch input devices need focus traversal. Requires a tab-order or spatial navigation scheme.

  #### Rendering

  - **Screen transitions** — screens can already be switched. Adding animated transitions (slide, fade, zoom) would make the experience more polished but isn't essential for embedded utility UIs.
  - **Dirty-rect canvas blit** — when using a canvas, the entire buffer is blitted every frame even if only one widget changed. Tracking the bounding box of dirty widgets and blitting only that sub-rect would reduce SPI traffic significantly (e.g., 150KB → a few KB on a 320×240 display).
  - **Background dimming for modals** — a visual nicety. Drawing a semi-transparent overlay behind a modal would require blending support or a pre-rendered dim buffer.
  - **Panel layering / z-order** — the `m_zOrder` field exists on AFPanel but is unused. Enabling it would allow stacked panels with proper ordering, but the use case on small screens is limited. This might require some type of AFDialogList.

  #### Events
- m_eventMask added to widgets to optimize event delivery, e.g. for text events

  - **`onExternalEvent()` routing to panels/dialogs** — currently screen-level only. If a panel or dialog needs to handle a custom event (kButton, kKey, kCustom), the screen subclass must manually route it. Consider propagating non-positional events down the widget tree.

#### Backend Packaging

**Legacy Adafruit_GFX support.** Older versions of Adafruit_GFX (e.g., the stripped-down version used in some coursework) are missing `getTextBounds()`, `drawRGBBitmap()`, and the `Print` base class. A legacy backend (`AFDisplayAdafruitGFXLegacy`) could fill these gaps: `getTextBounds` via manual character-width calculation, `drawRGBBitmap` via a pixel-by-pixel loop, and `print` via repeated `drawChar` calls. The rest of the API surface is identical between old and new versions.

  **TFT_eSPI support** should be possible.  Not explored yet.


  ## Longer-Term / Nice-to-Have

  - **Font management** — currently relies on Adafruit_GFX default font or a basic font in the SDL display. Supporting custom fonts, font selection per widget, or scaled fonts would improve visual quality.
  - **Internationalization / UTF-8** — no multi-byte character support. Would require a different text rendering path.
  - **Animations** — widget-level animations (fade, slide, pulse) for feedback and polish.


  ## Extension by Script 

  - **Layout** — Layout as a dynamic part of the AWFUI library is not planned. Instead, layout would be provided by a script to generate a C function that builds your UI for inclusion in your project.
  - **UI-from-data / serialization** — defining screens from a data format (JSON, binary) rather than code. Useful for tools-generated UIs.

    
