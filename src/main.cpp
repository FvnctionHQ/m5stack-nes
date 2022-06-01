#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include <SPIFFS.h>
#include <SD.h>
#include <esp_task_wdt.h>
#include <esp_wifi.h>

#include "display.h"
#include <M5TreeView.h>
#include <MenuItemSD.h>
#include <MenuItemSPIFFS.h>
extern "C" {
#include <nofrendo.h>
}


// FS
char *argv[1];
#define SD_FILESYSTEM_BEGIN SD.begin(4, SPI, 40000000, "/sd"); FS sd_filesystem = SD;
#define SPIFFS_FILESYSTEM_BEGIN SPIFFS.begin(false, "/fs"); FS spiffs_filesystem = SPIFFS;
#define SD_FILESYSTEM_END SD.end();
#define SPIFFS_FILESYSTEM_END SPIFFS.end();

// menu
M5TreeView treeView;
typedef std::vector<MenuItem *> vmi;
bool menu_is_running = false;

// Nofrendo
extern void display_begin();
int16_t bg_color;
extern Arduino_TFT *gfx;



bool is_rom_available() {

    SPIFFS_FILESYSTEM_BEGIN
    bool is_loaded = spiffs_filesystem.exists("/rom.nes");
    SPIFFS_FILESYSTEM_END
    return is_loaded;
}

void copy_rom(String sdPath) {

    SD_FILESYSTEM_BEGIN
    SPIFFS_FILESYSTEM_BEGIN

    File sourceFile = sd_filesystem.open(sdPath);
    File destFile = spiffs_filesystem.open("/rom.nes", FILE_WRITE);
    static uint8_t buf[512];
    int i = 0;
    while (sourceFile.read(buf, 512)) {
        i++;
        M5.Lcd.progressBar(10, 10, TFT_WIDTH - 20, 20, i);
        destFile.write(buf, 512);
    }
    destFile.close();
    sourceFile.close();

    SD_FILESYSTEM_END
    SPIFFS_FILESYSTEM_END

}

void on_menu_item_click(MenuItem *sender) {
    MenuItemFS *mi = static_cast<MenuItemFS *>(sender);
    if (!mi) return;

    if (mi->isDir) return;

    int idx = mi->path.lastIndexOf('.') + 1;
    String ext = mi->path.substring(idx);
    if (ext == "nes") {
        menu_is_running = false;
        M5.Lcd.clearDisplay(ILI9341_BLACK);
        copy_rom(mi->path);
        M5.Power.reset();
    }
}


static void load_rom() {


    SPIFFS_FILESYSTEM_BEGIN
    argv[0] = (char *) "/fs/rom.nes";

    display_begin();
    nofrendo_main(1, argv);
}

static void load_menu() {
    menu_is_running = true;
    Wire.begin();
    M5.Lcd.begin();
    treeView.clientRect.x = 2;
    treeView.clientRect.y = 16;
    treeView.clientRect.w = 316;
    treeView.clientRect.h = 200;
    treeView.setTextFont(2);
    treeView.itemHeight = 18;
    treeView.itemWidth = 220;

    treeView.useFACES = true;
    treeView.useJoyStick = false;
    treeView.usePLUSEncoder = false;
    treeView.useFACESEncoder = false;

    treeView.setItems(vmi
                              {
                                      new MenuItemSD("SD Card", on_menu_item_click),
                                      new MenuItemSPIFFS("SPIFFS", on_menu_item_click)
                              }
    );

    menu_is_running = true;
    treeView.begin();
    for (;;) {
        M5.Lcd.drawString(":=: M5Stack NES  [ Restart with B+C to change ROM ]", 0, 0);
        while (menu_is_running) {
            treeView.update();
        }

    }
}


void setup() {

    M5.begin(false, false, true, true);
    if (M5.BtnB.isPressed() && M5.BtnC.isPressed()) {
        SPIFFS_FILESYSTEM_BEGIN
        if (spiffs_filesystem.exists("/rom.nes")) {
            spiffs_filesystem.remove("/rom.nes");
        }
        SPIFFS_FILESYSTEM_END
        M5.Power.reset();
        return;
    }

#if defined(ILI9341_DRIVER) or defined(ILI9488_DRIVER)
    pinMode(TFT_MISO, OUTPUT);
    digitalWrite(TFT_MISO, LOW);
    pinMode(TFT_CS, OUTPUT);
    digitalWrite(TFT_CS, LOW);
#endif

    Serial.begin(115200);
    esp_wifi_deinit();

    TaskHandle_t idle_0 = xTaskGetIdleTaskHandleForCPU(0);
    esp_task_wdt_delete(idle_0);

    if (is_rom_available())
    {
        load_rom();

    } else {
        load_menu();
    }







}

void loop() {}