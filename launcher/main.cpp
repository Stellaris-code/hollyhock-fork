#include <sdk/os/file.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/mem.hpp>
#include <sdk/os/string.hpp>
#include <sdk/os/lcd.hpp>
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

class Find {
public:
	Find() : m_opened(false), m_findHandle(-1) {

	}

	~Find() {
		if (m_opened) {
			findClose(m_findHandle);
		}
	}

	int findFirst(const wchar_t *path, wchar_t *name, struct findInfo *findInfoBuf) {
		int ret = ::findFirst(path, &m_findHandle, name, findInfoBuf);
		m_opened = true;
		return ret;
	}

	int findNext(wchar_t *name, struct findInfo *findInfoBuf) {
		return ::findNext(m_findHandle, name, findInfoBuf);
	}

private:
	bool m_opened;
	int m_findHandle;
};

class Launcher : public GUIDialog {
public:
    int m_numApps = 0;
    struct {
        char fileName[100];
        char path[200];
    } m_apps[64];
    int m_selectedApp = 0;

    Launcher() : GUIDialog(
        GUIDialog::Height35, GUIDialog::AlignTop,
        "Hollyhock Launcher",
        GUIDialog::KeyboardStateNone
    ), m_appNames(
        GetLeftX() + 10, GetTopY() + 10, GetRightX() - 10, GetBottomY() - 10,
        APP_NAMES_EVENT_ID
    ), m_run(
        GetLeftX() + 10, GetTopY() + 45, GetLeftX() + 10 + 100, GetTopY() + 45 + 35,
        "Run", RUN_EVENT_ID
    ), m_close(
        GetRightX() - 10 - 100, GetTopY() + 45, GetRightX() - 10, GetTopY() + 45 + 35,
        "Close", CLOSE_EVENT_ID
    ) {
        FindApps();
        PopulateDropDown();

        m_appNames.SetScrollBarVisibility(
            GUIDropDownMenu::ScrollBarVisibleWhenRequired
        );
        AddElement(m_appNames);
        
        AddElement(m_run);
        AddElement(m_close);
    }

    void FindApps() {
        const char *basePath = "\\fls0\\";

        Find find;

        wchar_t fileName[100];
        struct findInfo findInfoBuf;
        int ret = find.findFirst(L"\\fls0\\*.hhk", fileName, &findInfoBuf);

        while (ret >= 0) {
            if (findInfoBuf.type == findInfoBuf.EntryTypeFile) {
                memset(&m_apps[m_numApps], 0, sizeof(m_apps[m_numApps]));

                // copy the file name (converting to a non-wide string in the
                // process)
                for (int i = 0; i < 100; ++i) {
                    wchar_t c = fileName[i];
                    m_apps[m_numApps].fileName[i] = c;
                    if (c == 0x0000) {
                        break;
                    }
                }

                // build the path
                strcat(m_apps[m_numApps].path, basePath);
                strcat(m_apps[m_numApps].path, m_apps[m_numApps].fileName);

                m_numApps++;
            }

            ret = find.findNext(fileName, &findInfoBuf);
        }
    }

    void PopulateDropDown() {
        for (int i = 0; i < m_numApps; ++i) {
            m_appNames.AddMenuItem(*(
                new GUIDropDownMenuItem(
                    m_apps[i].path, i + 1,
                    GUIDropDownMenuItem::FlagEnabled |
                    GUIDropDownMenuItem::FlagTextAlignLeft
                )
            ));
        }
    }

    virtual int OnEvent(GUIDialog_Wrapped *dialog, GUIDialog_OnEvent_Data *event) {
        if (event->GetEventID() == APP_NAMES_EVENT_ID) {
            m_selectedApp = event->data - 1;
        }

        return GUIDialog::OnEvent(dialog, event);
    }

private:
    const uint16_t APP_NAMES_EVENT_ID = 1;
    GUIDropDownMenu m_appNames;

    const uint16_t RUN_EVENT_ID = GUIDialog::DialogResultOK;
    GUIButton m_run;

    const uint16_t CLOSE_EVENT_ID = GUIDialog::DialogResultCancel;
    GUIButton m_close;
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

void RunELF(const char *path) {
    y = 0;
	int ret;

	File f;
	ret = f.open(path, OPEN_READ);
	if (ret < 0) {
		printf("Error: Could not open %s (%d)", path, ret);
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

	printf("Entry point %#010x - calling on key press", elf->e_entry);

    Debug_WaitKey();

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

void main() {
    Launcher launcher;
    if (launcher.ShowDialog() == GUIDialog::DialogResultOK) {
        RunELF(launcher.m_apps[launcher.m_selectedApp].path);
    }
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
