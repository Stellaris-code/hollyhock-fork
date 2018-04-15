#ifndef _SDK_UTIL_H
#define _SDK_UTIL_H
#include <stdint.h>
#include <sdk/system/debug.h>

char *DIGITS = "0123456789ABCDEF";

void print_number_hex(uint32_t v) {
	char digits[32] = { 0 };

	int idx = 0;

	// Use a do-while so that if v == 0, then a zero is printed
	do {
		digits[idx] = DIGITS[v % 16];
		v /= 16;

		++idx;
	} while (v != 0);

	char *str = "?";
	for (int i = idx - 1; i >= 0; --i) {
		str[0] = digits[i];
		Debug_PrintString(str, false);
	}
}

void print_number_dec(uint32_t v) {
	char digits[32] = { 0 };

	int idx = 0;

	// Use a do-while so that if v == 0, then a zero is printed
	do {
		digits[idx] = DIGITS[v % 10];
		v /= 10;

		++idx;
	} while (v != 0);

	char *str = "?";
	for (int i = idx - 1; i >= 0; --i) {
		str[0] = digits[i];
		Debug_PrintString(str, false);
	}
}

#endif
