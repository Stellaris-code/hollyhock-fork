#include <sdk/os/debug.hpp>
#include <sdk/os/lcd.hpp>

void main() {
	Debug_SetCursorPosition(0, 0);
	Debug_PrintString("Hello, world!", false);
	LCD_Refresh();
}
