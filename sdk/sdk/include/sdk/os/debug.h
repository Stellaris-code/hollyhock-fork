/**
 * @file
 * Functions commonly used while debugging, such as printing text or
 * numbers to the screen, or waiting for a key press.
 *
 * Example:
 * @code{c}
 * // Print "Hello, world!" at 0, 0 in black on white
 * Debug_SetCursorPosition(0, 0);
 * Debug_PrintString("Hello, world!", false);
 *
 * // Print "Inverted text" at 1, 3 in white on black
 * Debug_SetCursorPosition(0, 1);
 * Debug_PrintString("Inverted text", true);
 *
 * // Print the number 0x1322 at 3, 7
 * Debug_PrintNumberHex_Word(0x1322, 3, 7);
 *
 * // Draw the changes we made to VRAM onto the LCD
 * LCD_Refresh();
 *
 * // Block until the user presses a key (see Debug_WaitKey documentation)
 * Debug_WaitKey();
 * @endcode
 */

#ifndef _SDK_OS_DEBUG_H
#define _SDK_OS_DEBUG_H
#include <stdbool.h>
#include <stdint.h>

/**
 * Returns the current position of the cursor in debug text mode.
 * 
 * @param x Pointer to an int to store the X position of the cursor in.
 * @param y Pointer to an int to store the Y position of the cursor in.
 * @return Always returns 0.
 */
int Debug_GetCursorPosition(int *x, int *y);

/**
 * Prints the hex representation of a byte (8-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The byte to print.
 * @param x The X coordinate to print the number at.
 * @param y The Y coordinate to print the number at.
 */
void Debug_PrintNumberHex_Byte(uint8_t value, int x, int y);

/**
 * Prints the hex representation of a dword (32-bit number) at the specified
 * position in debug text mode.
 * 
 * @param value The dword to print.
 * @param x The X coordinate to print the number at.
 * @param y The Y coordinate to print the number at.
 */
void Debug_PrintNumberHex_Dword(uint32_t value, int x, int y);

/**
 * Prints the hex representation of a nibble (4-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The nibble to print. High 4 bits are ignored.
 * @param x The X coordinate to print the number at.
 * @param y The Y coordinate to print the number at.
 */
void Debug_PrintNumberHex_Nibble(uint8_t value, int x, int y);

/**
 * Prints the hex representation of a word (16-bit number) at the specified
 * position in debug text mode.
 *
 * @param value The word to print.
 * @param x The X coordinate to print the number at.
 * @param y The Y coordinate to print the number at.
 */
void Debug_PrintNumberHex_Word(uint16_t value, int x, int y);

/**
 * Prints a string in debug text mode, either in normal black-on-white or
 * inverted white-on-black. Color inversion occurs if \c invert is true.
 * 
 * Returns false if the string did not fit on the screen.
 * 
 * @param string The string to print.
 * @param invert True if the colors used to print the text should be inverted.
 * @return True if writing the string was successful, false otherwise.
 */
bool Debug_PrintString(char *string, bool invert);

/**
 * Sets the position of the cursor in debug text mode.
 * 
 * @param x The cursor's new X position.
 * @param y The cursor's new Y position.
 * @return Always returns 0.
 */
int Debug_SetCursorPosition(int x, int y);

/**
 * Waits until a key is pressed, then returns a number representing the key.
 * Only appears to react to number keys and the Power/Clear key. Returns 0x30
 * to 0x39 for keys 0 to 9, and 0x98 for the Power/Clear key.
 * 
 * @return A number representing the key that was pressed.
 */
int Debug_WaitKey();

#endif
