#ifndef _SDK_SYSTEM_DEBUG_H
#define _SDK_SYSTEM_DEBUG_H
#include <stdbool.h>

int (*const Debug_GetCursorPosition)(int *x, int *y) = (void *) 0x8002E448;
int (*const Debug_GetKey)() = (void *) 0x80094380;
void (*const Debug_PrintStandardErrorString)(int retVal, char *functionName) = (void *) 0x80065998;
bool (*const Debug_PrintString)(char *string, bool invert) = (void *) 0x8002DA0C;
int (*const Debug_SetCursorPosition)(int x, int y) = (void *) 0x8002E430;

#endif
