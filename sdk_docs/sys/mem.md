# Memory Functions
## `void *memcpy(void *destination, void *source, int num)`
Copies one region of memory to another. Equivalent to the C standard library function with the same name.

Copies `num` bytes from `source` to `destination`. Returns `destination`.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x800A78AC` |

## `void *memset(void *ptr, int value, int num)`
Sets a region of memory to a specific value. Equivalent to the C standard library function with the same name.

Fills the region pointed to by `ptr` with `num` bytes of value `value` (zero-extended to a byte). Returns `ptr`.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x800A7FC0` |
