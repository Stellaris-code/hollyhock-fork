#include <sdk/os/gui.hpp>

extern void main();

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
