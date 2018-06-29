#include <stdbool.h>
#include <stdint.h>
#include <sdk/cpu/ubc.h>
#include <sdk/os/debug.h>
#include <sdk/os/lcd.h>

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
extern void BreakpointHandlerStub();

/**
 * Called by @c BreakpointHandlerStub when a breakpoint is triggered.
 *
 * @param cpuState The state of the CPU when the breakpoint was triggered.
 */
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

/**
 * Prompts the user for an address to set the breakpoint at.
 * 
 * @returns An address, or 0 if the user cancelled the operation.
 */
uint32_t GetBreakpointAddress() {
	static char DIGITS[] = "0123456789ABCDEF";

	uint32_t address = 0;
	// We're calling the least significant nibble position 0
	// Therefore the most significant nibble is position 7
	int currentPosition = 7;

	int key = 0;

	do {
		LCD_ClearScreen();
		Debug_SetCursorPosition(0, 0);

		Debug_PrintString("Breakpoint at:", false);
		
		// Print the current address, highlighting the nibble that's currently
		// selected
		char *numberString = "0";
		for (int i = 0; i < 8; ++i) {
			int index = 7 - i;
			int digit = (address >> (index * 4)) & 0xF;
			numberString[0] = DIGITS[digit];

			Debug_SetCursorPosition(15 + i, 0);
			Debug_PrintString(numberString, index == currentPosition);
		}

		Debug_SetCursorPosition(0, 2);
		Debug_PrintString("8:     increase", false);
		Debug_SetCursorPosition(0, 3);
		Debug_PrintString("2:     decrease", false);
		Debug_SetCursorPosition(0, 4);
		Debug_PrintString("4:     left", false);
		Debug_SetCursorPosition(0, 5);
		Debug_PrintString("6:     right", false);

		Debug_SetCursorPosition(0, 7);
		Debug_PrintString("5:     set breakpoint", false);
		Debug_SetCursorPosition(0, 8);
		Debug_PrintString("Clear: cancel", false);

		LCD_Refresh();

		key = Debug_WaitKey();

		uint32_t mask = ~(0xF << (currentPosition * 4));
		int digit = (address >> (currentPosition * 4)) & 0xF;

		switch (key) {
		case 0x38:
				if (digit != 0xF) {
					digit++;
				}
				address = (address & mask) | (digit << (currentPosition * 4));
			break;
		case 0x32:
				if (digit != 0) {
					digit--;
				}
				address = (address & mask) | (digit << (currentPosition * 4));
			break;
		case 0x34:
			if (currentPosition != 7) {
				currentPosition++;
			}
			break;
		case 0x36:
			if (currentPosition != 0) {
				currentPosition--;
			}
			break;
		case 0x35:

			break;
		case 0x98:
			address = 0;
			break;
		}
	} while (key != 0x98 && key != 0x35);

	return address;
}

void main() {
	// Put your code here.
	LCD_ClearScreen();

	Debug_SetCursorPosition(0, 0);
	Debug_PrintString("1:     set breakpoint", false);
	Debug_SetCursorPosition(0, 1);
	Debug_PrintString("2:     remove breakpoint", false);
	Debug_SetCursorPosition(0, 2);
	Debug_PrintString("Clear: cancel", false);

	LCD_Refresh();

	int key = Debug_WaitKey();

	switch (key) {
	case 0x31: {
		uint32_t address = GetBreakpointAddress();

		LCD_ClearScreen();
		Debug_SetCursorPosition(0, 0);

		if (address != 0) {
			SetBreakpoint(address);
			Debug_PrintString("Breakpoint set!", true);
		} else {
			Debug_PrintString("Did nothing.", true);
		}

		LCD_Refresh();
		break;
	}
	case 0x32:
		LCD_ClearScreen();
		Debug_SetCursorPosition(0, 0);

		RemoveBreakpoint();
		Debug_PrintString("Breakpoint removed!", true);

		LCD_Refresh();
		break;
	default:
		LCD_ClearScreen();
		Debug_SetCursorPosition(0, 0);

		Debug_PrintString("Did nothing.", true);

		LCD_Refresh();
		break;
	}
}
