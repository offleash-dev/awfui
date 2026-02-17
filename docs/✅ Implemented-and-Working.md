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
| AFModalDialog     | Blocks input, explicit dismiss                 | ✅ Done                                  |
| AFTheme           | Colors/fonts inherited by widgets              | ✅ Done                                  |
| Hardware buttons  | onButton() event type                          | ✅ Done, injected                        |

External Events AF

------

## ⚠️ Implemented but Untested/Incomplete

| Component          | Spec                                        | Status                  |
| :----------------- | :------------------------------------------ | :---------------------- |
| AFPanel            | Container widget, routes events to children | ⚠️ Code exists, untested |
| AFFullscreenDialog | Full-screen modal                           | ⚠️ Code exists, untested |

------

## ❌ Not Yet Implemented

These are not implemented yet.  Canvas mode stubbed in because I think it has value.  Screen transitions are a fun idea but the board has to be fast enough to support them.  The other stuff is triggered by brainstorming event types and might be relevant or not.  Hardware buttons should be some kind of external event mechanism, not directly in AWFUI.  Same thing for timers, probably.   Keyboard support is different but basically out of scope for the first release.

| Component          | Spec                                   | Status                                      |
| :----------------- | :------------------------------------- | :------------------------------------------ |
| Canvas mode        | Off-screen buffer for smooth rendering | ⚠️ Structure exists, untested                |
| Keyboard/keys      | onKey() event type, injected           | ❌ Event type exists,  injected, not polling |
| Timer events       | kTimer event type                      | ❌ Not implemented                           |
| Screen transitions | Fade, slide (spec says "planned")      | ❌ Future                                    |

------

## Summary

Core framework: ~85% complete

The main gaps are:

1. Dialog testing - code exists but unused
1. Hardware buttons/timers - event types ready, polling not implemented

The dirty-flag optimization you asked for is actually ahead of the spec (listed as "future").