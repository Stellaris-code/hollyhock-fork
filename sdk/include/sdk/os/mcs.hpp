/**
 * @file
 * @brief Functions for retrieving and setting data in the MCS.
 */

#pragma once
#include <stdint.h>

/**
 * Variable type: %OBCD (@c struct @ref OBCD).
 */
const uint8_t VARTYPE_OBCD = 0x01;

/**
 * Variable type: %CBCD (@c struct @ref CBCD).
 */
const uint8_t VARTYPE_CBCD = 0x02;

/**
 * Variable type: null-terminated string.
 */
const uint8_t VARTYPE_STR = 0x05;

/**
 * Variable type: list.
 */
const uint8_t VARTYPE_LIST = 0x0A;

/**
 * Variable type: matrix.
 */
const uint8_t VARTYPE_MAT = 0x0C;

/**
 * Variable type: program.
 */
const uint8_t VARTYPE_PRGM = 0x47;

/**
 * Variable type: function.
 */
const uint8_t VARTYPE_FUNC = 0x48;

/**
 * Variable type: geometry data.
 */
const uint8_t VARTYPE_GEO = 0x54;

/**
 * Error code signaling that the variable does not exist.
 */
const int MCS_NO_VARIABLE = 0x30;

/**
 * Error code signaling that the folder does not exist.
 */
const int MCS_NO_FOLDER = 0x40;

/**
 * Error code signaling that the folder already exists.
 */
const int MCS_FOLDER_EXISTS = 0x42;

/**
 * Error code signaling that the specified size was not a power of 2.
 */
const int MCS_SIZE_NOT_PO2 = 0x61;

/**
 * Error code signaling that the variable was not a list.
 */
const int MCS_NOT_LIST = 0x62;

/**
 * Error code signaling that the index was out of bounds.
 */
const int MCS_INDEX_OOB = 0x63;

/**
 * Retrieves the BCD digit at position @c i (0 representing the least
 * significant place) from a BCD encoded number @c n.
 *
 * @param n The number to retrieve a digit from.
 * @param i The index of the digit to retrieve.
 */
#define GET_BCD_DIGIT(n, i) (((n) >> ((i) * 4)) & 0xF)

/**
 * Stores a decimal number. Both the mantissa and exponent are stored in BCD.
 *
 * See ClassPad 300 SDK documentation for more details.
 */
struct OBCD {
	uint8_t mantissa[10];
	uint16_t exponent;
};

/**
 * Stores a complex number, with real part @c re and imaginary part @c im.
 *
 * See ClassPad 300 SDK documentation for more details.
 */
struct CBCD {
	struct OBCD re;
	struct OBCD im;
};

/**
 * Creates a folder in the MCS.
 *
 * If the folder name is greater than the 8 character max, it is truncated to a
 * valid length.
 *
 * The @p folderIndex parameter is still populated if the folder already exists.
 * However, its purpose is currently unknown.
 *
 * @param[in] folder The name of the folder to create.
 * @param[out] folderIndex An unknown index for the folder.
 * @return 0 if the folder was created successfully, or @c MCS_FOLDER_EXISTS if
 * the folder already exists.
 */
extern "C"
int MCS_CreateFolder(const char *folder, uint8_t *folderIndex);

/**
 * Retrieves information about a variable stored in the MCS.
 *
 * @param[in] folder The folder the requested MCS variable is located in.
 * @param[in] name The name of the requested MCS variable.
 * @param[out] variableType The variable's type.
 * @param[out] name2 The variable's name.
 * @param[out] data A pointer to the variable's data.
 * @param[out] size The length of the variable's data.
 * @return 0 if the variable exists, @c MCS_NO_VARIABLE if the variable does not
 * exist, or @c MCS_NO_FOLDER if the folder does not exist.
 */
extern "C"
int MCS_GetVariable(
	const char *folder, const char *name,
	uint8_t *variableType, char **name2, void **data, uint32_t *size
);

/**
 * Creates a list variable in the MCS.
 *
 * The list is initially populated with the default value of the variable type
 * specified. If the variable already exists, it is overwritten (even if it's
 * not a list).
 * 
 * @param[in] folder The folder to create the list in.
 * @param[in] name The desired name of the variable.
 * @param size The size of the variable type specified.
 * @param length The number of entries in the list.
 * @param variableType The variable type to initialize the list with (macros
 * starting with @c VARTYPE).
 * @return 0 on success, @c MCS_NO_FOLDER if the requested folder does not
 * exist.
 */
extern "C"
int MCS_List_Create(
	const char *folder, const char *name,
	uint32_t size, uint16_t length, uint8_t variableType
);

/**
 * Sets an element of a list in the MCS.
 *
 * @param[in] folder The folder containing the list variable.
 * @param[in] name The name of the list variable.
 * @param size The size of the data to store in the element.
 * @param index The index into the list of the element to modify.
 * @param variableType The type of the data to be stored in the list element
 * (macros starting with @c VARTYPE).
 * @param[in] data The data to copy into the list element.
 * @return 0 on success, @c MCS_NO_FOLDER if the requested folder does not
 * exist, @c MCS_NO_VARIABLE if the variable does not exist, @c MCS_NOT_LIST
 * if the variable was not a list, or @c MCS_INDEX_OOB if the index was out of
 * bounds.
 */
extern "C"
int MCS_List_Set(
	const char *folder, const char *name,
	uint32_t size, int index, uint8_t variableType, void *data
);

/**
 * Sets the value of a variable stored in the MCS. Can be used to create a new
 * variable, or replace the value of an existing variable.
 *
 * This function does not create the folder if it does not exist. Use
 * @ref MCS_CreateFolder to create a folder if necessary.
 *
 * @param[in] folder The folder the variable to set it stored in.
 * @param[in] name The name of the desired MCS variable.
 * @param variableType The type of the variable (macros starting with
 * @c VARTYPE).
 * @param size The size of the variable's data.
 * @param[in] data A pointer to the data to copy into the variable.
 * @return 0 if the variable was set successfully, or @c MCS_NO_FOLDER if the
 * specified folder does not exist.
 */
extern "C"
int MCS_SetVariable(
	const char *folder, const char *name,
	uint8_t variableType, uint32_t size, void *data
);
