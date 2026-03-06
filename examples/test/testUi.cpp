// testUi.cpp — ui create for AWFUI test application
//
// Screen 1: Click test — corner buttons, status label, checkbox to enable/disable
// Screen 2: Radio test — radio buttons, checkboxes to enable/disable each other
// Screen 3: Panel test — top panel with buttons, bottom panel with image buttons, dialog


#include <stdint.h>

#include "testui.h"

#include "AFBase.h"
#include "AFButton.h"
#include "AFCheckbox.h"
#include "AFFullscreenDialog.h"
#include "AFImage.h"
#include "AFImageButton.h"
#include "AFImageWidget.h"
#include "AFLabel.h"
#include "AFModalDialog.h"
#include "AFPanel.h"
#include "AFRadioButton.h"
#include "AFRadioButtonGroup.h"
#include "AFScreen.h"
#include "AFWorld.h"




AFWorld* setupWorld = nullptr;
AFScreen* screen1;
AFScreen* screen2;
AFScreen* screen3;



// ============================================================
// Stub 20x20 1-bit images (replace with real assets)
// ============================================================

// Image A: simple "A" glyph, 20x20, 1-bit (3 bytes per row, 60 bytes total)
static const uint8_t imgA_data[] = {
      // AFImageHeader: w=20, h=20, format=1-bit, reserved=0
      0x14, 0x00,  // width  = 20
      0x14, 0x00,  // height = 20
      0x01,        // format = 1-bit
      0x00,        // reserved
      // pixel data (20 rows × 3 bytes = 60 bytes)
      0x00, 0x00, 0x00,  0x01, 0x80, 0x00,  0x03, 0xC0, 0x00,  0x07, 0xE0, 0x00,
      0x06, 0x60, 0x00,  0x0C, 0x30, 0x00,  0x0C, 0x30, 0x00,  0x1F, 0xF8, 0x00,
      0x18, 0x18, 0x00,  0x18, 0x18, 0x00,  0x30, 0x0C, 0x00,  0x30, 0x0C, 0x00,
      0x30, 0x0C, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,
      0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,
};

// Image B: simple "B" glyph, 20x20, 1-bit
static const uint8_t imgB_data[] = {
      0x14, 0x00,  0x14, 0x00,  0x01, 0x00,
      0x1F, 0xC0, 0x00,  0x18, 0x60, 0x00,  0x18, 0x30, 0x00,  0x18, 0x30, 0x00,
      0x18, 0x60, 0x00,  0x1F, 0xC0, 0x00,  0x18, 0x60, 0x00,  0x18, 0x30, 0x00,
      0x18, 0x30, 0x00,  0x18, 0x30, 0x00,  0x18, 0x60, 0x00,  0x1F, 0xC0, 0x00,
      0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,
      0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,  0x00, 0x00, 0x00,
};

static AFImage imageA(imgA_data);
static AFImage imageB(imgB_data);




// ============================================================
// Screen 1 — Click Test
// ============================================================

// Widgets that need cross-callback access
static AFLabel*    s1_statusLabel;
static AFButton*   s1_btnTL;
static AFButton*   s1_btnTR;
static AFButton*   s1_btnBL;
static AFButton*   s1_btnBR;

