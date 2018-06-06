DOXYGEN=doxygen

AS:=sh4-elf-as
AS_FLAGS:=

# -r flag so the sdk.o file can be linked with the user's application object
# files: generates a relocatable object file
LD:=sh4-elf-ld
LD_FLAGS:=-nostdlib -r

AS_SOURCES:=$(wildcard *.s)
OBJECTS:=$(AS_SOURCES:.s=.o)

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

.PHONY: all docs clean
