// testUi.cpp — ui create for AWFUI test application
//
// Screen 1: Click test — corner buttons, status label, checkbox to enable/disable
// Screen 2: Radio test — radio buttons, checkboxes to enable/disable each other
// Screen 3: Panel test — top panel with buttons, bottom panel with image buttons, dialog


#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
#include "AFProgressBar.h"
#include "AFRadioButton.h"
#include "AFRadioButtonGroup.h"
#include "AFScreen.h"
#include "AFScreenList.h"
#include "AFSlider.h"
#include "AFWidget.h"
#include "AFWorld.h"


// Global variables
AFWorld* setupWorld = nullptr;
AFScreen* screen1;
AFScreen* screen2;
AFFullscreenDialog* screen3;




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

extern void createTextInputPanel(AFScreen& s, AFLabel* keyOutputLabel);  // Forward declaration



void showKeyboard(AFWidget& sender) {
      printf("Show keyboard callback triggered!\n");
      
      createTextInputPanel(*screen1, s1_statusLabel);
}



static void setupScreen1(int16_t width, int16_t height) {
      printf("setupScreen1: Starting\n");
      int16_t W = width;
      int16_t H = height;

      printf("setupScreen1: Creating screen\n");
      screen1 = setupWorld->createScreen(true);
      printf("setupScreen1: Screen created\n");

      // Title label — near bottom
      auto* titleLbl = new AFButton(60, H - 40, W - 120, 20, makeID("S1Tt"), "Click Test");
      titleLbl->setOnClickCallback(showKeyboard);
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
      s1_statusLabel = new AFLabel(20, 100, W - 80, 30, "", makeID("S1St"));
      s1_statusLabel->setJustification(AFJustificationCenter);
      screen1->addWidget(s1_statusLabel, true);

      // Slider to test touch capture
      auto* testSlider = new AFSlider(20, 60, W - 40, 30, makeID("S1SL"));
      testSlider->setRange(0, 100);
      testSlider->setValue(50);
      testSlider->setOnReleaseCallback([](AFSlider& sender, int value) {
            char buf[32];
            snprintf(buf, sizeof(buf), "Slider: %d", value);
            s1_statusLabel->setText(buf);
      });
      screen1->addWidget(testSlider, true);

      // Checkbox — enables/disables corner buttons
      // between tl and tr buttons
      auto* enableCb = new AFCheckbox(60, 10, 16, makeID("S1Cb"), "Corner buttons enabled");
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
      nextBtn->setOnClickCallback([](AFWidget& sender) { screen3->show(*screen2); });
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

static AFModalDialog* s3_progressDialog;
static AFLabel*       s3_progressLabel;
static AFProgressBar* s3_progressBar;


static void showScreen3Dialog(const char* text) {
      s3_dialogLabel->setText(text);
      s3_dialog->show();  // Show on active screen (which should be screen3)
}



static void showProgressDialog(const char* text) {
      s3_progressBar->setValue(0);
      s3_progressLabel->setText(text);
      s3_progressDialog->show();  // Show on active screen (which should be screen3)
}



int progressValue = 0;

static void makeProgress(AFWidget& sender) {
      progressValue += 10;
      s3_progressBar->setValue(progressValue);
      s3_progressBar->update();
}



void dismissProgressDialog(AFWidget& sender) {
      s3_progressDialog->dismiss();
      progressValue = 0;
}

void dismissCommonDialog(AFWidget& sender) {
      s3_dialog->dismiss();
}

static void setupScreen3(int16_t width, int16_t height) {
      int16_t W = width;
      int16_t H = height;

      // Create fullscreen dialog instead of screen
      screen3 = new AFFullscreenDialog(makeID("S3FS"));

      // ---- Top panel: 3 buttons ----
      AFPanel* topPanel = new AFPanel(0, 0, W, 40, makeID("S3TP"));
      int16_t btnW = (W - 20) / 3;  // 3 buttons with small gaps
      AFButton* tp1 = new AFButton(5, 5, btnW - 5, 30, makeID("TP01"), "Panel 1");
      tp1->setOnClickCallback([](AFWidget& sender) { 
            printf("Panel button 1 clicked!\n");
            showScreen3Dialog("Top panel, button 1"); 
      });
      topPanel->addWidget(tp1, true);

      AFButton* tp2 = new AFButton(btnW + 5, 5, btnW - 5, 30, makeID("TP02"), "Panel 2");
      tp2->setOnClickCallback([](AFWidget& sender) { 
            printf("Panel button 2 clicked!\n");
            showScreen3Dialog("Top panel, button 2"); 
      });
      topPanel->addWidget(tp2, true);

      AFButton* tp3 = new AFButton(btnW * 2 + 5, 5, btnW - 5, 30, makeID("TP03"), "Panel 3");
      tp3->setOnClickCallback([](AFWidget& sender) { 
            printf("Panel button 3 clicked!\n");
            showScreen3Dialog("Top panel, button 3"); 
      });
      topPanel->addWidget(tp3, true);

      screen3->addWidget(topPanel, true);

      // ---- Bottom panel: 2 image buttons ----
      AFPanel* botPanel = new AFPanel(0, H - 40, W, 40, makeID("S3BP"));
      AFImageButton* ib1 = new AFImageButton(10, 5, &imageA, makeID("IB01"));
      ib1->setOnClickCallback([](AFWidget& sender) { 
            printf("Image button A clicked!\n");
            showScreen3Dialog("Bottom panel, image A"); 
      });
      botPanel->addWidget(ib1, true);

      AFImageButton* ib2 = new AFImageButton(40, 5, &imageB, makeID("IB02"));
      ib2->setOnClickCallback([](AFWidget& sender) { 
            printf("Image button B clicked!\n");
            showScreen3Dialog("Bottom panel, image B"); 
      });
      botPanel->addWidget(ib2, true);

      screen3->addWidget(botPanel, true);

      // ---- Button to open dialog ----
      AFButton* dlgBtn = new AFButton(20, 50, W - 40, 30, makeID("S3DB"), "Progress Dialog");
      dlgBtn->setOnClickCallback([](AFWidget& sender) { showProgressDialog("Progress Dialog"); });
      screen3->addWidget(dlgBtn, true);

      
      // ---- Label above bottom panel ----
      AFLabel* lbl = new AFLabel(60, H - 70, W - 120, 20, "Final Tests", makeID("S3Lb"));
      lbl->setJustification(AFJustificationCenter);
      screen3->addWidget(lbl, true);

      // ---- Restart button above label ----
      AFButton* restartBtn = new AFButton(20, H - 110, W - 40, 30, makeID("S3Rs"), "Restart");
      restartBtn->setOnClickCallback([](AFWidget& sender) { screen3->dismiss(); setupWorld->setActiveScreen(screen1); });
      screen3->addWidget(restartBtn, true);



      // ---- Shared dialog for screen 3 ----
      s3_dialog = new AFModalDialog(20, 40, W - 40, 140, makeID("S3Dg"));

      s3_dialogLabel = new AFLabel(10, 60, "placeholder", makeID("S3DL"));
      s3_dialog->addWidget(s3_dialogLabel, true);

      AFButton* okBtn = new AFButton(50, 90, 100, 40, makeID("S3OK"), "OK");
      okBtn->setOnClickCallback(dismissCommonDialog);
      s3_dialog->addWidget(okBtn, true);



      // ---- Progress dialog for screen 3 ----
      s3_progressDialog = new AFModalDialog(20, 40, W - 40, 140, makeID("S3PD"));

      s3_progressLabel = new AFLabel(30, 10, "placeholder", makeID("S3DL"));
      s3_progressDialog->addWidget(s3_progressLabel, true);

      s3_progressBar = new AFProgressBar(30, 40, W-70, 20, makeID("S3PL"));
      s3_progressDialog->addWidget(s3_progressBar, true);

      AFButton* progressOkBtn = new AFButton(50, 80, 100, 30, makeID("SPOK"), "OK");
      progressOkBtn->setOnClickCallback(dismissProgressDialog);
      s3_progressDialog ->addWidget(progressOkBtn, true);

      AFButton* makeProgressBtn = new AFButton(180, 8, 30, 30, makeID("SPMP"), "+");
      makeProgressBtn->setOnClickCallback( makeProgress );
      s3_progressDialog ->addWidget(makeProgressBtn, true);
      
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









AFModalDialog modal2;
AFLabel modalLabel2;
AFButton closeBtn2;
void secondModalDismiss(AFWidget& sender) {
      modal2.dismiss();
      printf("Second modal dismissed\n");
}     

void showSecondModalDialog(AFWidget& sender) {
      modal2.init(40, 80, 200, 140, makeID("Mod2"));

      modalLabel2.init(20, 20, 100, 30, "This is a second modal dialog", makeID("Lab2"));
      modal2.addWidget(&modalLabel2);

      closeBtn2.init(20, 60, 100, 30, makeID("Cls2"), "Close");
      closeBtn2.setOnClickCallback(secondModalDismiss);
      modal2.addWidget(&closeBtn2);

      modal2.show(*screen1); 
}

AFModalDialog modal1;
AFLabel modalLabel1;
AFButton showBtn1;
AFButton closeBtn1;

void firstModalDismiss(AFWidget& sender) {
      modal1.dismiss();
      printf("Second modal dismissed\n");
}     

void showFirstModalDialog(AFWidget& sender) {
      modal1.init(20, 40, 200, 140, makeID("Mod1"));

      modalLabel1.init(20, 20, 100, 30, "This is a modal dialog", makeID("Lab1"));
      modal1.addWidget(&modalLabel1);
      
      showBtn1.init(20, 60, 100, 30, makeID("Shw1"), "Show");
      showBtn1.setOnClickCallback(showSecondModalDialog);
      modal1.addWidget(&showBtn1);

      closeBtn1.init(20, 100, 100, 30, makeID("Cls1"), "Close");
      closeBtn1.setOnClickCallback(firstModalDismiss);
      modal1.addWidget(&closeBtn1);

      modal1.show(*screen1); 
}     






void setupScreenModalHost(int16_t width, int16_t height) {
      int16_t W = width;
      int16_t H = height;

      screen1 = setupWorld->createScreen(true);

      AFButton* showModalButton = new AFButton(60, H - 40, W - 120, 20, makeID("ShwS"), "Show First Modal");
      showModalButton->setOnClickCallback(showFirstModalDialog);
      screen1->addWidget(showModalButton, true);
}


void setupStackedModalTestUI(int16_t width, int16_t height) {
      setupWorld = AFWorld::instance();
 
      setupScreenModalHost(width, height);

      setupWorld->setActiveScreen(screen1);
}
