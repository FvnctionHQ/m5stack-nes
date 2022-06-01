#include <Arduino.h>

extern "C" {
#include <nes/nes.h>
}

#include <Arduino_GFX_Library.h>

#ifdef ST7789_DRIVER
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO);
Arduino_ST7789 *gfx = new Arduino_ST7789(bus, TFT_RST, MONITOR_ROTATION, true, MONITOR_HOR_RES, MONITOR_VER_RES, 0, 0);
#endif

#ifdef ILI9341_DRIVER
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, -1, TFT_SCLK, TFT_MOSI, -1);
//Arduino_DataBus *bus = new Arduino_SWSPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO);
Arduino_ILI9342 *gfx = new Arduino_ILI9342(bus, TFT_RST, MONITOR_ROTATION);
#endif

#ifdef ILI9488_DRIVER
Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, -1, TFT_SCLK, TFT_MOSI, -1);
//Arduino_DataBus *bus = new Arduino_SWSPI(TFT_DC, TFT_CS, TFT_SCLK, TFT_MOSI, TFT_MISO);
Arduino_ILI9488_18bit *gfx =new Arduino_ILI9488_18bit(bus,  TFT_RST, MONITOR_ROTATION);
#endif

static int16_t w, h;
extern int16_t bg_color;
extern uint16_t myPalette[];

extern void display_begin() {
  gfx->begin();
  bg_color = gfx->color565(0, 0, 0);
  gfx->fillScreen(bg_color);

#ifdef TFT_BL
  // turn display backlight on
  ledcAttachPin(TFT_BL, 1);      // assign TFT_BL pin to channel 1
  ledcSetup(1, 12000, 8);        // 12 kHz PWM, 8-bit resolution
  ledcWrite(1, TFT_BRIGHTNESS);  // brightness 0 - 255
#endif
}

extern "C" void display_init() {
  w = NES_SCREEN_WIDTH; //gfx->width();
  h = NES_SCREEN_HEIGHT; //gfx->height();
}

extern "C" void display_write_frame(const uint8_t *data[]) {
  #ifdef ILI9341_DRIVER
  int offset = 32;
  #endif
  #ifdef ILI9488_DRIVER
  int offset = 112;
  #endif
  #ifdef ST7789_DRIVER
  int offset = 0;
  #endif
  gfx->startWrite();
  gfx->writeAddrWindow(offset, 0, NES_SCREEN_WIDTH, NES_SCREEN_HEIGHT);
  for (int32_t i = 0; i < NES_SCREEN_HEIGHT; i++) {
    gfx->writeIndexedPixels((uint8_t *)(data[i]), myPalette,NES_SCREEN_WIDTH);
  }
  gfx->endWrite();
}

extern "C" void display_clear() { gfx->fillScreen(bg_color); }
