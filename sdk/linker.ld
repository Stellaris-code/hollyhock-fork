OUTPUT_FORMAT("binary");

SECTIONS {
	start_addr = 0x8CFE0000;

	. = start_addr;

	.init start_addr : AT(start_addr) {
		*(.init)
	}
}
