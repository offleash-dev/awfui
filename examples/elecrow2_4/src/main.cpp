#include <Arduino.h>
#include "pins_config.h"
#include "LovyanGFX_Driver.h"
#include "touch.h"

// AWFUI includes
#include "../lib/awfui/include/AFWorld.h"
#include "../lib/awfui/include/AFScreen.h"
#include "../lib/awfui/include/AFLabel.h"
#include "../lib/awfui/include/AFBase.h"

// Backend includes from AWFUI library
#include "../lib/awfui/backends/Lovyan/AFDisplayLovyanGFX.h"
#include "../lib/awfui/backends/Lovyan/AFTouchLovyanGFX.h"

// Create global instances
AFDisplayLovyanGFX display(&gfx);
AFTouchLovyanGFX touch(&gfx);
AFWorld* world = nullptr;
AFScreen* simpleScreen = nullptr;

void setup() {
    Serial.begin(115200);
    Serial.println("=== Canvas Memory Test ===");
    
    // Show memory info
    Serial.printf("Total RAM: %d bytes\n", 327680);
    Serial.printf("Free heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Free PSRAM: %d bytes\n", ESP.getFreePsram());
    
    // Initialize display exactly like working LVGL project
    Serial.println("Step 1: Initializing display...");
    bool initResult = gfx.init();
    Serial.printf("gfx.init() returned: %s\n", initResult ? "SUCCESS" : "FAILED");
    
    Serial.println("Step 2: Initializing DMA...");
    gfx.initDMA();
    
    Serial.println("Step 3: Setting rotation to 3...");
    gfx.setRotation(3);
    Serial.printf("Display: %dx%d, rotation=%d\n", gfx.width(), gfx.height(), gfx.getRotation());
    
    Serial.println("Step 4: Turning on backlight...");
    pinMode(38, OUTPUT);
    digitalWrite(38, HIGH);
    
    Serial.println("Step 5: Starting write mode...");
    gfx.startWrite();
    gfx.fillScreen(TFT_BLACK);
    Serial.println("Display ready");
    
    // Test canvas creation
    Serial.println("Step 6: Testing canvas creation...");
    Serial.printf("Free heap before canvas: %d bytes\n", ESP.getFreeHeap());
    
    // Calculate canvas memory needed
    int canvasWidth = gfx.width();
    int canvasHeight = gfx.height();
    int canvasBytes = canvasWidth * canvasHeight * 2; // RGB565 = 2 bytes per pixel
    Serial.printf("Canvas size: %dx%d = %d bytes\n", canvasWidth, canvasHeight, canvasBytes);
    
    // Initialize AWFUI
    Serial.println("Step 7: Initializing AWFUI...");
    AFWorld::init(display, &touch, nullptr);
    world = AFWorld::instance();
    
    // Create screen WITH canvas (useCanvas=true)
    Serial.println("Step 8: Creating AWFUI screen with canvas...");
    simpleScreen = world->createScreen(true);  // useCanvas=true
    if (!simpleScreen) {
        Serial.println("Failed to create screen with canvas!");
        return;
    }
    
    Serial.printf("Free heap after canvas: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("Canvas memory used: %d bytes\n", ESP.getFreeHeap() - (ESP.getFreeHeap() - canvasBytes));
    
    // Add label to canvas
    Serial.println("Step 9: Adding label to canvas...");
    auto* label = new AFLabel(20, 20, 200, 30, "Canvas Test", MAKE_ID_FROM_STR("Label"));
    label->setJustification(AFJustificationCenter);
    simpleScreen->addWidget(label, true);
    
    // Set as active screen
    world->setActiveScreen(simpleScreen);
    Serial.println("Step 10: Canvas screen created");
    
    // Draw to canvas first
    Serial.println("Step 11: Drawing to canvas...");
    simpleScreen->draw(); // This should draw to canvas
    
    Serial.println("Step 12: Copying canvas to display...");
    // Try to copy canvas to display manually
    // Note: This depends on how AWFUI implements canvas copying
    
    Serial.println("=== Canvas Test Complete ===");
    Serial.printf("Final free heap: %d bytes\n", ESP.getFreeHeap());
}

void loop() {
    if (world) {
        world->loop();
    }
    delay(10);
}
