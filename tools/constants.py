# Checksums are SHA256
VERSION_SPECIFIC_INFO = {
	'v02.01.2000.0000': {
		'OSupdateDLL_checksum': '22dc7e129f8840e8bab9490ee96d05c825c98e6060dd27fc66b523ce67180eda',
		'missing_byte_3069_offset': 0x2E88,
		'missing_byte_3070_offset': 0x2E51
	}
}

COMPRESSED_IMAGE_HEADER = [0x1F, 0x8B, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00]
MISSING_BYTE_OFFSET = 0x2FF6

GZIP_COMPRESSION_LEVEL = 6

def find_version_from_OSupdateDLL_checksum(checksum):
	"""Determines the firmware version with the checksum of the OSupdateDLL.dll file.

	Args:
		checksum: The SHA256 checksum of the OSupdateDLL.dll file.

	Returns:
		The version, or None if the checksum did not match any known version.
	"""

	for version, data in VERSION_SPECIFIC_INFO.items():
		if data['OSupdateDLL_checksum'].lower() == checksum.lower():
			return version
