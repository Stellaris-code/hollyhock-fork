import hashlib
import json
import os

PATCHES_FORMAT_VERSION = '1'

def apply_patch(fw3070_data, patch_address, patch_data):
	"""Applies a patch to a firmware image at the specified address.

	Args:
		fw3070_data: The data of the firmware image.
		patch_address: The offset into the file to apply the patch to.
		patch_data: The data of the patch.

	Returns:
		The modified firmware image data.
	"""

	return fw3070_data[:patch_address] + patch_data + fw3070_data[(patch_address + len(patch_data)):]

def apply_patchset(fw3070_data, patchset_dir):
	"""Applies a patchset to a firmware image.
		
	Args:
		fw3070_data: The data of the firmware image.
		patchset_dir: The path to the directory containing the patchset.

	Returns:
		The modified firmware image data.
	"""

	patchset_metadata_path = os.path.join(
		patchset_dir, 'metadata.json'
	)

	with open(patchset_metadata_path, 'r') as f:
		patchset_metadata = json.loads(f.read())

	patchset_name = patchset_metadata['name']

	print(f'Applying patchset "{patchset_name}"')

	for patch in patchset_metadata['patches']:
		patch_file_path = os.path.join(
			patchset_dir, patch['file']
		)

		with open(patch_file_path, 'rb') as f:
			patch_data = f.read()

		patch_length = len(patch_data)
		patch_address = int(patch['address'], 0)

		print(f'    Applying {patch_length} byte patch at {hex(patch_address)}')
		fw3070_data = apply_patch(fw3070_data, patch_address, patch_data)

	return fw3070_data

def go(args):
	with open(args.fw3070_path, 'rb') as f:
		fw3070_data = f.read()

		m = hashlib.sha256()
		m.update(fw3070_data)
		fw3070_checksum = m.hexdigest()

	patches_metadata_path = os.path.join(
		args.patches_dir, 'metadata.json'
	)

	with open(patches_metadata_path, 'r') as f:
		patches_metadata = json.loads(f.read())

	if patches_metadata['patches_format_version'] != PATCHES_FORMAT_VERSION:
		print('The version of the patches format does not match the version required by this command.\nAborting.')
		return

	if patches_metadata['fw3070_checksum'].lower() != fw3070_checksum.lower():
		print('The checksum of the 3070 firmware image does not match the checksum specified in the patches metadata.\nAborting.')
		return

	for patchset_name in os.listdir(args.patches_dir):
		patchset_dir = os.path.join(
			args.patches_dir, patchset_name
		)

		if os.path.isdir(patchset_dir):
			fw3070_data = apply_patchset(fw3070_data, patchset_dir)

	print('Writing patched firmware image...', flush=True)
	with open(args.patched_fw3070_path, 'wb') as f:
		f.write(fw3070_data)
	print('Done!')
