#include <appdef.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/input.hpp>
#include <sdk/os/lcd.hpp>

APP_NAME("Input_IsAnyKeyDown/Input_GetKeyState Demo")

struct LUTEntry {
	const char *str;
	InputScancode scancode;
};

#define LUT_ENTRY(name) { #name, name }

// A simple lookup table, mapping scancodes to their readable names.
struct LUTEntry lut[] = {
	LUT_ENTRY(ScancodeKeyboard),
    LUT_ENTRY(ScancodeShift),
    LUT_ENTRY(ScancodeBackspace),
    LUT_ENTRY(ScancodeClear),
    LUT_ENTRY(ScancodeUp),
    LUT_ENTRY(ScancodeDown),
    LUT_ENTRY(ScancodeLeft),
    LUT_ENTRY(ScancodeRight),
    LUT_ENTRY(ScancodeEquals),
    LUT_ENTRY(ScancodeX),
    LUT_ENTRY(ScancodeY),
    LUT_ENTRY(ScancodeZ),
    LUT_ENTRY(ScancodePower),
    LUT_ENTRY(ScancodeDivide),
    LUT_ENTRY(ScancodeOpenParenthesis),
    LUT_ENTRY(Scancode7),
    LUT_ENTRY(Scancode8),
    LUT_ENTRY(Scancode9),
    LUT_ENTRY(ScancodeTimes),
    LUT_ENTRY(ScancodeCloseParenthesis),
    LUT_ENTRY(Scancode4),
    LUT_ENTRY(Scancode5),
    LUT_ENTRY(Scancode6),
    LUT_ENTRY(ScancodeMinus),
    LUT_ENTRY(ScancodeComma),
    LUT_ENTRY(Scancode1),
    LUT_ENTRY(Scancode2),
    LUT_ENTRY(Scancode3),
    LUT_ENTRY(ScancodePlus),
    LUT_ENTRY(ScancodeNegative),
    LUT_ENTRY(Scancode0),
    LUT_ENTRY(ScancodeDot),
    LUT_ENTRY(ScancodeEXP),
    LUT_ENTRY(ScancodeEXE)
};
constexpr int numScancodes = sizeof(lut) / sizeof(lut[0]);

extern "C"
void main() {
    bool running = true;
    while (running) {
        LCD_ClearScreen();

        bool isKeyDown = Input_IsAnyKeyDown();
        Debug_Printf(
            0, 0, false, 0,
            "IsAnyKeyDown = %s", isKeyDown ? "true" : "false"
        );

        // The Y position to start printing the names of the currently pressed
        // keys at
        int printY = 2;

        if (isKeyDown) {
            for (int i = 0; i < numScancodes; ++i) {
                struct LUTEntry lutEntry = lut[i];

                // Pass a pointer to the scancode to check into
                // Input_GetKeyState
                if (Input_GetKeyState(&lutEntry.scancode)) {
                    // Print the name of the key that was pressed
                    Debug_Printf(
                        0, printY++, false, 0,
                        "%s", lutEntry.str
                    );

                    // If the scancode we're currently checking is the Clear key
                    // and it's pressed (we can't be here if Input_GetKeyState
                    // was false), exit the loop.
                    if (lutEntry.scancode == ScancodeClear) {
                        running = false;
                    }
                }
            }
        }

        LCD_Refresh();
    }
}
