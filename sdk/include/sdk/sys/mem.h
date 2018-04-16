#ifndef _SDK_SYSTEM_MCS_H
#define _SDK_SYSTEM_MCS_H

void *(*const memcpy)(void *destination, void *source, int num) = (void *) 0x800A78AC;
void *(*const memset)(void *ptr, int value, int num) = (void *) 0x800A7FC0;

#endif
