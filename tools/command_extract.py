import constants
import gzip
import hashlib
import pefile

def get_missing_bytes(dll_data, version):
	"""Returns the missing bytes from the 3069 and 3070 firmware images.

	Args:
		dll_data: The DLL file's contents.
		version: The firmware version the DLL file is associated with.

	Returns:
		A 2-tuple of the missing bytes from the 3069 and 3070 firmware images, in that order.
	"""

	return (
		dll_data[constants.VERSION_SPECIFIC_INFO[version]['missing_byte_3069_offset']],
		dll_data[constants.VERSION_SPECIFIC_INFO[version]['missing_byte_3070_offset']]
	)

def get_compressed_images(dll_path):
	"""Extracts the compressed 3069 and 3070 firmware images from the DLL.

	Args:
		dll_path: The path to the DLL.

	Returns:
		A 2-tuple of the compressed 3069 and 3070 firmware images, in that order.
	"""

	fw3069c_data = None
	fw3070c_data = None

	pe = pefile.PE(dll_path)
	pe_image = pe.get_memory_mapped_image()

	# Find the index of the RC data directory by building a list of the entry types and finding the index of the one whose type is RT_RCDATA
	rt_rcdata_index = [entry.id for entry in pe.DIRECTORY_ENTRY_RESOURCE.entries].index(pefile.RESOURCE_TYPE['RT_RCDATA'])
	rt_rcdata_directory = pe.DIRECTORY_ENTRY_RESOURCE.entries[rt_rcdata_index]

	for entry in rt_rcdata_directory.directory.entries:
		if entry.struct.Name not in [3069, 3070]:
			continue

		true_entry = entry.directory.entries[0]

		data_rva = true_entry.data.struct.OffsetToData
		size = true_entry.data.struct.Size

		data = pe_image[data_rva:(data_rva + size)]

		if entry.struct.Name == 3069:
			fw3069c_data = data
		elif entry.struct.Name == 3070:
			fw3070c_data = data

	return (fw3069c_data, fw3070c_data)

def decompress_image(compressed_data, missing_byte):
	"""Decompresses a compressed firmware image.

	Args:
		compressed_data: The compressed image, as extracted from the DLL.
		missing_byte: The missing byte to be added to the compressed image.

	Returns:
		The image, decompressed.
	"""

	# make a copy, so we don't overwrite the constant
	gzip_data = constants.COMPRESSED_IMAGE_HEADER[:]
	gzip_data.extend(compressed_data[:constants.MISSING_BYTE_OFFSET])
	gzip_data.append(missing_byte)
	gzip_data.extend(compressed_data[constants.MISSING_BYTE_OFFSET:])

	return gzip.decompress(bytes(gzip_data))

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

	print('Extracting compressed images...', flush=True)
	fw3069c_data, fw3070c_data = get_compressed_images(args.dll_path)
	print('Extracted!\n')

	fw3069_missing_byte, fw3070_missing_byte = get_missing_bytes(dll_data, version)
	
	print('Missing bytes:')
	print(f'    3069: {fw3069_missing_byte}')
	print(f'    3070: {fw3070_missing_byte}\n')

	print('Decompressing images...', flush=True)
	fw3069_data = decompress_image(fw3069c_data, fw3069_missing_byte)
	fw3070_data = decompress_image(fw3070c_data, fw3070_missing_byte)
	print('Decompressed!\n')

	print('Writing...', flush=True)
	with open(args.fw3069_path, 'wb') as f:
		f.write(fw3069_data)
	with open(args.fw3070_path, 'wb') as f:
		f.write(fw3070_data)
	print('Done!')
