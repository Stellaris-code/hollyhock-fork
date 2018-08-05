/**
 * @file
 * @brief String manipulation functions.
 * 
 * For documentation, see the C standard library.
 */
#pragma once

extern "C"
char *strcat(char *dest, const char *src);

extern "C"
const char *strchr(const char *str, char c);

extern "C"
int strcmp(const char *str1, const char *str2);

extern "C"
char *strcpy(char *destination, const char *source);

extern "C"
int strlen(const char *str);
