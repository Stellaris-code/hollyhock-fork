DOXYGEN=doxygen

AS:=sh4-elf-as
AS_FLAGS:=

CC:=sh4-elf-gcc
CC_FLAGS:=-ffreestanding -Wall -Wextra -O2 -I include/

# -r flag so the sdk.o file can be linked with the user's application object
# files: generates a relocatable object file
LD:=sh4-elf-ld
LD_FLAGS:=-nostdlib -r

AS_SOURCES:=$(wildcard *.s) $(wildcard **/*.s)
CC_SOURCES:=$(wildcard *.c)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.c=.o)

all: sdk.o Makefile

docs:
	$(DOXYGEN)

clean:
	rm -f $(OBJECTS) sdk.o
	rm -rf doc/

sdk.o: $(OBJECTS)
	$(LD) -o $@ $(LD_FLAGS) $(OBJECTS)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CC_FLAGS)

.PHONY: all docs clean
