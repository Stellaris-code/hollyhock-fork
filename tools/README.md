# `tools/`
## `fw_extract.py`
Usage: `fw_extract.py zip_file_path firmware_version fw3069_file_path fw3070_file_path`

Extracts firmware images from a ZIP file firmware updater, as downloaded from the CASIO website. This tool can only be used on Windows, and requires elevation to extract the EXE file - as such, you will need to accept a UAC prompt if you have UAC enabled.

The `_msi_read_stream` C extension must also be installed. When inside the `_msi_read_stream/` directory, run `setup.py build && setup.py install` to build and install the extension. Also, go pester the developers of the Python library `msilib` to add a function to read a stream from a record. That would have saved a 2am night trying to work out how Python C extensions work.

## `fw_pack.py`
Usage: `fw_pack.py fw_bin_file_path fw_cbin_file_path`

Compresses a firmware file in the specific way required for it to be embedded into an `OSupdateDLL.dll` file. Reports the missing byte, so changes can be made to the DLL file if necessary.
