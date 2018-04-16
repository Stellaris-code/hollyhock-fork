#ifndef _SDK_SYSTEM_LCD_H
#define _SDK_SYSTEM_LCD_H
#include <stdint.h>

uint16_t (*const LCD_GetPixel)(int x, int y) = (void *) 0x80039328;
void (*const LCD_GetSize)(int *width, int *height) = (void *) 0x8002E140;
uint16_t *(*const LCD_GetVRAMAddress)() = (void *) 0x8002E154;
void (*const LCD_Refresh)() = (void *) 0x8003733E;
void (*const LCD_SetPixel)(int x, int y, uint16_t color) = (void *) 0x80039302;

#endif
