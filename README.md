# AWFUI
AWFUI — 'A' lightweight 'W'idget-set 'F'or embedded 'UI' framework for C++.
MIT licensed. Designed for clarity, portability, and small systems.

AWFUI (think “awww… fooey”) is a lightweight, embedded‑friendly UI framework. Using intentionally minimal C++, it provides a consistent widget model, a screen/state system, dialogs and modal overlays, and a unified event model.

AWFUI uses a drawing and event abstraction layer to allow support for hardware backends.  It includes an SDL-based backend to simplify desktop development and rapid prototyping.  Currently it has been tested with AdafruitGFX (on STM32) and LovyanGFX (Arduino).

4/11/2026 update - I think it is complete.  Woot.  Offscreen canvases and DMA fills are rethought and implemented.  The second hardware backend (LovyanGFX) is functioning and demonstrates an offscreen canvas in use via Lovyan sprites.  The DMA is demonstrated using AdafruitGFX - mostly as example because I think their fillRect() already does this already (my board doesn't seem much faster).  So, complete for what I wanted it to do; though, of course, I'm not saying it's done-done or even bug free.

Documentation is in /docs.

​	[Main](./docs/AWFUI-Documentation-Main.md)  - an overview

​	[Widgets](docs/AWFUI-Documentation-widgets.md)  - widgets provided

​	[Developing](docs/AWFUI-Documentation-Developing.md) - some background if you want to contribute

[	Futures](docs/AWFUI-Documentation-Futures.md) - musings on where AWFUI could go



## (SDL desktop)

For desktop development (as in for doing layouts and getting your UI running), consider using the SDL backend. 
Note: SDL presets disable Adafruit backend by default.

### Prerequisites:
- CMake
- Ninja
- C++17 compiler
- SDL2 (either as `../SDL2` or installed on your system)

Tested C++ compilers include:
- Clang (Xcode Command Line Tools on Mac)
- GCC (MinGW on Windows)
- MSVC (Visual Studio on Windows)

### Build from a script   
From the `modules/awfui` directory:

#### Build on Mac
```sh
./buildAwfui.sh
```

#### Build on Windows
```sh
./buildAwfui.bat
```

Or build directly via presets:

```sh
cmake --preset sdl-ninja
cmake --build --preset sdl-ninja
```
