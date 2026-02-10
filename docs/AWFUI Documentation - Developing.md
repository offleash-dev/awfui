## Developing AWFUI

These are notes for anyone working on the framework itself. The guiding principle: **clarity, avoid code overhead and expansion, think embedded.**  AWFUI is a C++ framework, but it is using the language in a minimal way to follow that principle.  



### Project Structure

AWFUI lives in a `/modules` directory alongside its dependencies. The expected layout:

```
/modules
	/awfui      # this framework`
	/etl       	# Embedded Template Library
	/sdl       	# SDL2 (desktop simulator only)
	/drivers    # Adafruit, HAL, and other hardware support`
```

Application projects that use AWFUI are siblings or parents of `/modules` — not inside `/awfui`. The exception to this is the applications in the `/awfui/examples` directory which are example projects for demonstration and testing.



### C++ Usage

AWFUI uses C++ minimally. Features that add binary expansionare avoided:

- **No STL** — use [ETL](https://www.etlcpp.com/) containers instead.
- **No exceptions** — return `bool`, `nullptr`, or an `AFResult` enum. 
- **Asserts** — for programmer errors only.
- **No RTTI** — disabled to save space.



### Error Handling

Keep it simple and checkable:

- `bool` for success/failure
- `nullptr` for "nothing to return"
- `AFResult` enum for richer error codes
- `assert` only for conditions that indicate a bug, never for runtime errors



### Naming Conventions

AWFUI doesn't use C++ namespaces — it's small enough that the `AF` prefix handles it.

- **Classes:** `AF*` prefix (e.g., `AFButton`, `AFPanel`)
- **Members:** `m_*` (e.g., `m_visible`)
- **Statics:** `s_*` (e.g., `s_instance`)
- **Constants:** `k*` prefix, camelCase (e.g., `kAFImageFormat1bit`). Not ALL_CAPS.
- **Virtual overrides:** always use `override`

  
### Formatting

The code is the style guide. A few things worth calling out:

- **Braces:** K&R style.

- **Spacing:** 3 blank lines between functions in `.cpp` files, 2 blank lines between declarations in headers. The generous spacing makes individual methods easy to scan.

- **Include order:** C standard headers, then ETL/platform libraries, then `AF*` headers.

  

### Style Automation

`clang-format` is configured via `.clang-format` at the project root. It handles most formatting, but doesn't manage inter-function spacing well. A helper script does both:

```
formatProjectFiles.sh    # or .ps1
```

It runs `clang-format` and then fixes the spacing. Run it occasionally.

## String Lifetime

String lifetime management is the application's responsibility.

Widgets store `const char*` pointers — they do not copy strings. The caller owns the string and must keep it valid for the widget's lifetime.

**Safe:**

```cpp
button.setLabel("OK");                              // literal — lives forever
static const char* kLabels[] = {"OK", "Cancel"};
button.setLabel(kLabels[0]);                        // static — fine
```

**Unsafe:**

```cpp
button.setLabel(String("OK").c_str());              // temporary — dangling pointer
```

For dynamic text, maintain a buffer that outlives the widget. This is covered in the main documentation as well, but it's important enough to repeat here.

So, best practices are:
    Use string literals where possible
    Define constants at file/class scope: static const char* kButtonLabels[] = {"OK", "Cancel", "Apply"};
    For dynamic text (like sensor readings), maintain a buffer that outlives the widget
    Never pass temporary string objects



### Design Patterns

- **Factories** are used where the framework needs to track ownership. `AFWorld::createScreen()` is a factory because the world needs to manage the screen list.
- **Panels**, on the other hand, do not use a factory for child widgets — you create widgets yourself and add them. This keeps the framework open to new widget types without modification.
- **Singletons** are used sparingly. `AFWorld` is the only one.
- **Callbacks** are plain C function pointers. No `std::function`, no lambdas with captures.



### Other Conventions

- Consistent geometry: all widgets use upper-left origin.
- Single-canvas model: at most one full-screen canvas per screen.
- Argument order in widget constructors: `x, y, w, h, id, ...` (or `x, y, ...` for widgets that derive size from content).
- Follow existing patterns when adding new widgets. Read a few existing widget implementations before writing a new one.



### See Also

- [Futures](AWFUI%20Documentation%20-%20Futures.md) — what's missing and what might come next