#include <sdk/os/debug.h>
#include <sdk/os/lcd.h>

void main() {
	Debug_SetCursorPosition(0, 0);
	Debug_PrintString("Hello, world!", false);
	LCD_Refresh();
}
