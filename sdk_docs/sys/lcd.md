# LCD Functions
## `uint16_t LCD_GetPixel(int x, int y)`
Returns the color of a pixel. This is not necessarily the color which is being displayed on the screen, but instead is the color in the VRAM buffer. Color is in RGB565 format.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x80039328` |

## `void LCD_GetSize(int *width, int *height)`
Retrieves the size, in pixels, of the LCD. The values of the variables pointed to by `width` and `height` are populated with the pixel width and height of the LCD respectively.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8002E140` |

## `uint16_t *LCD_GetVRAMAddress()`
Returns a pointer to the video RAM. Video RAM is composed of `width * height` 16-bit integers (in row-major order) representing the color at each pixel in RGB565 format.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8002E154` |

## `void LCD_Refresh()`
Pushes the contents of VRAM to the LCD.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8003733E` |

## `void LCD_SetPixel(int x, int y, uint16_t color)`
Sets the color of a pixel. The result of this operation will not be visible until `LCD_Refresh` is called. Color is in RGB565 format.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x80039302` |
