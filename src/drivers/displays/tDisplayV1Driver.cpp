#include "displayDriver.h"

#ifdef V1_DISPLAY

#include <TFT_eSPI.h>
#include "media/images_320_170.h"
#include "media/myFonts.h"
#include "media/Free_Fonts.h"
#include "version.h"
#include "monitor.h"
#include "OpenFontRender.h"
#include "rotation.h"
#include "../../motoko_screen.h"

#define WIDTH 320
#define HEIGHT 170

OpenFontRender render;
TFT_eSPI tft = TFT_eSPI();                  // Invoke library, pins defined in User_Setup.h
TFT_eSprite background = TFT_eSprite(&tft); // Invoke library sprite

void tDisplay_Init(void)
{
  tft.init();
  tft.setRotation(ROTATION_90);
  tft.setSwapBytes(true);                 // Swap the colour byte order when rendering
  background.createSprite(WIDTH, HEIGHT); // Background Sprite
  background.setSwapBytes(true);
  render.setDrawer(background);  // Link drawing object to background instance (so font will be rendered on background)
  render.setLineSpaceRatio(0.9); // Espaciado entre texto

  // Load the font and check it can be read OK
  // if (render.loadFont(NotoSans_Bold, sizeof(NotoSans_Bold))) {
  if (render.loadFont(DigitalNumbers, sizeof(DigitalNumbers)))
  {
    Serial.println("Initialise error");
    return;
  }
}

void tDisplay_AlternateScreenState(void)
{
  int screen_state = digitalRead(TFT_BL);
  Serial.println("Switching display state");
  digitalWrite(TFT_BL, !screen_state);
}

void tDisplay_AlternateRotation(void)
{
  tft.setRotation( flipRotation(tft.getRotation()) );
}

void tDisplay_MinerScreen(unsigned long mElapsed)
{
  mining_data data = getMiningData(mElapsed);

  // Print background screen
  background.pushImage(0, 0, MinerWidth, MinerHeight, MinerScreen);

  Serial.printf(">>> Completed %s share(s), %s Khashes, avg. hashrate %s KH/s\n",
                data.completedShares.c_str(), data.totalKHashes.c_str(), data.currentHashRate.c_str());

  // Hashrate
 // render.setFontSize(30);
 // render.setCursor(19, 118);
  render.setFontSize(38);
  render.setCursor(19, 318);
  render.setFontColor(TFT_BLACK);

  render.rdrawString(data.currentHashRate.c_str(), 125, 110, TFT_BLACK);
  // Total hashes
  render.setFontSize(15);
  render.rdrawString(data.totalMHashes.c_str(), 268, 142, TFT_BLACK);
  // Block templates
  render.setFontSize(16);
  render.drawString(data.templates.c_str(), 190, 20, 0xDEDB);
  // Best diff
  render.drawString(data.bestDiff.c_str(), 190, 48, 0xDEDB);
  // 32Bit shares
  render.setFontSize(16);
  render.drawString(data.completedShares.c_str(), 190, 76, 0xDEDB);
  // Hores
  render.setFontSize(12);
  render.rdrawString(data.timeMining.c_str(), 304, 105, 0xDEDB);

  // Valid Blocks
  render.setFontSize(29);
  render.drawString(data.valids.c_str(), 278, 52, 0xDEDB);

  // Print Temp
  render.setFontSize(10);
  render.rdrawString(data.temp.c_str(), 238, 1, TFT_BLACK);

  render.setFontSize(3);
  render.rdrawString(String(0).c_str(), 284, 2, TFT_BLACK);

  // Print Hour
  render.setFontSize(10);
  render.rdrawString(data.currentTime.c_str(), 284, 1, TFT_BLACK);

  // Push prepared background to screen
  background.pushSprite(0, 0);
}

void tDisplay_ClockScreen(unsigned long mElapsed)
{
  clock_data data = getClockData(mElapsed);

  // Print background screen
  background.pushImage(0, 0, minerClockWidth, minerClockHeight, minerClockScreen);

  Serial.printf(">>> Completed %s share(s), %s Khashes, avg. hashrate %s KH/s\n",
                data.completedShares.c_str(), data.totalKHashes.c_str(), data.currentHashRate.c_str());

  // Hashrate
  render.setFontSize(26);
  render.setCursor(20, 142);
  render.setFontColor(TFT_BLACK);
  render.rdrawString(data.currentHashRate.c_str(), 80, 123, TFT_BLACK);

  // Print BTC Price
  background.setFreeFont(FSSB9);
  background.setTextSize(1);
  background.setTextDatum(TL_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.btcPrice.c_str(), 248, 1, GFXFF);

  // Print BlockHeight
  render.setFontSize(14);
  render.rdrawString(data.blockHeight.c_str(), 250, 142, TFT_BLACK);

  // Print Hour
  background.setFreeFont(FF23);
  background.setTextSize(2);
  background.setTextColor(0xDEDB, TFT_BLACK);

  background.drawString(data.currentTime.c_str(), 130, 40, GFXFF);

  // Push prepared background to screen
  background.pushSprite(0, 0);
}

