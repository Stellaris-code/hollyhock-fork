# Debug Functions
## `int Debug_SetCursorPosition(int x, int y)`
Sets the position of the cursor in debug text mode.

Always returns 0.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8002E430` |

## `int Debug_GetCursorPosition(int *x, int *y)`
Returns the current position of the cursor in debug text mode. Current X and Y positions are returned through pointers to `int`s.

Always returns 0.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8002E448` |

## `bool Debug_PrintString(char *string, bool invert)`
Prints a string in debug text mode, either in normal black-on-white or inverted white-on-black. Color inversion occurs if `invert == true`.

Returns `true` if writing was successful, `false` otherwise. A return value of 0 may mean the string was too long to fit on the display.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8002DA0C` |

## `void Debug_DrawScreen()`
Draws text to the screen. If not called, any strings printed with `Debug_PrintString` will not be visible.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8003733E` |

## `int Debug_GetKey()`
Waits until a key is pressed, then returns a number representing the key. Only appears to react to number keys and the Power/Clear key. Returns `0x30` to `0x39` for keys 0 to 9, and `0x98` for the Power/Clear key.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x80094380` |
