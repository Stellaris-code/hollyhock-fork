#include <appdef.hpp>
#include <stdint.h>
#include <sdk/cpu/ubc.hpp>
#include <sdk/os/debug.hpp>
#include <sdk/os/gui.hpp>
#include <sdk/os/lcd.hpp>

APP_NAME("Breakpoint Utility")
APP_DESCRIPTION("Set breakpoints within apps or OS code to view CPU state.")
APP_AUTHOR("The6P4C")
APP_VERSION("1.0.0")

struct CPUState {
	uint32_t r0;
	uint32_t r1;
	uint32_t r2;
	uint32_t r3;
	uint32_t r4;
	uint32_t r5;
	uint32_t r6;
	uint32_t r7;
	uint32_t r8;
	uint32_t r9;
	uint32_t r10;
	uint32_t r11;
	uint32_t r12;
	uint32_t r13;
	uint32_t r14;
	uint32_t r15;

	uint32_t gbr;
	uint32_t pr;

	uint32_t mach;
	uint32_t macl;
};

// Defined in breakpoint_handler_stub.s
extern "C" void BreakpointHandlerStub();

/**
 * Called by @c BreakpointHandlerStub when a breakpoint is triggered.
 *
 * @param cpuState The state of the CPU when the breakpoint was triggered.
 */
