#define TOUCH_MODULES_CST_SELF


#include "Arduino.h"
#include "TFT_eSPI.h"
#include "TouchLib.h"
#include "Wire.h"
#include "pin_config.h"

#include "../lib/awfui/backends/tft_espi/AFDisplayTFT_espi.h"
#include "../lib/awfui/backends/tft_espi/AFTouchTouchLib.h"
#include "AFButton.h"
#include "AFLabel.h"
#include "AFModalDialog.h"
#include "AFScreen.h"
#include "AFWorld.h"



#define LCD_MODULE_CMD_1

#if defined(TOUCH_MODULES_CST_MUTUAL)
TouchLib touchLib(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS328_SLAVE_ADDRESS, PIN_TOUCH_RES);
#elif defined(TOUCH_MODULES_CST_SELF)
TouchLib touchLib(Wire, PIN_IIC_SDA, PIN_IIC_SCL, CTS820_SLAVE_ADDRESS, PIN_TOUCH_RES);
#else
#      error "Please choose the correct touch driver model!"
#endif

#if defined(LCD_MODULE_CMD_1)
typedef struct {
      uint8_t cmd;
      uint8_t data[14];
      uint8_t len;
} lcd_cmd_t;

lcd_cmd_t lcd_st7789v[] = {
    {0x11, {0}, 0 | 0x80},
    {0x3A, {0X05}, 1},
    {0xB2, {0X0B, 0X0B, 0X00, 0X33, 0X33}, 5},
    {0xB7, {0X75}, 1},
    {0xBB, {0X28}, 1},
    {0xC0, {0X2C}, 1},
    {0xC2, {0X01}, 1},
    {0xC3, {0X1F}, 1},
    {0xC6, {0X13}, 1},
    {0xD0, {0XA7}, 1},
    {0xD0, {0XA4, 0XA1}, 2},
    {0xD6, {0XA1}, 1},
    {0xE0, {0XF0, 0X05, 0X0A, 0X06, 0X06, 0X03, 0X2B, 0X32, 0X43, 0X36, 0X11, 0X10, 0X2B, 0X32}, 14},
    {0xE1, {0XF0, 0X08, 0X0C, 0X0B, 0X09, 0X24, 0X2B, 0X22, 0X43, 0X38, 0X15, 0X16, 0X2F, 0X37}, 14},
};
#endif



TFT_eSPI          tft = TFT_eSPI();
AFDisplayTFT_espi display(&tft);
AFTouchTouchLib   touch(&touchLib);

AFWorld*       world;
AFScreen*      mainScreen;
AFModalDialog* dialog;
AFLabel*       statusLabel;



void handleShowButtonClick(AFButton& sender) {
      dialog->show(*mainScreen);
}



void handleDialogOkClick(AFButton& sender) {
      dialog->dismiss();
}



void handleUpdateLabelClick(AFButton& sender) {
      static char countBuffer[32];
      static int  count = 0;

      count++;
      snprintf(countBuffer, sizeof(countBuffer), "Clicks: %d", count);
      statusLabel->setText(countBuffer);
}



void setup() {
      gpio_hold_dis((gpio_num_t) PIN_TOUCH_RES);

      pinMode(PIN_POWER_ON, OUTPUT);
      digitalWrite(PIN_POWER_ON, HIGH);

      pinMode(PIN_TOUCH_RES, OUTPUT);
      digitalWrite(PIN_TOUCH_RES, LOW);
      delay(500);
      digitalWrite(PIN_TOUCH_RES, HIGH);

      Serial.begin(115200);
      delay(1000);
      Serial.println("\n\n=== AWFUI Demo - T-Display-S3 ===");

      tft.begin();

#if defined(LCD_MODULE_CMD_1)
      for (uint8_t i = 0; i < (sizeof(lcd_st7789v) / sizeof(lcd_cmd_t)); i++) {
            tft.writecommand(lcd_st7789v[i].cmd);
            for (int j = 0; j < lcd_st7789v[i].len & 0x7f; j++) {
                  tft.writedata(lcd_st7789v[i].data[j]);
            }
            if (lcd_st7789v[i].len & 0x80) {
                  delay(120);
            }
      }
#endif

      tft.setRotation(3);
      Serial.println("TFT begin() complete");

      pinMode(PIN_LCD_BL, OUTPUT);
      digitalWrite(PIN_LCD_BL, HIGH);

      Wire.begin(PIN_IIC_SDA, PIN_IIC_SCL);
      if (!touchLib.init()) {
            Serial.println("Touch IC not found - CHECK YOUR BOARD MODEL!");
      } else {
            Serial.println("Touch IC initialized successfully");
      }

      touch.begin();
      touch.setRotation(3, 170, 320);
      Serial.println("Touch backend initialized");

      AFWorld::init(display, &touch, nullptr);
      world = AFWorld::instance();
      Serial.println("AFWorld initialized");

      mainScreen = new AFScreen(display, true, MAKE_ID_FROM_STR("Main"));
      world->addScreen(mainScreen);
      world->setActiveScreen(mainScreen);
      mainScreen->clear(0x0000);

      statusLabel = new AFLabel(10, 10, 300, 20, "Clicks: 0", MAKE_ID_FROM_STR("Stat"));
      mainScreen->addWidget(statusLabel);

      AFButton* updateBtn = new AFButton(10, 40, 140, 40, "Update Label", MAKE_ID_FROM_STR("Updt"));
      updateBtn->setOnClickCallback(handleUpdateLabelClick);
      mainScreen->addWidget(updateBtn);

      AFButton* openBtn = new AFButton(170, 40, 140, 40, "Open Dialog", MAKE_ID_FROM_STR("Open"));
      openBtn->setOnClickCallback(handleShowButtonClick);
      mainScreen->addWidget(openBtn);

      dialog = new AFModalDialog(40, 30, 240, 110, MAKE_ID_FROM_STR("HDlg"));

      AFLabel* lbl = new AFLabel(20, 20, "Hello from AWFUI!", MAKE_ID_FROM_STR("Helo"));
      dialog->addWidget(lbl);

      AFButton* okBtn = new AFButton(70, 60, 100, 40, "OK", MAKE_ID_FROM_STR("OKBt"));
      okBtn->setOnClickCallback(handleDialogOkClick);
      dialog->addWidget(okBtn);
}



void loop() {
      static unsigned long lastPrint = 0;
      static int           loopCount = 0;

      world->loop();

      loopCount++;
      if (millis() - lastPrint > 5000) {
            Serial.print("[LOOP] Running... count=");
            Serial.println(loopCount);
            lastPrint = millis();
      }

      delay(1);
}
