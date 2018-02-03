# File Functions
## `int open(char *path, int flags)`
Opens a file on the file system, and returns a file descriptor representing the open file.

The `flags` argument appears to be a simple bitfield with the following meanings:

<table>
	<tr>
		<td><b>Bit</b></td>
		<td><b>7</b></td>
		<td><b>6</b></td>
		<td><b>5</b></td>
		<td><b>4</b></td>
		<td><b>3</b></td>
		<td><b>2</b></td>
		<td><b>1</b></td>
		<td><b>0</b></td>
	</tr>
	<tr>
		<td><b>Value</b></td>
		<td></td>
		<td></td>
		<td></td>
		<td>Append</td>
		<td></td>
		<td>Create</td>
		<td>Write</td>
		<td>Read</td>
	</tr>
</table>

Returns 0 on success. On failure, returns a negative error code.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x80057854` |

## `int fstat(int fd, struct stat *buf)`
Retrieves information about an open file. `buf` is filled with data such as the file size, and creation/modification dates. The full structure is not yet known.

Returns 0 on success. On failure, returns a negative error code.

```
struct {
	uint32_t unknown;
	uint32_t file_size;
	uint16_t creation_date;
	uint16_t creation_time;
	uint16_t last_modified_date;
	uint32_t last_modified_time;
	uint16_t unknown;
	uint16_t last_accessed_date;
} stat;
```

Dates are stored as follows:
<table>
	<tr>
		<td><b>Bit</b></td>
		<td><b>15</b></td>
		<td><b>14</b></td>
		<td><b>13</b></td>
		<td><b>12</b></td>
		<td><b>11</b></td>
		<td><b>10</b></td>
		<td><b>9</b></td>
		<td><b>8</b></td>
		<td><b>7</b></td>
		<td><b>6</b></td>
		<td><b>5</b></td>
		<td><b>4</b></td>
		<td><b>3</b></td>
		<td><b>2</b></td>
		<td><b>1</b></td>
		<td><b>0</b></td>
	</tr>
	<tr>
		<td><b>Value</b></td>
		<td colspan='7'>Year - 1980</td>
		<td colspan='4'>Month (indexed from 1)</td>
		<td colspan='5'>Day (indexed from 1)</td>
	</tr>
</table>

Times are stored as follows:
<table>
	<tr>
		<td><b>Bit</b></td>
		<td><b>15</b></td>
		<td><b>14</b></td>
		<td><b>13</b></td>
		<td><b>12</b></td>
		<td><b>11</b></td>
		<td><b>10</b></td>
		<td><b>9</b></td>
		<td><b>8</b></td>
		<td><b>7</b></td>
		<td><b>6</b></td>
		<td><b>5</b></td>
		<td><b>4</b></td>
		<td><b>3</b></td>
		<td><b>2</b></td>
		<td><b>1</b></td>
		<td><b>0</b></td>
	</tr>
	<tr>
		<td><b>Value</b></td>
		<td colspan='5'>Hour</td>
		<td colspan='6'>Minute</td>
		<td colspan='5'>Second (divided by two, rounded down)</td>
	</tr>
</table>

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x8005798E` |

## `int read(int fd, uint8_t *buf, int count)`
Reads up to `count` bytes from an open file and stores them in `buf`.

Returns the number of bytes read on success. On failure, returns a negative error code.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x800578A2` |

## `int write(int fd, uint8_t *buf, int count)`
Writes `count` bytes to a file from `buf`.

Returns the number of bytes written on success. On failure, returns a negative error code.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x800578DA` |

## `int close(int fd)`
Closes an open file.

Returns 0 on success. On failure, returns a negative error code.

### Memory Location
| Version | Address |
| --- | --- |
| v02.01.2000.0000 | `0x80057912` |

# Error Constants
Note: these values were found in the `PrintStandardErrorString` function, located at `0x80065998` in v02.01.2000.0000. There may be more which aren't documented.

| Human-Readable Name | Error Code (unsigned) | Error Code (signed) |
| --- | --- | --- |
| `ENOMEM` | `0xFFFFFFFF` | -1 |
| `EINVAL` | `0xFFFFFFFE` | -2 |
| `EDEVFAIL` | `0xFFFFFFFD` | -3 |
| `EMOUNTED` | `0xFFFFFFFC` | -4 |
| `EACCES` | `0xFFFFFFFB` | -5 |
| `EBADFSID` | `0xFFFFFFFA` | -6 |
| `ENOVOLUME` | `0xFFFFFFF9` | -7 |
| `ENOPATH` | `0xFFFFFFF8` | -8 |
| `EEXIST` | `0xFFFFFFF7` | -9 |
| `ENAMETOOLONG` | `0xFFFFFFF6` | -10 |
| `EOUTOFBOUND` | `0xFFFFFFF5` | -11 |
| `EUNFORMAT` | `0xFFFFFFF4` | -12 |
| `ENOSPC` | `0xFFFFFFF3` | -13 |
| `ENOENT` | `0xFFFFFFF2` | -14 |
| `EISDIRECTORY` | `0xFFFFFFF1` | -15 |
| `ESHARE` | `0xFFFFFFF0` | -16 |
| `EMFILE` | `0xFFFFFFEF` | -17 |
| `EBADF` | `0xFFFFFFEE` | -18 |
| `EEOF` | `0xFFFFFFED` | -19 |
| `ENOTEMPTY` | `0xFFFFFFEC` | -20 |
| `ECLUSTERSIZEMISMATCH` | `0xFFFFFFD8` | -40 |
| `ESYSTEM` | `0xFFFFFF9D` | -99 |
