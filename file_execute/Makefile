AS:=sh4-elf-as
AS_FLAGS:=

OBJCOPY:=sh4-elf-objcopy
OBJCOPY_FLAGS:=-O binary

all: file_execute.bin Makefile

clean:
	rm -f file_execute.o file_execute.bin

file_execute.bin: file_execute.o
	$(OBJCOPY) $(OBJCOPY_FLAGS) file_execute.o file_execute.bin

file_execute.o: file_execute.s
	$(AS) $< -o $@ $(AS_FLAGS)

.PHONY: all clean
