import argparse
import glob
import hashlib
import json
import os

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
		'patches_directory_path',
		help='Path to the patches directory.'
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

def apply_patch(fw3070_bytes, patch_metadata, patchset_folder_path):
	with open(os.path.join(patchset_folder_path, patch_metadata['file']), 'rb') as f:
		patch_bytes = f.read()

	patch_address_str = patch_metadata['address']
	patch_address = int(patch_address_str, 0)
	patch_length = len(patch_bytes)

	print(f'    Applied patch at {patch_address_str} with length {patch_length} bytes')

	return fw3070_bytes[:patch_address] + patch_bytes + fw3070_bytes[(patch_address + patch_length):]

def apply_patchset(fw3070_bytes, patchset_metadata, patchset_folder_path):
	patchset_name = patchset_metadata['name']

	print(f'Applying patchset \"{patchset_name}\"')

	for patch_metadata in patchset_metadata['patches']:
		fw3070_bytes = apply_patch(fw3070_bytes, patch_metadata, patchset_folder_path)

	return fw3070_bytes

def main():
	args = parse_args()

	with open(os.path.join(args.patches_directory_path, 'metadata.json'), 'r') as f:
		patches_metadata = json.loads(f.read())

	if patches_metadata['patches_format_version'] != PATCHES_FORMAT_VERSION:
		print('Patches file format version does not match patch.py version. Aborting.')
		return

	with open(args.fw3070_file_path, 'rb') as f:
		fw3070_bytes = f.read()

		m = hashlib.sha256()
		m.update(fw3070_bytes)
		fw3070_checksum = m.hexdigest()

	if patches_metadata['fw3070_checksum'] != fw3070_checksum:
		print('Checksum for fw3070 specified in patches file did not match the checksum of the fw3070 file to be patched. Aborting.')
		return

	for patchset_folder_relpath in os.listdir(args.patches_directory_path):
		patchset_folder_path = os.path.join(args.patches_directory_path, patchset_folder_relpath)
		patchset_metadata_file_path = os.path.join(patchset_folder_path, 'metadata.json')
		if os.path.isdir(patchset_folder_path) and os.path.exists(patchset_metadata_file_path):
			with open(patchset_metadata_file_path, 'r') as f:
				patchset_metadata = json.loads(f.read())
			fw3070_bytes = apply_patchset(fw3070_bytes, patchset_metadata, patchset_folder_path)

	print('Writing patched firmware...')
	with open(args.patched_fw3070_file_path, 'wb') as f:
		f.write(fw3070_bytes)
	print('Done!')

if __name__ == '__main__':
	main()
