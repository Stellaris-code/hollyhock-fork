import constants
import gzip
import hashlib

def go(args):
	with open(args.dll_path, 'rb') as f:
		dll_data = f.read()

		m = hashlib.sha256()
		m.update(dll_data)
		dll_checksum = m.hexdigest()

	version = constants.find_version_from_OSupdateDLL_checksum(dll_checksum)
	if version == None:
		print('Could not determine the firmware version associated with the specified OSupdateDLL.dll file.\nAborting.')
		return

	print(f'Found OSupdateDLL.dll firmware version: {version}\n')

	print('Reading image...', flush=True)
	with open(args.fw3070_path, 'rb') as f:
		fw3070_data = f.read()
	print('Done!\n')

	print('Packing image...', flush=True)
	fw3070c_data = gzip.compress(fw3070_data, constants.GZIP_COMPRESSION_LEVEL)

	# remove the header
	fw3070c_data = fw3070c_data[len(constants.COMPRESSED_IMAGE_HEADER):]

	# remove our missing byte
	missing_byte = fw3070c_data[constants.MISSING_BYTE_OFFSET]
	fw3070c_data = fw3070c_data[:constants.MISSING_BYTE_OFFSET] + fw3070c_data[(constants.MISSING_BYTE_OFFSET + 1):]

	print(f'    Missing byte: {missing_byte}')

	if missing_byte != dll_data[constants.VERSION_SPECIFIC_INFO[version]['missing_byte_3070_offset']]:
		print('This doesn\'t match the missing byte in your DLL. Since we aren\'t modifying the DLL, we can\'t continue.\nAborting.')
		return

	print('Packed!\n')

	print('Writing packed image...', flush=True)
	with open(args.packed_fw3070_path, 'wb') as f:
		f.write(fw3070c_data)
	print('Done!')