void tDisplay_GlobalHashScreen(unsigned long mElapsed)
{
  coin_data data = getCoinData(mElapsed);

  // Print background screen
  background.pushImage(0, 0, globalHashWidth, globalHashHeight, globalHashScreen);

  Serial.printf(">>> Completed %s share(s), %s Khashes, avg. hashrate %s KH/s\n",
                data.completedShares.c_str(), data.totalKHashes.c_str(), data.currentHashRate.c_str());

  // Print BTC Price
  background.setFreeFont(FSSB9);
  background.setTextSize(1);
  background.setTextDatum(TL_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.btcPrice.c_str(), 248, 2, GFXFF);

  // Print Hour
  background.setFreeFont(FSSB9);
  background.setTextSize(1);
  background.setTextDatum(TL_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.currentTime.c_str(), 274, 2, GFXFF);

  // Print Last Pool Block
  background.setFreeFont(FSS9);
  background.setTextDatum(TR_DATUM);
  background.setTextColor(0x9C92);
  background.drawString(data.halfHourFee.c_str(), 300, 52, GFXFF);  //ok
  //background.drawString(data.halfHourFee.c_str(), 280, 148, GFXFF);  // test

  // Print Difficulty ok
  background.setFreeFont(FSS9);
  background.setTextDatum(TR_DATUM);
  background.setTextColor(0x9C92);
  background.drawString(data.netwrokDifficulty.c_str(), 300, 92, GFXFF);  


  // Print Global Hashrate
  render.setFontSize(12);
  render.rdrawString(data.globalHashRate.c_str(), 280, 148, TFT_BLACK);

  // Print BlockHeight
  render.setFontSize(25);
  render.rdrawString(data.blockHeight.c_str(), 145, 105, 0xDEDB);

  // Draw percentage rectangle
  int x2 = 2 + (138 * data.progressPercent / 100);
  background.fillRect(2, 149, x2, 168, 0xDEDB);

  // Print Remaining BLocks
  background.setTextFont(FONT2);
  background.setTextSize(1);
  background.setTextDatum(MC_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.remainingBlocks.c_str(), 59, 160, FONT2);

  // Push prepared background to screen
  background.pushSprite(0, 0);
}

void tDisplay_BTCprice(unsigned long mElapsed)
{
  clock_data data = getClockData(mElapsed);
  
  // Print background screen
  background.pushImage(0, 0, priceScreenWidth, priceScreenHeight, priceScreen);

  Serial.printf(">>> Completed %s share(s), %s Khashes, avg. hashrate %s KH/s\n",
                data.completedShares.c_str(), data.totalKHashes.c_str(), data.currentHashRate.c_str());

  // Hashrate
  render.setFontSize(25);
  render.setCursor(19, 122);
  render.setFontColor(TFT_BLACK);
  render.rdrawString(data.currentHashRate.c_str(), 70, 103, TFT_BLACK);

  // Print BlockHeight
  render.setFontSize(18);
  render.rdrawString(data.blockHeight.c_str(), 190, 110, TFT_WHITE);

  // Print Hour
  background.setFreeFont(FSSB9);
  background.setTextSize(1);
  background.setTextDatum(TL_DATUM);
  background.setTextColor(TFT_BLACK);
  background.drawString(data.currentTime.c_str(), 148, 1, GFXFF);

  // Print BTC Price 
  background.setFreeFont(FF24);
  background.setTextDatum(TR_DATUM);
  background.setTextSize(1);
  background.setTextColor(0xDEDB, TFT_BLACK);
  background.drawString(data.btcPrice.c_str(), 70, 25, GFXFF);

  // Push prepared background to screen
  background.pushSprite(0, 0);
}

void tDisplay_LoadingScreen(void)
{
  tft.fillScreen(TFT_BLACK);
  tft.pushImage(0, 0, initWidth, initHeight, initScreen);
  tft.setTextColor(TFT_BLACK);
  tft.drawString(CURRENT_VERSION, 24, 147, FONT2);
}

void tDisplay_SetupScreen(void)
{
  tft.pushImage(0, 0, setupModeWidth, setupModeHeight, setupModeScreen);
}

void tDisplay_AnimateCurrentScreen(unsigned long frame)
{
}

