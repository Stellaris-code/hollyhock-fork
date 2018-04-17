#ifndef _SDK_SYSTEM_LCD_H
#define _SDK_SYSTEM_LCD_H
#include <stdint.h>

/**
 * Returns the color of a pixel. This is not necissailly the color which is
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

#endif
