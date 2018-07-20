#include <sdk/os/debug.hpp>
#include <sdk/os/file.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/lcd.hpp>
#include <sdk/os/mem.hpp>
#include "elf.h"

int y;

#define printf(format, ...) do { \
	Debug_Printf(0, y++, false, 0, format, ## __VA_ARGS__); \
	LCD_Refresh(); \
} while (0)

class File {
public:
	File() : m_opened(false), m_fd(-1) {

	}

	~File() {
		if (m_opened) {
			close(m_fd);
		}
	}

	int open(const char *path, int flags) {
		m_fd = ::open(path, flags);
		m_opened = true;
		return m_fd;
	}

	int getAddr(int offset, const void **addr) {
		return ::getAddr(m_fd, offset, addr);
	}

private:
	bool m_opened;
	int m_fd;
};

bool strcmp(const char *str1, const char *str2) {
	while (*str1 == *str2) {
		// if we've reached the end of one or both strings
		if (*str1 == '\0' || *str2 == '\0') {
			// return true if we're at the end of both (i.e. they matched) and
			// false if we didn't
			return *str1 == '\0' && *str2 == '\0';
		}

		str1++;
		str2++;
	}

	return false;
}

void main() {
	y = 0;
	int ret;

	File f;
	ret = f.open("\\fls0\\app.hhk", OPEN_READ);
	if (ret < 0) {
		printf("Error: Could not open app.hhk (%d)", ret);
		return;
	}

	const Elf32_Ehdr *elf;
	ret = f.getAddr(0, (const void **) &elf);
	if (ret < 0) {
		printf("Could not getAddr (%d)", ret);
		return;
	}

	// Check magic number
	if (!(
		elf->e_ident[EI_MAG0] == ELFMAG0 &&
		elf->e_ident[EI_MAG1] == ELFMAG1 &&
		elf->e_ident[EI_MAG2] == ELFMAG2 &&
		elf->e_ident[EI_MAG3] == ELFMAG3
	)) {
		printf("Not an ELF - invalid magic");
		return;
	}

	// Check file class
	if (elf->e_ident[EI_CLASS] != ELFCLASS32) {
		printf("Invalid ELF - not 32-bit objects");
		return;
	}

	// Check data encoding
	if (elf->e_ident[EI_DATA] != ELFDATA2MSB) {
		printf("Invalid ELF - not 2's complement big endian");
		return;
	}

	// Check ELF version
	if (elf->e_ident[EI_VERSION] != EV_CURRENT) {
		printf("Invalid ELF - not current version (e_ident[EI_VERSION])");
		return;
	}

	// Check ABI (ignore ABI version EI_ABIVERSION)
	if (elf->e_ident[EI_OSABI] != ELFOSABI_SYSV) {
		printf("Invalid ELF - incorrect ABI");
		return;
	}

	// Check ELF is an executable file
	if (elf->e_type != ET_EXEC) {
		printf("Invalid ELF - not an executable file");
		return;
	}

	// Check machine
	if (elf->e_machine != EM_SH) {
		printf("Invalid ELF - wrong machine type");
		return;
	}

	// Check version
	if (elf->e_version != EV_CURRENT) {
		printf("Invalid ELF - not current version (e_version)");
	}

	const Elf32_Shdr *sectionHeaders = reinterpret_cast<const Elf32_Shdr *>(
		reinterpret_cast<const uint8_t *>(elf) + elf->e_shoff
	);

	const Elf32_Shdr *sectionHeaderStringTable = &sectionHeaders[elf->e_shstrndx];

	const char *appName = nullptr;
	const char *appDescription = nullptr;
	const char *appAuthor = nullptr;
	const char *appVersion = nullptr;

	for (int i = 0; i < elf->e_shnum; ++i) {
		const Elf32_Shdr *sectionHeader = &sectionHeaders[i];

		// skip the first empty section header
		if (sectionHeader->sh_type == SHT_NULL) {
			continue;
		}

		const char *sectionName = reinterpret_cast<const char *>(
			reinterpret_cast<const uint8_t *>(elf) +
			sectionHeaderStringTable->sh_offset +
			sectionHeader->sh_name
		);

		const void *sectionData = reinterpret_cast<const void *>(
			reinterpret_cast<const uint8_t *>(elf) +
			sectionHeader->sh_offset
		);

		printf(
			"%s: %#010x (len %#010x) ",
			sectionName, sectionHeader->sh_addr, sectionHeader->sh_size
		);

		if (strcmp(sectionName, ".hollyhock_name")) {
			appName = static_cast<const char *>(sectionData);
		} else if (strcmp(sectionName, ".hollyhock_description")) {
			appDescription = static_cast<const char *>(sectionData);
		} else if (strcmp(sectionName, ".hollyhock_author")) {
			appAuthor = static_cast<const char *>(sectionData);
		} else if (strcmp(sectionName, ".hollyhock_version")) {
			appVersion = static_cast<const char *>(sectionData);
		}

		if ((sectionHeader->sh_flags & SHF_ALLOC) == SHF_ALLOC) {
			void *dest = reinterpret_cast<void *>(sectionHeader->sh_addr);

			if (sectionHeader->sh_type == SHT_PROGBITS) {
				printf("    copying");
				memcpy(dest, sectionData, sectionHeader->sh_size);
			} else if (sectionHeader->sh_type == SHT_NOBITS) {
				printf("    zeroing");
				memset(dest, 0, sectionHeader->sh_size);
			}
		}
	}

	if (appName) printf("App name: %s", appName);
	if (appDescription) printf("App description: %s", appDescription);
	if (appAuthor) printf("App author: %s", appAuthor);
	if (appVersion) printf("App version: %s", appVersion);

	printf("Entry point %#010x - calling", elf->e_entry);

	__asm__ volatile(
		"jsr @%0\n"
		"nop\n"
		: // no outputs
		: "r"(elf->e_entry)
		: "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"
	);

	printf("App exited - press to finish");

	Debug_WaitKey();
}

extern "C"
void *main_bootstrap() {
	main();

	// Returning the output of this function is necessary to allow the
	// OS to resume execution once our code has finished running.
	// Removing this line (i.e. not returning the output of this function) will
	// cause the GC to lock up when the program's execution finishes.
	// This is only because of the current location of the file execution stub.
	return GUI_DisplayMessageBox_Internal(
		0,
		"Program",
		"run.bin", "The program has finished execution.",
		0, false
	);
}
