import argparse
import command_extract
import command_pack
import command_patch

def parse_args():
	parser = argparse.ArgumentParser(
		description='hollyhock: tools and software for the fx-CP400'
	)
	subparsers = parser.add_subparsers(title='command')
	subparsers.required = True
	subparsers.dest = 'command'

	parser_extract = subparsers.add_parser(
		'extract',
		description='Extract firmware images from an OSupdateDLL.dll file.'
	)
	parser_extract.set_defaults(func=command_extract.go)
	parser_extract.add_argument(
		'dll_path',
		help='Path to the OSupdateDLL.dll file to extract from.'
	)
	parser_extract.add_argument(
		'fw3069_path',
		help='Path to save the extracted 3069 image to.'
	)
	parser_extract.add_argument(
		'fw3070_path',
		help='Path to save the extracted 3070 image to.'
	)

	parser_patch = subparsers.add_parser(
		'patch',
		description='Patch a firmware image.'
	)
	parser_patch.set_defaults(func=command_patch.go)
	parser_patch.add_argument(
		'patches_dir',
		help='Path to the directory containing the patch information.'
	)
	parser_patch.add_argument(
		'fw3070_path',
		help='Path to the 3070 firmware image to patch.'
	)
	parser_patch.add_argument(
		'patched_fw3070_path',
		help='Path to save the patched 3070 firmware image to.'
	)

	parser_pack = subparsers.add_parser(
		'pack',
		description='Pack a modified firmware image ready to be embedded in an OSupdateDLL.dll file.'
	)
	parser_pack.set_defaults(func=command_pack.go)
	parser_pack.add_argument(
		'fw3070_path',
		help='Path to the 3070 image to pack into the format required by the DLL.'
	)
	parser_pack.add_argument(
		'packed_fw3070_path',
		help='Path to save the packed 3070 image to.'
	)
	parser_pack.add_argument(
		'dll_path',
		help='Path to the DLL which the packed image will be embedded in (will not be modified).'
	)

	return parser.parse_args()

def main():
	args = parse_args()
	args.func(args)

if __name__ == '__main__':
	main()
