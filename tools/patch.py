import argparse
import hashlib
import logging
import json

logging.basicConfig(format='|%(levelname)s| %(message)s', level=logging.DEBUG)

PATCHES_FORMAT_VERSION = '1'

def parse_args():
	"""
	@brief Parses the arguments passed into the firmware patching utility.

	@return A dictionary, representing the arguments passed in.
	"""

	argument_parser = argparse.ArgumentParser(
		description=(
			'Tool to apply patches to an fx-CP400 firmware image.'
		)
	)

	argument_parser.add_argument(
		'patches_file_path',
		help='Path to a patches.json file.'
	)

	argument_parser.add_argument(
		'fw3070_file_path',
		help='Path to the uncompressed fw3070 file to patch.'
	)

	argument_parser.add_argument(
		'patched_fw3070_file_path',
		help='Path to save the uncompressed and patched fw3070 file to.'
	)

	return argument_parser.parse_args()

def apply_patch(fw3070_bytes, patch_metadata):
	with open(patch_metadata['file'], 'rb') as f:
		patch_bytes = f.read()

	patch_address_str = patch_metadata['address']
	patch_address = int(patch_address_str, 0)
	patch_length = len(patch_bytes)

	logging.info(f'Applied patch at {patch_address_str} with length {patch_length} bytes')

	return fw3070_bytes[:patch_address] + patch_bytes + fw3070_bytes[(patch_address + patch_length):]

def main():
	args = parse_args()

	with open(args.patches_file_path, 'r') as f:
		patches_metadata = json.loads(f.read())

	if patches_metadata['patches_format_version'] != PATCHES_FORMAT_VERSION:
		logging.error('Patches file format version does not match patch.py version. Aborting.')
		return

	with open(args.fw3070_file_path, 'rb') as f:
		fw3070_bytes = f.read()

		m = hashlib.sha256()
		m.update(fw3070_bytes)
		fw3070_checksum = m.hexdigest()

	if patches_metadata['fw3070_checksum'] != fw3070_checksum:
		logging.error('Checksum for fw3070 specified in patches file did not match the checksum of the fw3070 file to be patched. Aborting.')
		return

	for patch_metadata in patches_metadata['patches']:
		fw3070_bytes = apply_patch(fw3070_bytes, patch_metadata)

	with open(args.patched_fw3070_file_path, 'wb') as f:
		f.write(fw3070_bytes)

if __name__ == '__main__':
	main()
