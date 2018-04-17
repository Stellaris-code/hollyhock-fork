#ifndef _SDK_SYSTEM_FILE_H
#define _SDK_SYSTEM_FILE_H
#include <stdint.h>

#define ENOMEM -1
#define EINVAL -2
#define EDEVFAIL -3
#define EMOUNTED -4
#define EACCES -5
#define EBADFSID -6
#define ENOVOLUME -7
#define ENOPATH -8
#define EEXIST -9
#define ENAMETOOLONG -10
#define EOUTOFBOUND -11
#define EUNFORMAT -12
#define ENOSPC -13
#define ENOENT -14
#define EISDIRECTORY -15
#define ESHARE -16
#define EMFILE -17
#define EBADF -18
#define EEOF -19
#define ENOTEMPTY -20
#define ECLUSTERSIZEMISMATCH -40
#define ESYSTEM -99

/**
 * Flag passed to @ref open. Specifies the file descriptor to be readable.
 */
#define OPEN_READ (1 << 0)
/**
 * Flag passed to @ref open. Specifies the file descriptor to be writable.
 */
#define OPEN_WRITE (1 << 1)
/**
 * Flag passed to @ref open. Specifies the file should be created if it does not
 * already exist.
 */
#define OPEN_CREATE (1 << 2)
/**
 * Flag passed to @ref open. Specifies the file descriptor's internal pointer
 * should begin at the end of the file.
 */
#define OPEN_APPEND (1 << 4)

/**
 * Retrieves the year from a @c struct @ref stat date field.
 * 
 * @param date The date field from a @c struct @ref stat.
 * 
 * @see struct stat
 */
#define FSTAT_DATE_YEAR(date) (((date >> 9) & 0b1111111) + 1980)

/**
 * Retrieves the month from a @c struct @ref stat date field.
 * 
 * @param date The date field from a @c struct @ref stat.
 * 
 * @see struct stat
 */
#define FSTAT_DATE_MONTH(date) ((date >> 5) & 0b1111)

/**
 * Retrieves the day from a @c struct @ref stat date field.
 * 
 * @param date The date field from a @c struct @ref stat.
 * 
 * @see struct stat
 */
#define FSTAT_DATE_DAY(date) (date & 0b11111)

/**
 * Retrieves the hour from a @c struct @ref stat time field.
 * 
 * @param time The time field from a @c struct @ref stat.
 * 
 * @see struct stat
 */
#define FSTAT_TIME_HOUR(time) ((time >> 11) & 0b11111)

/**
 * Retrieves the minute from a @c struct @ref stat time field.
 * 
 * @param time The time field from a @c struct @ref stat.
 * 
 * @see struct stat
 */
#define FSTAT_TIME_MINUTE(time) ((time >> 5) & 0b111111)

/**
 * Retrieves the second from a @c struct @ref stat time field.
 * 
 * @param time The time field from a @c struct @ref stat.
 * 
 * @see struct stat
 */
#define FSTAT_TIME_SECOND(time) ((time & 0b11111) * 2)

/**
 * Information about a file as retrieved from @ref fstat or @ref stat.
 * 
 * Dates and times are stored as a bitfield. The values they represent can be
 * accessed using the @ref FSTAT_DATE_YEAR, @ref FSTAT_DATE_MONTH,
 * @ref FSTAT_DATE_DAY, @ref FSTAT_TIME_HOUR, @ref FSTAT_TIME_MINUTE, and
 * @ref FSTAT_TIME_SECOND macros.
 *
 * Dates are stored in the following format:
 * <table>
 *     <tr>
 *         <td><b>Bit</b></td>
 *         <td><b>15</b></td>
 *         <td><b>14</b></td>
 *         <td><b>13</b></td>
 *         <td><b>12</b></td>
 *         <td><b>11</b></td>
 *         <td><b>10</b></td>
 *         <td><b>9</b></td>
 *         <td><b>8</b></td>
 *         <td><b>7</b></td>
 *         <td><b>6</b></td>
 *         <td><b>5</b></td>
 *         <td><b>4</b></td>
 *         <td><b>3</b></td>
 *         <td><b>2</b></td>
 *         <td><b>1</b></td>
 *         <td><b>0</b></td>
 *     </tr>
 *     <tr>
 *         <td><b>Value</b></td>
 *         <td colspan='7'>Year - 1980</td>
 *         <td colspan='4'>Month (indexed from 1)</td>
 *         <td colspan='5'>Day (indexed from 1)</td>
 *     </tr>
 * </table> 
 *
 * Times are stored in the following format:
 * <table>
 *     <tr>
 *         <td><b>Bit</b></td>
 *         <td><b>15</b></td>
 *         <td><b>14</b></td>
 *         <td><b>13</b></td>
 *         <td><b>12</b></td>
 *         <td><b>11</b></td>
 *         <td><b>10</b></td>
 *         <td><b>9</b></td>
 *         <td><b>8</b></td>
 *         <td><b>7</b></td>
 *         <td><b>6</b></td>
 *         <td><b>5</b></td>
 *         <td><b>4</b></td>
 *         <td><b>3</b></td>
 *         <td><b>2</b></td>
 *         <td><b>1</b></td>
 *         <td><b>0</b></td>
 *     </tr>
 *     <tr>
 *         <td><b>Value</b></td>
 *         <td colspan='5'>Hour</td>
 *         <td colspan='6'>Minute</td>
 *         <td colspan='5'>Second (divided by two, rounded down)</td>
 *     </tr>
 * </table>
 * 
 * @see stat
 * @see fstat
 */