static void setupScreen1(int16_t width, int16_t height) {
      printf("setupScreen1: Starting\n");
      int16_t W = width;
      int16_t H = height;

      printf("setupScreen1: Creating screen\n");
      screen1 = setupWorld->createScreen(true);
      printf("setupScreen1: Screen created\n");

      // Title label — near bottom
      auto* titleLbl = new AFButton(60, H - 40, W - 120, 20, makeID("S1Tt"), "Click Test");
      screen1->addWidget(titleLbl, true);

      // "Next Test Screen" button — above title
      printf("setupScreen1: Creating button\n");
      auto* nextBtn = new AFButton(20, H - 90, W - 40, 30, makeID("S1Nx"), "Next Test Screen");
      nextBtn->setOnClickCallback([](AFWidget& sender) { setupWorld->setActiveScreen(screen2); });
      screen1->addWidget(nextBtn, true);
      printf("setupScreen1: second Button created\n");

      // Corner buttons — 40x40
      printf("setupScreen1: Adding widget\n");
      s1_btnTL = new AFButton(0, 0, 40, 40, makeID("S1TL"), "TL");
      s1_btnTL->setOnClickCallback([](AFWidget& sender) { s1_statusLabel->setText("TL clicked"); });
      screen1->addWidget(s1_btnTL, true);
      printf("setupScreen1: Widget added\n");

      printf("setupScreen1: Creating button s1tr\n");
      s1_btnTR = new AFButton(W - 40, 0, 40, 40, makeID("S1TR"), "TR");
      printf("setupScreen1: new button s1tr done\n");
      s1_btnTR->setOnClickCallback([](AFWidget& sender) { s1_statusLabel->setText("TR clicked"); });
      printf("setupScreen1: set button calllback s1tr done\n");
      if (s1_btnTR == nullptr) {
            printf("setupScreen1: ERROR - s1_btnTR is null\n");
      } else {
            printf("setupScreen1: s1_btnTR is valid\n");
      }
      screen1->addWidget(s1_btnTR, true);
      printf("setupScreen1: Button created\n");

      s1_btnBL = new AFButton(0, H - 40, 40, 40, makeID("S1BL"), "BL");
      s1_btnBL->setOnClickCallback([](AFWidget& sender) { s1_statusLabel->setText("BL clicked"); });
      screen1->addWidget(s1_btnBL, true);
      printf("setupScreen1: Button created\n");

      s1_btnBR = new AFButton(W - 40, H - 40, 40, 40, makeID("S1BR"), "BR");
      s1_btnBR->setOnClickCallback([](AFWidget& sender) { s1_statusLabel->setText("BR clicked"); });
      screen1->addWidget(s1_btnBR, true);
      printf("setupScreen1: Button created\n");

      // Status label — shows which corner button was clicked
      s1_statusLabel = new AFLabel(20, 60, W - 40, 30, "", makeID("S1St"));
      s1_statusLabel->setJustification(AFJustificationCenter);
      screen1->addWidget(s1_statusLabel, true);

      // Checkbox — enables/disables corner buttons
      auto* enableCb = new AFCheckbox(20, 110, 16, makeID("S1Cb"), "Corner buttons enabled");
      enableCb->setChecked(true);
      enableCb->setOnChangeCallback([](AFCheckbox& sender, bool checked) {
            s1_btnTL->setEnabled(checked);
            s1_btnTR->setEnabled(checked);
            s1_btnBL->setEnabled(checked);
            s1_btnBR->setEnabled(checked);
      });
      screen1->addWidget(enableCb, true);
}



// ============================================================
// Screen 2 — Radio Test
// ============================================================

static AFRadioButton*    s2_radio2;
static AFCheckbox*       s2_cb1;
static AFRadioButtonGroup s2_radioGroup;

static void setupScreen2(int16_t width, int16_t height) {
      int16_t W = width;
      int16_t H = height;

      screen2 = setupWorld->createScreen(true);

      // Title label
      auto* titleLbl = new AFLabel(60, H - 40, W - 120, 20, "Radio Test", makeID("S2Tt"));
      titleLbl->setJustification(AFJustificationCenter);
      screen2->addWidget(titleLbl, true);

      // "Next Test Screen" button
      auto* nextBtn = new AFButton(20, H - 90, W - 40, 30, makeID("S2Nx"), "Next Test Screen");
      nextBtn->setOnClickCallback([](AFWidget& sender) { setupWorld->setActiveScreen(screen3); });
      screen2->addWidget(nextBtn, true);

      // Radio buttons
      auto* radio1 = new AFRadioButton(40, 60, 8, makeID("S2R1"), "Radio Test 1");
      s2_radio2    = new AFRadioButton(40, 80, 8, makeID("S2R2"), "Radio Test 2");
      auto* radio3 = new AFRadioButton(40, 100, 8, makeID("S2R3"), "Radio Test 3");

      s2_radioGroup.addButton(radio1);
      s2_radioGroup.addButton(s2_radio2);
      s2_radioGroup.addButton(radio3);

      screen2->addWidget(radio1, true);
      screen2->addWidget(s2_radio2, true);
      screen2->addWidget(radio3, true);

      // Checkbox 1 — enables/disables radio 2
      s2_cb1 = new AFCheckbox(40, H - 110, 16, makeID("S2C1"), "Radio 2 enabled");
      s2_cb1->setChecked(true);
      s2_cb1->setOnChangeCallback([](AFCheckbox& sender, bool checked) {
            s2_radio2->setEnabled(checked);
      });
      screen2->addWidget(s2_cb1, true);

      // Checkbox 2 — enables/disables checkbox 1
      auto* cb2 = new AFCheckbox(40, H - 130, 16, makeID("S2C2"), "Checkbox 1 enabled");
      cb2->setChecked(true);
      cb2->setOnChangeCallback([](AFCheckbox& sender, bool checked) {
            s2_cb1->setEnabled(checked);
      });
      screen2->addWidget(cb2, true);
}



