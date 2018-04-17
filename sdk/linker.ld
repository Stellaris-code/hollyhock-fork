OUTPUT_FORMAT("binary");

INCLUDE function_addresses.ld;

SECTIONS {
	start_addr = 0x8CFE0000;

	. = start_addr;

	.init start_addr : AT(start_addr) {
		*(.init)
	}
}
