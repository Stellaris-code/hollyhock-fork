/**
 * @file
 * @brief Functions for interacting with the LCD and VRAM.
 *
 * Different levels of indirection are availiable to write to VRAM (direct
 * memory access, getter/setters and palette-based drawing). The contents of
 * VRAM are not automatically drawn to the LCD, and must be rendered with
 * @ref LCD_Refresh. 
 * 
 * Example: drawing a 30x50 rectangle at 10, 20 in purple
 * @code{cpp}
 * uint16_t *vram = LCD_GetVRAMAddress();
 * int width, height;
 * LCD_GetSize(&width, &height);
 *
 * for (int y = 0; y < 50; ++y) {
 *     for (int x = 0; x < 30; ++x) {
 *         vram[(x + 10) + (y + 20) * width] = RGB_TO_RGB565(0x1F, 0x3B, 0x08);
 *     }
 * }
 *
 * // Put our changes on the display
 * LCD_Refresh();
 * @endcode
 */

#pragma once
#include <stdint.h>

/**
 * @defgroup palette_colors Palette Colors
 * Passed to @ref LCD_SetPixelFromPalette.
 * @{
 */
const uint8_t PALETTE_BLACK = 0;
const uint8_t PALETTE_BLUE = 1;
const uint8_t PALETTE_GREEN = 2;
const uint8_t PALETTE_CYAN = 3;
const uint8_t PALETTE_RED = 4;
const uint8_t PALETTE_MAGENTA = 5;
const uint8_t PALETTE_YELLOW = 6;
const uint8_t PALETTE_WHITE = 7;
/// @}

/**
 * Converts three RGB values into one RGB565 value.
 *
 * @param r The red component, between 0 and 31 (0x1F) inclusive.
 * @param g The green component, between 0 and 63 (0x3F) inclusive.
 * @param b The blue component, between 0 and 31 (0x1F) inclusive.
 * @return The specified color, in RGB565 format.
 */
#define RGB_TO_RGB565(r, g, b) ( \
	((r & 0x1F) << 11) | \
	((g & 0x3F) << 5) | \
	(b & 0x1F) \
)

/**
 * Extracts the red component from an RGB565 value.
 *
 * @param rgb565 The RGB565 value.
 * @return The red component.
 */
#define RGB565_TO_R(rgb565) ((rgb565 >> 11) & 0x1F)

/**
 * Extracts the green component from an RGB565 value.
 *
 * @param rgb565 The RGB565 value.
 * @return The green component.
 */
#define RGB565_TO_G(rgb565) ((rgb565 >> 5) & 0x3F)

/**
 * Extracts the blue component from an RGB565 value.
 *
 * @param rgb565 The RGB565 value.
 * @return The blue component.
 */
#define RGB565_TO_B(rgb565) (rgb565 & 0x1F)

/**
 * Clears the LCD. Fills VRAM with white, but does not refresh the LCD.
 */
extern "C"
void LCD_ClearScreen();

/**
 * Returns the color of a pixel. This is not necessarily the color which is
 * being displayed on the LCD, but instead is the color specified for the pixel
 * in the VRAM buffer. Color is in RGB565 format.
 * 
 * @param x,y The coordinates of the pixel.
 * @return The color of the pixel, in RGB565 format.
 */
extern "C"
uint16_t LCD_GetPixel(int x, int y);

/**
 * Retrieves the size, in pixels, of the LCD.
 * 
 * @param[out] width,height The LCD's size.
 */
extern "C"
void LCD_GetSize(int *width, int *height);

/**
 * Returns a pointer to the video RAM. Video RAM is composed of
 * <tt>width * height</tt> 16-bit integers (in row-major order) representing the
 * color at each pixel, in RGB565 format.
 *
 * @return A pointer to the VRAM.
 */
extern "C"
uint16_t *LCD_GetVRAMAddress();

/**
 * Pushes the content of the VRAM to the LCD.
 */
extern "C"
void LCD_Refresh();

/**
 * Sets the color of a pixel. The result of this operation will not be visible
 * until @ref LCD_Refresh is called. Color is in RGB565 format.
 * 
 * @param x,y The coordinate of the pixel.
 * @param color The color to set the pixel, in RGB565 format.
 */
extern "C"
void LCD_SetPixel(int x, int y, uint16_t color);

/**
 * Sets the color of a pixel, from a pre-defined palette. Result is not visible
 * until @ref LCD_Refresh is called. See @ref palette_colors.
 *
 * @param x,y The coordinate of the pixel.
 * @param index The index of the color in the palette to use.
 */
extern "C"
void LCD_SetPixelFromPalette(int x, int y, uint8_t index);

/**
 * Backs up the current contents of VRAM.
 * 
 * Should be used to prevent display corruption when writing directly to VRAM.
 * 
 * Used in conjunction with @ref LCD_VRAMRestore.
 */
extern "C"
void LCD_VRAMBackup();

/**
 * Restores the backed up contents of VRAM. The restored content is not
 * displayed until @ref LCD_Refresh is called.
 * 
 * Used in conjunction with @ref LCD_VRAMBackup.
 */
extern "C"
void LCD_VRAMRestore();
