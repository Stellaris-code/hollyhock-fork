AS:=sh4-elf-as
AS_FLAGS:=

CC:=sh4-elf-gcc
CC_FLAGS:=-ffreestanding -Wall -Wextra -O2 -I sdk/include/

LD:=sh4-elf-ld
LD_FLAGS:=-nostdlib

AS_SOURCES:=$(wildcard *.s) $(wildcard sdk/*.s)

CC_SOURCES:=$(wildcard *.c) $(wildcard sdk/*.c)

OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o)

all: run.bin Makefile

clean:
	rm -f $(OBJECTS) run.bin

run.bin: $(OBJECTS) sdk/linker.ld
	$(LD) -T sdk/linker.ld -o $@ $(LD_FLAGS) $(OBJECTS)

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

.PHONY: all clean
