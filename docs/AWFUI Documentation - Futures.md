## Possible Future Extensions

The initial release of AWFUI is, as state elsewhere, made of click and react widgets.  There is much more that could be added to a UI toolkit.  What is added will depend on demand, but always be done with the idea of keeping the focus on a the lightweight and embedded goals.  Additions might be integrated or made to add in as needed.

Additional widgets possible with the initial architecture:   

AFSpinButton - one of those widgets with up/down arrows for inputting a value

AFMenu - a popup menu offering text choices.

AFProgressBar - some mechanism for showing delays/progress.  These can be tricky for percentage progress, so maybe it's just a cylon eye.

AFList - like a menu, but in a list.  Of course, that means scrollbars, too.

AFPanel (layout container) - basically a container for organizing your massive embedded UIs.  Hmmm.  It could be a variant of AFDialog without chrome.  Probably has local coordinates (oh, then we need localToParent, localToScreen etc)

AFLayout - This has already been punted.  A script to generate a C function that builds your UI is possible, though.

Screen transitions (slide, fade, zoom) - there are already screens and they can be switch.  It would be amusing to see them switch with style.

Background dimming for dialogs - just an enhancement.  Again, we have screeens.

Dirty‑rect rendering - instead of the simple dirty widget mechanism, use a more refine approach to redraws to reduce load and flicker.

Focus management - AWFUI doesn't have widget focus, ie. there is no 'active' widget that receives input events.  That is needed for text editing (which would also imply keboard support) or custom button input events.   Focus means new events like kGetFocus and kLoseFocus.  

AFTextEdit - a way to type a value, which needs focus, cursors, and keyboard events.  

AFWidgetList - adding focus realistically means that AWFUI containers need a widget list to help manage the overall state.

AFDialogList - for layered AFDialogs, allowing z order, or panels on top and bottom of screen
