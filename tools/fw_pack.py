import argparse
import gzip

GZIP_COMPRESSION_LEVEL = 6

def parse_args():
	"""
	@brief Parses the arguments passed into the firmware packing utility.

	@return A dictionary, representing the arguments passed in.
	"""

	argument_parser = argparse.ArgumentParser(
		description=(
			'Tool to pack a firmware image, ready to be inserted into an OSupdateDLL.dll file.'
		)
	)

	argument_parser.add_argument(
		'fw_bin_file_path',
		help='Path to the uncompressed firmware file.'
	)

	argument_parser.add_argument(
		'fw_cbin_file_path',
		default='fw.cbin',
		nargs='?',
		help='Path to save the compressed firmware file to (default: fw.cbin).'
	)

	return argument_parser.parse_args()

def main():
	args = parse_args()

	with open(args.fw_bin_file_path, 'rb') as fw_bin_file:
		# skip in by 0xA to skip the header
		compressed_data = gzip.compress(fw_bin_file.read(), GZIP_COMPRESSION_LEVEL)[0xA:]

		with open(args.fw_cbin_file_path, 'wb') as fw_cbin_file:
			fw_cbin_file.write(compressed_data[:0x2FF6])
			fw_cbin_file.write(compressed_data[0x2FF7:])

			print(f'Missing byte: {compressed_data[0x2FF6]}\nIf this missing byte does not match the missing byte in the OSupdateDLL.dll file you\'re using, you must modify the DLL before flashing.')

if __name__ == '__main__':
	main()
