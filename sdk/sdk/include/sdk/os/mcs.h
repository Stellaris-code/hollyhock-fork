/**
 * @file
 * Functions for retrieving and setting data in the MCS.
 */

#ifndef _SDK_OS_MCS_H
#define _SDK_OS_MCS_H

void MCS_List_GetValue(char *folder, char *name, int count, int index, uint32_t *unknown, uint8_t *buf);
int MCS_GetVariableSize(uint8_t *unused, char *folder, char *name);
int MCS_GetVariableInfo(char *folder, char *name, uint8_t *variableType, char **name2, uint8_t **data, uint32_t *size);
int MCS_List_GetNumEntries(char *folder, char *name, uint32_t *size, uint8_t **data, char **name2, uint16_t *numEntries);

#endif
