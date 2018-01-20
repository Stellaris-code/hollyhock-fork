# DISCLAIMER
- I AM NOT RESPONSIBLE FOR ANY DAMAGE YOU MAY DO TO YOUR fx-CP400.  
- I AM NOT LIABLE IF YOU BRICK OR OTHERWISE DAMAGE YOUR CALCULATOR.  
- CASIO WILL NOT FIX YOUR CALCULATOR IF IT SHOWS EVIDENCE OF TAMPERING, I.E. FLASHING WITH MODIFIED FIRMWARE IMAGES OR MESSING WITH DEBUG MENUS.  
- I WILL NOT FIX YOUR CALCULATOR.  
- IF YOU CANNOT FIX YOUR CALCULATOR YOURSELF, DO NOT USE THIS TOOLKIT.  
- **BY USING THESE TOOLS, YOU AGREE TO TAKE FULL RESPONSIBILITY FOR ANY DAMAGE THAT MAY OCCUR.**  
- **YOU HAVE BEEN WARNED**

# fx-CP400
Tools and software for the CASIO fx-CP400 graphics calculator.

## `src/`
Code written to be run on the fx-CP400.

### `file_execute/`
Tool used to execute code from a file stored on the calculator's flash.

## `tools/`
Tools for manipulating fx-CP400 firmware images, and useful IDAPython scripts.

### `fw_extract.py`
Extracts firmware images from a ZIP file firmware updater, as downloaded from the CASIO website.

### `fw_pack.py`
Compresses a firmware file in the specific way required for it to be embedded into an `OSupdateDLL.dll` file.
