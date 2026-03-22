#pragma once
//// AFContainer.h
//// Part of the AWFUI library
////
//// Interface for widget containers (screens and panels)
////
//// Copyright (c) 2026 Matt Foster
//// Licensed under the MIT License. See LICENSE file for details.


#include <stdint.h>
#include "AFVector.h"

#include "AFBase.h"

// Forward declarations
class AFWidget;
class AFPanel;



// Interface for widget containers
class AFContainer {
public:
    // Container management
    virtual bool addWidget(AFWidget* widget, bool owned = false) = 0;
    virtual void removeWidget(AFWidget* widget) = 0;
    virtual bool addPanel(AFPanel* panel, bool owned = false) = 0;
    virtual void removePanel(AFPanel* panel) = 0;
    
    // Mark intersecting widgets dirty
    virtual void markIntersectingWidgetsDirty(int16_t rx, int16_t ry, int16_t rw, int16_t rh) = 0;
    
    // Access to children
    const AFVector<AFWidget*, MAX_WIDGETS_PER_PANEL>& getWidgets() const { return m_widgets; }
    const AFVector<AFPanel*, MAX_PANELS_PER_SCREEN>& getPanels() const { return m_panels; }
    
    virtual ~AFContainer() = default;


protected:
    // Helper implementation that containers can use
    void markIntersectingDirty(AFVector<AFWidget*, MAX_WIDGETS_PER_PANEL>& widgets, 
                               int16_t rx, int16_t ry, int16_t rw, int16_t rh);
    
    void markIntersectingDirty(AFVector<AFPanel*, MAX_PANELS_PER_SCREEN>& panels, 
                               int16_t rx, int16_t ry, int16_t rw, int16_t rh);


protected:
    AFVector<AFWidget*, MAX_WIDGETS_PER_PANEL> m_widgets;
    AFVector<AFPanel*, MAX_PANELS_PER_SCREEN> m_panels;
};