extern "C"
void BreakpointHandler(struct CPUState *cpuState) {
	Debug_SetCursorPosition(0, 0);
	Debug_PrintString("Breakpoint triggered!", true);
	
	Debug_SetCursorPosition(0, 1);
	Debug_PrintString("r0 =xxxxxxxx r1 =xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r0, 4, 1);
	Debug_PrintNumberHex_Dword(cpuState->r1, 17, 1);

	Debug_SetCursorPosition(0, 2);
	Debug_PrintString("r2 =xxxxxxxx r3 =xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r2, 4, 2);
	Debug_PrintNumberHex_Dword(cpuState->r3, 17, 2);

	Debug_SetCursorPosition(0, 3);
	Debug_PrintString("r4 =xxxxxxxx r5 =xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r4, 4, 3);
	Debug_PrintNumberHex_Dword(cpuState->r5, 17, 3);

	Debug_SetCursorPosition(0, 4);
	Debug_PrintString("r6 =xxxxxxxx r7 =xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r6, 4, 4);
	Debug_PrintNumberHex_Dword(cpuState->r7, 17, 4);

	Debug_SetCursorPosition(0, 5);
	Debug_PrintString("r8 =xxxxxxxx r9 =xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r8, 4, 5);
	Debug_PrintNumberHex_Dword(cpuState->r9, 17, 5);

	Debug_SetCursorPosition(0, 6);
	Debug_PrintString("r10=xxxxxxxx r11=xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r10, 4, 6);
	Debug_PrintNumberHex_Dword(cpuState->r11, 17, 6);

	Debug_SetCursorPosition(0, 7);
	Debug_PrintString("r12=xxxxxxxx r13=xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r12, 4, 7);
	Debug_PrintNumberHex_Dword(cpuState->r13, 17, 7);

	Debug_SetCursorPosition(0, 8);
	Debug_PrintString("r14=xxxxxxxx r15=xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->r14, 4, 8);
	Debug_PrintNumberHex_Dword(cpuState->r15, 17, 8);

	Debug_SetCursorPosition(0, 10);
	Debug_PrintString("gbr=xxxxxxxx pr =xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->gbr, 4, 10);
	Debug_PrintNumberHex_Dword(cpuState->pr, 17, 10);

	Debug_SetCursorPosition(0, 12);
	Debug_PrintString("mach=xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->mach, 5, 12);
	
	Debug_SetCursorPosition(0, 13);
	Debug_PrintString("macl=xxxxxxxx", false);
	Debug_PrintNumberHex_Dword(cpuState->macl, 5, 13);

	LCD_Refresh();
}

/**
 * Sets a breakpoint at @p address, triggered after an instruction at the
 * address is executed.
 * 
 * @param address The address to set the breakpoint at.
 */
void SetBreakpoint(uint32_t address) {
	// Set our breakpoint handler stub as the DBR (debug base register)
	__asm__ volatile(
		"ldc %0, dbr\n"
		:
		: "r"(&BreakpointHandlerStub)
		:
	);

	// Tell the UBC to use the function pointed to by DBR
	UBC_REG_CBCR = 1 << UBC_CBCR_UBDE;

	// Set the breakpoint to occur on an instruction fetch cycle
	UBC_REG_CBR0 = 1 << UBC_CBR_ID;

	// Set the address of the function we want to debug
	UBC_REG_CAR0 = address;

	// Set address mask to 0 (only trigger on the exact address)
	UBC_REG_CAMR0 = 0;

	// Enable the breakpoint and set the breakpoint to trigger after instruction
	// execution
	UBC_REG_CRR0 = (1 << UBC_CRR_RESERVED) | (1 << UBC_CRR_PCB) | (1 << UBC_CRR_BIE);

	// Enable the channel
	UBC_REG_CBR0 |= 1 << UBC_CBR_CE;
}

/**
 * Disables any breakpoint which was set.
 */
void RemoveBreakpoint() {
	// Disable the channel
	UBC_REG_CBR0 = 0 << UBC_CBR_CE;
}

class BreakpointDialog : public GUIDialog {
public:
	BreakpointDialog() : GUIDialog(
		Height55, AlignCenter,
		"Breakpoint Utility",
		KeyboardStateABC
	), m_breakpointAddressLabel(
		GetLeftX() + 10, GetTopY() + 10,
		"Breakpoint address (hex)"
	), m_breakpointAddress(
		GetLeftX() + 10, GetTopY() + 30, GetRightX() - GetLeftX() - 20,
		8, true 
	), m_setBreakpoint(
		GetLeftX() + 10, GetTopY() + 60, GetRightX() - 10, GetTopY() + 90,
		"Set breakpoint", BUTTON_SET_BREAKPOINT_EVENT_ID
	), m_removeBreakpoint(
		GetLeftX() + 10, GetTopY() + 95, GetRightX() - 10, GetTopY() + 125,
		"Remove breakpoint", BUTTON_REMOVE_BREAKPOINT_EVENT_ID
	), m_close(
		GetLeftX() + 10, GetTopY() + 130, GetRightX() - 10, GetTopY() + 160,
		"Close", BUTTON_CLOSE_EVENT_ID
	) {
		AddElement(m_breakpointAddressLabel);
		AddElement(m_breakpointAddress);
		AddElement(m_setBreakpoint);
		AddElement(m_removeBreakpoint);
		AddElement(m_close);
	}

	/**
	 * Returns the integer representation of the address the user entered.
	 * 
	 * Accepts both uppercase and lowercase hexidecimal.
	 * 
	 * @return The integer representation of the address the user entered, or 0
	 * if the address was invalid in some way.
	 */
	uint32_t GetBreakpointAddress() {
		const char *text = m_breakpointAddress.GetText();

		if (text == 0) {
			return 0;
		}

		char c;
		uint32_t address = 0;
		while ((c = *text++) != '\0') {
			uint32_t v;

			// Since 0-9, a-f and A-F are consecutive in the ASCII space, this
			// is an easy way to cheat the conversion from hex string -> number
			if (c >= '0' && c <= '9') {
				v = c - '0';
			} else if (c >= 'a' && c <= 'f') {
				v = c - 'a' + 0xA;
			} else if (c >= 'A' && c <= 'F') {
				v = c - 'A' + 0xA;
			} else {
				return 0;
			}

			address *= 0x10;
			address += v;
		}

		// Check if the address is within an actual region of memory
		// i.e 0x8000000-0x81500000 or 0x8C000000-0x8D000000
		if (
			!(address >= 0x80000000 && address <= 0x81500000) &&
			!(address >= 0x8C000000 && address <= 0x8D000000)
		) {
			return 0;
		}

		return address;
	}

	int OnEvent(struct GUIDialog_Wrapped *dialog, struct GUIDialog_OnEvent_Data *event) {
		// Annoyingly, setting the text of the address textbox is the best
		// way I have at the moment of providing some feedback to the user.
		// Not elegant, but it gets the job done.
		// TODO: Add a label to indicate the status -> requires finding a
		// SetText function for labels.
		if (event->GetEventID() == BUTTON_SET_BREAKPOINT_EVENT_ID) {
			uint32_t address = GetBreakpointAddress();

			if (address != 0) {
				SetBreakpoint(address);

				m_breakpointAddress.SetText("Set!");
			} else {
				m_breakpointAddress.SetText("Invalid.");
			}

			Refresh();
			return 0;
		}

		if (event->GetEventID() == BUTTON_REMOVE_BREAKPOINT_EVENT_ID) {
			RemoveBreakpoint();

			m_breakpointAddress.SetText("Removed.");
			Refresh();
			return 0;
		}

		return GUIDialog::OnEvent(dialog, event);
	}

private:
	GUILabel m_breakpointAddressLabel;
	GUITextBox m_breakpointAddress;

	static const int BUTTON_SET_BREAKPOINT_EVENT_ID = 1;
	GUIButton m_setBreakpoint;

	static const int BUTTON_REMOVE_BREAKPOINT_EVENT_ID = 2;
	GUIButton m_removeBreakpoint;

	static const int BUTTON_CLOSE_EVENT_ID = GUIDialog::DialogResultCancel;
	GUIButton m_close;
};

void main() {
	BreakpointDialog dialog;
	dialog.ShowDialog();
}
