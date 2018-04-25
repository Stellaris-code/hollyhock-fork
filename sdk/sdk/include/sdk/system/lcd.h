#ifndef _SDK_SYSTEM_LCD_H
#define _SDK_SYSTEM_LCD_H
#include <stdint.h>

/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_BLACK 0
/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_BLUE 1
/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_GREEN 2
/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_CYAN 3
/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_RED 4
/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_MAGENTA 5
/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_YELLOW 6
/**
 * Palette color passed to @ref LCD_SetPixelFromPalette.
 */
#define PALETTE_WHITE 7

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
void LCD_ClearScreen();

/**
 * Returns the color of a pixel. This is not necessarily the color which is
 * being displayed on the LCD, but instead is the color specified for the pixel
 * in the VRAM buffer. Color is in RGB565 format.
 * 
 * @param x The X coordinate of the pixel.
 * @param y The Y coordinate of the pixel.
 * @return The color of the pixel, in RGB565 format.
 */
uint16_t LCD_GetPixel(int x, int y);

/**
 * Retrieves the size, in pixels, of the LCD.
 * 
 * @param width A pointer to an int to store the LCD's width in.
 * @param height A pointer to an int to store the LCD's height in.
 */
void LCD_GetSize(int *width, int *height);

/**
 * Returns a pointer to the video RAM. Video RAM is composed of
 * <tt>width * height</tt> 16-bit integers (in row-major order) representing the
 * color at each pixel, in RGB565 format.
 *
 * @return A pointer to the VRAM.
 */
uint16_t *LCD_GetVRAMAddress();

/**
 * Pushes the content of the VRAM to the LCD.
 */
void LCD_Refresh();

/**
 * Sets the color of a pixel. The result of this operation will not be visible
 * until @ref LCD_Refresh is called. Color is in RGB565 format.
 * 
 * @param x The X coordinate of the pixel.
 * @param y The Y coordinate of the pixel.
 * @param color The color to set the pixel, in RGB565 format.
 */
void LCD_SetPixel(int x, int y, uint16_t color);

/**
 * Sets the color of a pixel, from a pre-defined palette. Result is not visible
 * until @ref LCD_Refresh is called. Index is from macros beginning with
 * @c PALETTE_.
 *
 * @param x The X coordinate of the pixel.
 * @param y The Y coordinate of the pixel.
 * @param index The index of the color in the palette to use.
 */
void LCD_SetPixelFromPalette(int x, int y, uint8_t index);

#endif
