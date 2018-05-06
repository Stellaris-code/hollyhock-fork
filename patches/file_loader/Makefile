AS:=sh4-elf-as
AS_FLAGS:=

OBJCOPY:=sh4-elf-objcopy
OBJCOPY_FLAGS:=-O binary

all: file_loader.bin Makefile

clean:
	rm -f file_loader.o file_loader.bin

file_loader.bin: file_loader.o
	$(OBJCOPY) $(OBJCOPY_FLAGS) file_loader.o file_loader.bin

file_loader.o: file_loader.s
	$(AS) $< -o $@ $(AS_FLAGS)

.PHONY: all clean
