import argparse
import glob
import gzip
import io
import logging
import msilib
import os
import pefile
import platform
import shutil
import tempfile
import zipfile
import _msi_read_stream

logging.basicConfig(format='|%(levelname)s| %(message)s', level=logging.DEBUG)

HEADER = [0x1F, 0x8B, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00]
MISSING_BYTES = {
	'v02.00.4000.0000': {
		'fw3069': 11,
		'fw3070': 155
	},
	'v02.01.0000.0000': {
		'fw3069': 11,
		'fw3070': 0
	},
	'v02.01.1000.0000': {
		'fw3069': 11,
		'fw3070': 0
	},
	'v02.01.2000.0000': {
		'fw3069': 11,
		'fw3070': 155
	}
}

def parse_args():
	"""
	@brief Parses the arguments passed into the firmware extraction utility.

	@return A dictionary, representing the arguments passed in.
	"""

	argument_parser = argparse.ArgumentParser(
		description=(
			'Tool to extract firmware images from a CASIO fx-CP400 firmware update ZIP file.'
		)
	)

	argument_parser.add_argument(
		'zip_file_path',
		help='Path to the ZIP file.'
	)

	argument_parser.add_argument(
		'firmware_version',
		choices=MISSING_BYTES.keys(),
		help='Version of the firmware ZIP. Required to fill in missing bytes.'
	)

	argument_parser.add_argument(
		'fw3069_file_path',
		default='fw3069.bin',
		nargs='?',
		help='Path to save fw3069.bin to (default: fw3069.bin).'
	)

	argument_parser.add_argument(
		'fw3070_file_path',
		default='fw3070.bin',
		nargs='?',
		help='Path to save fw3070.bin to (default: fw3070.bin).'
	)

	return argument_parser.parse_args()

def extract_zip(file_path, extract_directory_path):
	"""
	@brief Extracts a firmware updater ZIP file downloaded from the CASIO website.

	@param file_path The path to the ZIP file to extract.
	@param extract_directory_path The path to the directory into which the ZIP will be extracted.

	@return True if the extraction was successful, False otherwise.
	"""

	try:
		zip = zipfile.ZipFile(file_path, 'r')
		zip.extractall(extract_directory_path)
		zip.close()
	except e:
		logging.error('The ZIP file could not be extracted due to the following exception:')
		logging.error(e)

	return True

def extract_exe(file_path, extract_directory_path):
	"""
	@brief Extracts the installer EXE.

	@param file_path The path to the installer EXE.
	@param extract_directory_path The path to the directory into which the EXE will be extracted.

	@return True if the extraction was successful, False otherwise.
	"""

	# subprocess doesn't work here for some reason, so I'm resorting to os.system
	arguments = [file_path, '/s', '/s', f'/b"{extract_directory_path}"', '/v"/qn"', '/w']
	if os.system(' '.join(arguments)) != 0:
		logging.error('The EXE did not return 0 when extraction was attempted.')
		return False

	return True

def extract_msi(file_path, extract_directory_path):
	"""
	@brief Extracts the installer MSI.

	@param file_path The path to the installer MSI.
	@param extract_directory_path The path to the directory into which the MSI will be extracted.

	@return True if the extraction was successful, False otherwise.
	"""

	msi_database = msilib.OpenDatabase(file_path, msilib.MSIDBOPEN_READONLY)
	view = msi_database.OpenView('SELECT * FROM `ISSetupFile`')
	view.Execute(None)

	while True:
		record = None
		try:
			record = view.Fetch()
		except msilib.MSIError:
			# This is the only way to check if we've reached the end of the view
			pass

		# record will be None when we've reached the end of the view, so we'll exit the while loop
		if record is None:
			break

		file_name = record.GetString(2)
		data = _msi_read_stream.ReadStream(record, 3)
		with open(os.path.join(extract_directory_path, file_name), 'wb') as f:
			f.write(data)

	view.Close()

	return True

def extract_dll(file_path, extract_directory_path):
	"""
	@brief Extracts the compressed fw3069 and fw3070 firmware images from the OSupdateDLL.dll file. No header or missing byte is added.

	@param file_path The path to the OSupdateDLL.dll file.
	@param extract_directory_path The path to the directory into which the two compressed firmware images will be extracted.

	@return True if the extraction was successful, False otherwise.
	"""

	files = {}

	pe = pefile.PE(file_path)
	rt_rcdata_index = [entry.id for entry in pe.DIRECTORY_ENTRY_RESOURCE.entries].index(pefile.RESOURCE_TYPE['RT_RCDATA'])
	rt_rcdata_directory = pe.DIRECTORY_ENTRY_RESOURCE.entries[rt_rcdata_index]
	for entry in rt_rcdata_directory.directory.entries:
		true_entry = entry.directory.entries[0]

		data_rva = true_entry.data.struct.OffsetToData
		size = true_entry.data.struct.Size

		data = pe.get_memory_mapped_image()[data_rva:data_rva + size]
		files[entry.struct.Name] = data[:]

	for resource_name, data in files.items():
		with open(os.path.join(extract_directory_path, f'fw{str(resource_name)}.cbin'), 'wb') as f:
			f.write(data) 

	return True

