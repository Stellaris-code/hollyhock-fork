#ifndef _SDK_SYSTEM_GUI_H
#define _SDK_SYSTEM_GUI_H
#include <stdbool.h>

#define BUTTON_OK 1 << 5
#define BUTTON_YES 1 << 6
#define BUTTON_NO 1 << 7
#define BUTTON_ABORT 1 << 8
#define BUTTON_RETRY 1 << 9
#define BUTTON_CANCEL 1 << 10

void (*const GUI_DisplayMessageBox)(int unknown, int titleStringID, int contentStringID) = (void *) 0x80146A38;
void *(*const GUI_DisplayMessageBox_Internal)(
	int unknown,
	char *titleString,
	char *contentPrefix, char *contentString,
	int buttons, bool disableCloseButton
) = (void *) 0x800B7914;

#endif
