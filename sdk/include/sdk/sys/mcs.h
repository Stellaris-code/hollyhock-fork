#ifndef _SDK_SYSTEM_MCS_H
#define _SDK_SYSTEM_MCS_H

void (*const MCS_List_GetValue)(char *folder, char *name, int count, int index, uint32_t *unknown, uint8_t *buf) = (void *) 0x8012B17E;
int (*const MCS_GetVariableSize)(uint8_t *unused, char *folder, char *name) = (void *) 0x8012EF9A;
int (*const MCS_GetVariableInfo)(char *folder, char *name, uint8_t *variableType, char **name2, uint8_t **data, uint32_t *size) = (void *) 0x80076A82;
int (*const MCS_List_GetNumEntries)(char *folder, char *name, uint32_t *size, uint8_t **data, char **name2, uint16_t *numEntries) = (void *) 0x8012B442;

#endif
