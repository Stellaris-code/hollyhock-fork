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

#define OPEN_READ (1 << 0)
#define OPEN_WRITE (1 << 1)
#define OPEN_CREATE (1 << 2)
#define OPEN_APPEND (1 << 4)

#define FSTAT_DATE_YEAR(date) (((date >> 9) & 0b1111111) + 1980)
#define FSTAT_DATE_MONTH(date) ((date >> 5) & 0b1111)
#define FSTAT_DATE_DAY(date) (date & 0b11111)

#define FSTAT_TIME_HOUR(time) ((time >> 11) & 0b11111)
#define FSTAT_TIME_MINUTE(time) ((time >> 5) & 0b111111)
#define FSTAT_TIME_SECOND(time) ((time & 0b11111) * 2)

struct stat {
	uint32_t unknown1;
	uint32_t fileSize;
	uint16_t creationDate;
	uint16_t creationTime;
	uint16_t lastModifiedDate;
	uint16_t lastModifiedTime;
	uint16_t unknown2;
	uint16_t lastAccessedDate;
};

int (*const close)(int fd) = (void *) 0x80057912;
int (*const fstat)(int fd, struct stat *buf) = (void *) 0x8005798E;
int (*const mkdir)(char *path) = (void *) 0x80057814;
int (*const open)(char *path, int flags) = (void *) 0x80057854;
int (*const read)(int fd, uint8_t *buf, int count) = (void *) 0x800578A2;
int (*const remove)(char *path) = (void *) 0x8005794E;
int (*const rename)(char *oldPath, char *newPath) = (void *) 0x80057A06;
int (*const stat)(char *path, struct stat *buf) = (void *) 0x800579C2;
int (*const write)(int fd, uint8_t *buf, int count) = (void *) 0x800578DA;

#endif