// ============================================================
// Screen 3 — Panel Test
// ============================================================

static AFModalDialog* s3_dialog;
static AFLabel*       s3_dialogLabel;

static void showScreen3Dialog(const char* text) {
      s3_dialogLabel->setText(text);
      s3_dialog->show(*screen3);
}

static void setupScreen3(int16_t width, int16_t height) {
      int16_t W = width;
      int16_t H = height;

      screen3 = setupWorld->createScreen(true);

      // ---- Top panel: 3 buttons ----
      auto* topPanel = new AFPanel(0, 0, W, 40, makeID("S3TP"));
      int16_t btnW = (W - 20) / 3;  // 3 buttons with small gaps
      auto* tp1 = new AFButton(5, 5, btnW - 5, 30, makeID("TP01"), "Panel 1");
      tp1->setOnClickCallback([](AFWidget& sender) { showScreen3Dialog("Top panel, button 1"); });
      topPanel->addWidget(tp1, true);

      auto* tp2 = new AFButton(btnW + 5, 5, btnW - 5, 30, makeID("TP02"), "Panel 2");
      tp2->setOnClickCallback([](AFWidget& sender) { showScreen3Dialog("Top panel, button 2"); });
      topPanel->addWidget(tp2, true);

      auto* tp3 = new AFButton(btnW * 2 + 5, 5, btnW - 5, 30, makeID("TP03"), "Panel 3");
      tp3->setOnClickCallback([](AFWidget& sender) { showScreen3Dialog("Top panel, button 3"); });
      topPanel->addWidget(tp3, true);

      screen3->addPanel(topPanel, true);

      // ---- Bottom panel: 2 image buttons ----
      auto* botPanel = new AFPanel(0, H - 40, W, 40, makeID("S3BP"));
      auto* ib1 = new AFImageButton(10, H - 35, &imageA, makeID("IB01"));
      ib1->setOnClickCallback([](AFWidget& sender) { showScreen3Dialog("Bottom panel, image A"); });
      botPanel->addWidget(ib1, true);

      auto* ib2 = new AFImageButton(40, H - 35, &imageB, makeID("IB02"));
      ib2->setOnClickCallback([](AFWidget& sender) { showScreen3Dialog("Bottom panel, image B"); });
      botPanel->addWidget(ib2, true);

      screen3->addPanel(botPanel, true);

      // ---- Button to open dialog ----
      auto* dlgBtn = new AFButton(20, 60, W - 40, 30, makeID("S3DB"), "Open Dialog");
      dlgBtn->setOnClickCallback([](AFWidget& sender) { showScreen3Dialog("Dialog from button"); });
      screen3->addWidget(dlgBtn, true);

      // ---- Label above bottom panel ----
      auto* lbl = new AFLabel(60, H - 80, W - 120, 20, "Final Tests", makeID("S3Lb"));
      lbl->setJustification(AFJustificationCenter);
      screen3->addWidget(lbl, true);

      // ---- Restart button above label ----
      auto* restartBtn = new AFButton(20, H - 120, W - 40, 30, makeID("S3Rs"), "Restart");
      restartBtn->setOnClickCallback([](AFWidget& sender) { setupWorld->setActiveScreen(screen1); });
      screen3->addWidget(restartBtn, true);

      // ---- Shared dialog for screen 3 ----
      s3_dialog = new AFModalDialog(20, 40, W - 40, 140, makeID("S3Dg"));

      s3_dialogLabel = new AFLabel(30, 60, "placeholder", makeID("S3DL"));
      s3_dialog->addWidget(s3_dialogLabel, true);

      auto* okBtn = new AFButton(50, 120, 100, 40, makeID("S3OK"), "OK");
      okBtn->setOnClickCallback([](AFWidget& sender) { s3_dialog->dismiss(); });
      s3_dialog->addWidget(okBtn, true);
}




// ============================================================
// Setup UI.  You should be able to put this in any project (e.g. hardware specific)
// ============================================================
void setupUI(int16_t width, int16_t height) {
      setupWorld = AFWorld::instance();
 
      setupScreen1(width, height);
      setupScreen2(width, height);
      setupScreen3(width, height);

      setupWorld->setActiveScreen(screen1);
}
