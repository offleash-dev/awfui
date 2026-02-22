# AWFUI
AWFUI — A lightweight embedded UI framework for C++.
MIT licensed. Designed for clarity, portability, and small systems.

AWFUI (think “awww… fooey”) is a lightweight, embedded‑friendly UI framework. Using intentionally minimal C++, it provides a consistent widget model, a screen/state system, dialogs and modal overlays, and a unified event model.

While based on Adafruit_GFX, AWFUI uses drawing and event abstraction layer to allow support for other hardware backends.  It includes an SDL-based backend to simplify desktop development and rapid prototyping.

Documentation is in /docs.

​	[Main](./docs/AWFUI-Documentation-Main.md)  - an overview

​	[Widgets](docs/AWFUI-Documentation-widgets.md)  - widgets provided

​	[Developing](docs/AWFUI-Documentation-Developing.md) - some background if you want to contribute

[	Futures](docs/AWFUI-Documentation-Futures.md) - musings on where AWFUI could go



## macOS build (SDL desktop)

This repository uses CMake presets (`CMakePresets.json`). On macOS, the easiest path is the SDL desktop build.

Note: SDL presets disable Adafruit backend by default.

Prerequisites:

- CMake
- Ninja
- Clang (Xcode Command Line Tools)
- SDL2 (either vendored as `../SDL2` or installed on your system)

From the `modules/awfui` directory:

```sh
./buildAwfui.sh
```

If ETL is installed elsewhere on your machine, pass it via `ETL_INCLUDE_DIR`:

```sh
CMAKE_ARGS='-DETL_INCLUDE_DIR=/path/to/etl/include' ./buildAwfui.sh
```

Or directly via presets:

```sh
cmake --preset sdl-ninja
cmake --build --preset sdl-ninja
```