struct stat {
	uint32_t unknown1;

	/**
	 * The size of the file, in bytes.
	 */
	uint32_t fileSize;

	/**
	 * The creation date of the file. For information on the storage format,
	 * see @c struct @ref stat.
	 */
	uint16_t creationDate;

	/**
	 * The creation time of the file. For information on the storage format,
	 * see @c struct @ref stat.
	 */
	uint16_t creationTime;

	/**
	 * The date the file was last modified. For information on the storage
	 * format, see @c struct @ref stat.
	 */
	uint16_t lastModifiedDate;

	/**
	 * The time the file was last modified. For information on the storage
	 * format, see @c struct @ref stat.
	 */
	uint16_t lastModifiedTime;

	uint16_t unknown2;

	/**
	 * The date the file was last accessed. For information on the storage
	 * format, see @c struct @ref stat.
	 */
	uint16_t lastAccessedDate;
};

/**
 * Closes an open file.
 *
 * @param fd The file descriptor for the open file.
 * @return 0 on success, or a negative error code on failure.
 */
int close(int fd);

/**
 * Retrieves information about an open file.
 *
 * @param fd The file descriptor of an open file to retrieve the information of.
 * @param buf A pointer to an @c struct @ref stat to store the retrieved information
 * in.
 * @return 0 on success, or a negative error code on failure.
 * 
 * @see stat
 */
int fstat(int fd, struct stat *buf);

/**
 * Creates a directory.
 *
 * @param path The path to the directory to be created.
 * @return 0 on success, or a negative error code on failure.
 */
int mkdir(char *path);

/**
 * Opens a file on the file system.
 * 
 * The argument @p flags is fed a bitwise OR'd combination of flags, defined by
 * macros beginning with @c OPEN_.
 *
 * @param path The path to the file to open.
 * @param flags A bitfield describing the mode in which to open the file.
 * @return A file descriptor on success, or a negative error code on failure.
 */
int open(char *path, int flags);

/**
 * Reads up to @c count bytes from a file, and stores them in @c buf.
 *
 * If @c count bytes cannot be read from the file, as many bytes as possible are
 * read (i.e. between 0 and @c count - 1 bytes are read and stored in @c buf).
 *
 * @param fd The file descriptor of an open file to read from.
 * @param buf A buffer to place the read bytes into.
 * @param count The maximum number of bytes to read.
 * @return The number of bytes read on success, or a negative error code on
 * failure.
 */
int read(int fd, uint8_t *buf, int count);

/**
 * Deletes a file or directory.
 *
 * @param path The path to the file or directory to be deleted.
 * @return 0 on success, or a negative error code on failure.
 */
int remove(char *path);

/**
 * Renames a file or directory.
 *
 * @param oldPath The path to the file or directory to be renamed.
 * @param newPath The path to the new name for the file or directory.
 * @return 0 on success, or a negative error code on failure.
 */
int rename(char *oldPath, char *newPath);

/**
 * Retrieves information about a file.
 *
 * @param path The path to the file to retrieve the information of.
 * @param buf A pointer to a @c struct @ref stat to store the retrieved information
 * in.
 * @return 0 on success, or a negative error code on failure.
 * 
 * @see fstat
 */
int stat(char *path, struct stat *buf);

/**
 * Writes @c count bytes from @c buf to an file.
 *
 * @param fd The file descriptor of an open file to write to.
 * @param buf A buffer of bytes to write to the file.
 * @param count The number of bytes from @c buf to write to the file.
 * @return 0 on success, or a negative error code on failure.
 */
int write(int fd, uint8_t *buf, int count);

#endif
