- # AWFUI Futures

  Version 1 is click-and-react. This document acknowledges what's missing, sometimes why, and sketches what might come next. Additions will always be weighed against the lightweight, embedded-first goals of the framework.  They might be integrated or made to add-in as needed

  See also [Developing](AWFUI%20Documentation%20-%20Developing.md) for contributor guidance.


  ## Widgets That Could Be Added

  These fit within the current architecture without major changes:

  - **AFSpinButton** — up/down arrows for numeric input. Simple to build on AFButton.
  - **AFMenu** — a popup offering text choices. Likely a modal panel with a list of items.
  - **AFProgressBar** — visual feedback for long operations. Could be a determinate bar or an indeterminate animation (the "cylon eye" approach avoids the need for accurate percentages).
  - **AFList** — a scrollable list of items. Implies scroll tracking and a scrollbar widget.


  ## Architectural Extensions

  These require deeper changes and are intentionally deferred:

  #### Focus and TextEdit

  - **Focus management** — Version 1 has no concept of an "active" widget that receives keyboard or button input. Adding focus means new events (`kGetFocus`, `kLoseFocus`), a focus chain, and tab-order tracking. It's a prerequisite for text editing and non-touch input.
  - **AFTextEdit** — text input needs focus, a cursor, and keyboard events. A significant addition.
  - **AFWidgetList** — if focus is added, containers will need a managed widget list to track focus order and state. Currently, containers use a simple `etl::vector` of pointers.

  #### Rendering

  - **Screen transitions** — screens can already be switched. Adding animated transitions (slide, fade, zoom) would make the experience more polished but isn't essential for embedded utility UIs.
  - **Dirty-rect rendering** — the current dirty-widget mechanism redraws entire widgets. A finer-grained dirty-rect system would reduce overdraw and flicker, but adds complexity.
  - **Background dimming for modals** — a visual nicety. Drawing a semi-transparent overlay behind a modal would require blending support or a pre-rendered dim buffer.
  - **Panel layering / z-order** — the `m_zOrder` field exists on AFPanel but is unused. Enabling it would allow stacked panels with proper ordering, but the use case on small screens is limited. This might require some type of AFDialogList.

#### Backend Packaging

**Legacy Adafruit_GFX support.** Older versions of Adafruit_GFX (e.g., the stripped-down version used in some coursework) are missing `getTextBounds()`, `drawRGBBitmap()`, and the `Print` base class. A legacy backend (`AFDisplayAdafruitGFXLegacy`) could fill these gaps: `getTextBounds` via manual character-width calculation, `drawRGBBitmap` via a pixel-by-pixel loop, and `print` via repeated `drawChar` calls. The rest of the API surface is identical between old and new versions.

  **TFT_eSPI support** should be possible.  No explored yet.

  ## Extension by Script 

  - **Layout** —Layout as a dynamic part of the the AF library is not planned.  Instead, layout would be provided by a script to generate a C function that builds your UI for inclusion in you project.

    
