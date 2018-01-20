# `file_execute/`
Tool used to execute code from a file stored on the calculator's flash. Copies the contents of `\fls0\run.bin` into a unused section of RAM, then jumps to it.

## Compiling
Run `make` to compile the tool. The resulting `file_execute.bin` can be placed into the firmware and flashed onto the device.
