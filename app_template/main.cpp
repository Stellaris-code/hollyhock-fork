#include <appdef.h>
#include <sdk/os/lcd.hpp>
#include <stdint.h>

APP_NAME("App Template")
APP_DESCRIPTION("This is the description")
APP_AUTHOR("The6P4C")
APP_VERSION("1.0.0")

extern "C"
void main() {
    uint16_t *vram = LCD_GetVRAMAddress();

    for (int i = 0; i < 1000; ++i) {
        vram[i] = 0xF00F;
    }

    LCD_Refresh();
}