void tDisplay_DoLedStuff(unsigned long frame)
{
}

CyclicScreenFunction tDisplayCyclicScreens[] = {tDisplay_MinerScreen, tDisplay_ClockScreen, tDisplay_GlobalHashScreen, tDisplay_MotokoScreen};

DisplayDriver tDisplayV1Driver = {
    tDisplay_Init,
    tDisplay_AlternateScreenState,
    tDisplay_AlternateRotation,
    tDisplay_LoadingScreen,
    tDisplay_SetupScreen,
    tDisplayCyclicScreens,
    tDisplay_AnimateCurrentScreen,
    tDisplay_DoLedStuff,
    SCREENS_ARRAY_SIZE(tDisplayCyclicScreens),
    0,
    WIDTH,
    HEIGHT};
#endif
// Inhalt zum Anhängen an src/drivers/displays/tDisplayV1Driver.cpp
// im Pixelssquad-Fork (NerdMiner_v2_IdeaSpark1.9in-ST7789-Display-LCD-TFT-170x320)
// — basiert auf #ifdef V1_DISPLAY Block + nutzt sprite-Pattern.

#include "../../monitor.h"
#include "../../motoko_screen.h"
#include "displayDriver.h"
#include <TFT_eSPI.h>
#include "OpenFontRender.h"

extern TFT_eSPI tft;
extern TFT_eSprite background;
extern OpenFontRender render;

// Layout für T-Display S3 (320×170), Landscape via ROTATION_90
// — alles in background-Sprite gerendert, dann pushSprite ins TFT
void tDisplay_MotokoScreen(unsigned long mElapsed) {
    motoko_data data = motoko_getData();

    background.fillSprite(TFT_BLACK);

    if (!data.valid) {
        background.setTextColor(TFT_DARKGREY, TFT_BLACK);
        background.setTextFont(2);
        background.setTextSize(2);
        background.drawCentreString("Motoko --", 160, 60, 1);
        background.setTextSize(1);
        background.drawCentreString("waiting for data", 160, 100, 1);
        background.pushSprite(0, 0);
        return;
    }

    // ── BTC-Preis (oben, gross zentriert) ──────────────────────
    background.setTextColor(TFT_WHITE, TFT_BLACK);
    background.setTextSize(1);
    render.setFontSize(48);
    render.rdrawString(("$" + String(data.btc_usd)).c_str(), 200, 5, TFT_WHITE);

    // ── 24h-Change (oben rechts vom Preis) ─────────────────────
    bool up = data.btc_change_24h >= 0.0f;
    uint16_t changeColor = up ? TFT_GREEN : TFT_RED;
    String chgStr = (up ? "+" : "") + String(data.btc_change_24h, 1) + "%";
    render.setFontSize(20);
    render.rdrawString(chgStr.c_str(), 315, 18, changeColor);

    // ── FnG-Zeile (links, mit Zone-Farbe) ──────────────────────
    uint16_t fngColor = TFT_YELLOW;
    if (data.fng_value <= 25) fngColor = TFT_RED;
    else if (data.fng_value <= 45) fngColor = TFT_ORANGE;
    else if (data.fng_value >= 75) fngColor = TFT_GREEN;
    render.setFontSize(22);
    String fngStr = "FnG " + String(data.fng_value) + "  " + data.fng_class;
    render.rdrawString(fngStr.c_str(), 315, 58, fngColor);

    // ── Block + Mempool-Fee (mittlere Zeile) ───────────────────
    render.setFontSize(18);
    String blockStr = "Block " + String(data.block_height);
    render.rdrawString(blockStr.c_str(), 315, 88, TFT_LIGHTGREY);

    String feeStr = String(data.median_fee_sat_vb) + " sat/vB";
    render.rdrawString(feeStr.c_str(), 315, 110, TFT_LIGHTGREY);

    // ── Halving-Countdown (unten links) ────────────────────────
    String halvingStr = "Halving " + String(data.halving_days) + "d";
    render.rdrawString(halvingStr.c_str(), 160, 135, TFT_CYAN);

    // ── Pi-LNbits Saldo (unten rechts, Lightning-orange) ──────
    String lnStr = String(data.pi_lnbits_sats) + " sats";
    render.rdrawString(lnStr.c_str(), 315, 135, TFT_ORANGE);

    // Bottom-bar mit Motoko-Label
    background.fillRect(0, 158, 320, 12, TFT_DARKGREY);
    background.setTextColor(TFT_BLACK, TFT_DARKGREY);
    background.setTextSize(1);
    background.drawString("Motoko Stats", 5, 160);

    background.pushSprite(0, 0);
}
