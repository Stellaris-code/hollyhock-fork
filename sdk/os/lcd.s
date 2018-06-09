.global _LCD_ClearScreen
.set _LCD_ClearScreen, 0x800394C0

.global _LCD_GetPixel
.set _LCD_GetPixel, 0x80039328

.global _LCD_GetSize
.set _LCD_GetSize, 0x8002E140

.global _LCD_GetVRAMAddress
.set _LCD_GetVRAMAddress, 0x8002E154

.global _LCD_Refresh
.set _LCD_Refresh, 0x8003733E

.global _LCD_SetPixel
.set _LCD_SetPixel, 0x80039302

.global _LCD_SetPixelFromPalette
.set _LCD_SetPixelFromPalette, 0x8003931A
