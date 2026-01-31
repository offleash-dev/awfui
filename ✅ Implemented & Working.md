Comparison of Spec vs Implementation:

------

## ✅ Implemented & Working

| Component         | Spec                                           | Status                                  |
| :---------------- | :--------------------------------------------- | :-------------------------------------- |
| AFWorld           | Singleton, owns theme, screen list, event loop | ✅ Done - init(), instance(), loop()     |
| AFScreenList      | Stores/tracks screens, setActive               | ✅ Done                                  |
| AFScreen          | Owns widgets, dialogs, canvas, event dispatch  | ✅ Done                                  |
| AFWidget          | Base class, geometry, hitTest, events          | ✅ Done + dirty flag (ahead of spec)     |
| AFButton          | Clickable widget with callback                 | ✅ Done                                  |
| AFLabel           | Static text display                            | ✅ Done                                  |
| AFEvent           | TouchDown/Up/Move, Button, Key, Timer          | ✅ Done                                  |
| AFTouchInterface  | Abstract touch input                           | ✅ Done (not in spec, but good addition) |
| AFFt6206Touch     | FT6206 implementation with rotation            | ✅ Done                                  |
| Dirty flag redraw | Spec says "future"                             | ✅ Done early!                           |
| No STL/exceptions | ETL only, no exceptions                        | ✅ Done                                  |

------

## ⚠️ Implemented but Untested/Incomplete

| Component          | Spec                                        | Status                       |
| :----------------- | :------------------------------------------ | :--------------------------- |
| AFDialog           | Container widget, routes events to children | ⚠️ Code exists, untested      |
| AFModalDialog      | Blocks input, explicit dismiss              | ⚠️ Code exists, untested      |
| AFFullscreenDialog | Full-screen modal                           | ⚠️ Code exists, untested      |
| Canvas mode        | Off-screen buffer for smooth rendering      | ⚠️ Structure exists, untested |

------

## ❌ Not Yet Implemented

| Component          | Spec                              | Status                               |
| :----------------- | :-------------------------------- | :----------------------------------- |
| AFTheme usage      | Colors/fonts inherited by widgets | ❌ Defined but not wired into drawing |
| Hardware buttons   | onButton() event type             | ❌ Event type exists, no polling      |
| Keyboard/keys      | onKey() event type                | ❌ Event type exists, no polling      |
| Timer events       | kTimer event type                 | ❌ Not implemented                    |
| Screen transitions | Fade, slide (spec says "planned") | ❌ Future                             |

------

## Summary

Core framework: ~85% complete

The main gaps are:

1. Touch coordinate accuracy - needs verification

1. AFTheme - needs to be wired into widget drawing

1. Dialog testing - code exists but unused

1. Hardware buttons/timers - event types ready, polling not implemented

The dirty-flag optimization you asked for is actually ahead of the spec (listed as "future").