def extract_cbin(
	fw3069_cbin_file_path, fw3070_cbin_file_path,
	fw3069_bin_file_path, fw3070_bin_file_path,
	firmware_version
):
	"""
	@brief Extracts the firmware images from their compressed counterparts.

	@param fw3069_cbin_file_path Path to the compressed fw3069 file.
	@param fw3070_cbin_file_path Path to the compressed fw3070 file.
	@param fw3069_bin_file_path Path to save the decompressed fw3069 file to.
	@param fw3070_bin_file_path Path to save the decompressed fw3070 file to.
	@param firmware_version The firmware version being extracted, used to lookup the missing bytes.
	"""

	def extract_cbin_internal(cbin_file_path, bin_file_path, missing_byte):
		# Copy the header, don't just make gzip_data a reference to HEADER
		gzip_data = HEADER[:]
		with open(cbin_file_path, 'rb') as cbin_file:
			cbin_data = cbin_file.read()
			gzip_data.extend(cbin_data[0:0x2FF6])
			gzip_data.append(missing_byte)
			gzip_data.extend(cbin_data[0x2FF6:])

		with open(bin_file_path, 'wb') as bin_file:
			bin_file.write(gzip.decompress(bytes(gzip_data)))

	extract_cbin_internal(fw3069_cbin_file_path, fw3069_bin_file_path, MISSING_BYTES[firmware_version]['fw3069'])
	extract_cbin_internal(fw3070_cbin_file_path, fw3070_bin_file_path, MISSING_BYTES[firmware_version]['fw3070'])

	return True

def main():
	if platform.system() != 'Windows':
		logging.error('This tool only works on Windows.')
		return

	args = parse_args()

	zip_extract_directory_path = tempfile.mkdtemp()
	logging.info(f'Extracting ZIP file to {zip_extract_directory_path}')

	if not extract_zip(args.zip_file_path, zip_extract_directory_path):
		logging.error('ZIP extraction failed.')
		return

	# There should only be one EXE inside the ZIP. Find its path.
	exe_file_paths = glob.glob(os.path.join(zip_extract_directory_path, '*.exe'))

	if len(exe_file_paths) != 1:
		logging.error('Zero or more than one EXE file found inside ZIP file.')
		return

	exe_file_path = exe_file_paths[0]
	exe_extract_directory_path = tempfile.mkdtemp()
	logging.info(f'Extracting EXE file to {exe_extract_directory_path}')
	logging.info('This may take >30 seconds, and you may need to accept a UAC prompt.')

	if not extract_exe(exe_file_path, exe_extract_directory_path):
		logging.error('EXE extraction failed.')
		return

	# There should only be one MSI file inside the EXE. Find its path.
	msi_file_paths = glob.glob(os.path.join(exe_extract_directory_path, '*.msi'))

	if len(msi_file_paths) != 1:
		logging.error('Zero or more than one MSI file found inside EXE file.')
		return

	msi_file_path = msi_file_paths[0]
	msi_extract_directory_path = tempfile.mkdtemp()
	logging.info(f'Extracting MSI file to {msi_extract_directory_path}')

	if not extract_msi(msi_file_path, msi_extract_directory_path):
		logging.error('MSI extraction failed.')
		return

	dll_file_path = os.path.join(msi_extract_directory_path, 'OSupdateDLL.dll')
	dll_extract_directory_path = tempfile.mkdtemp()
	logging.info(f'Extracting DLL file to {dll_extract_directory_path}')

	if not extract_dll(dll_file_path, dll_extract_directory_path):
		logging.error('DLL extraction failed.')
		return

	fw3069_cbin_file_path = os.path.join(dll_extract_directory_path, 'fw3069.cbin')
	fw3070_cbin_file_path = os.path.join(dll_extract_directory_path, 'fw3070.cbin')
	logging.info('Extracting firmware images')

	if not extract_cbin(
		fw3069_cbin_file_path, fw3070_cbin_file_path,
		args.fw3069_file_path, args.fw3070_file_path,
		args.firmware_version
	):
		logging.error('Firmware image extraction failed.')
		return

	logging.info('Firmware files extracted successfully!')

if __name__ == '__main__':
	main()
