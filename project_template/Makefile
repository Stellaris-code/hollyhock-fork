ifndef SDK_DIR
$(error You need to define the SDK_DIR environment variable, and point it to the sdk/ folder)
endif

AS:=sh4-elf-as
AS_FLAGS:=

CC:=sh4-elf-g++
CC_FLAGS:=-ffreestanding -fno-exceptions -fno-rtti -Wall -Wextra -O2 -I $(SDK_DIR)/include/

LD:=sh4-elf-ld
LD_FLAGS:=-nostdlib --no-undefined

AS_SOURCES:=$(wildcard *.s)
CC_SOURCES:=$(wildcard *.cpp)
OBJECTS:=$(AS_SOURCES:.s=.o) $(CC_SOURCES:.cpp=.o)

all: run.bin Makefile

clean:
	rm -f $(OBJECTS) run.bin

run.bin: $(OBJECTS) $(SDK_DIR)/linker.ld $(SDK_DIR)/sdk.o
	$(LD) -T $(SDK_DIR)/linker.ld -o $@ $(LD_FLAGS) $(OBJECTS) $(SDK_DIR)/sdk.o

# We're not actually building sdk.o, just telling the user they need to do it
# themselves. Just using the target to trigger an error when the file is
# required but does not exist.
$(SDK_DIR)/sdk.o:
	$(error You need to build the SDK before using it. Run make in the SDK directory, and check the README.md in the SDK directory for more information)

%.o: %.s
	$(AS) $< -o $@ $(AS_FLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CC_FLAGS)

.PHONY: all clean
