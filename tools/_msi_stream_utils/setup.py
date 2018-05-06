from distutils.core import setup, Extension

module1 = Extension('_msi_stream_utils', sources=['_msi_stream_utils.c'], libraries=['msi'])

setup(
	name = 'MSIStreamUtils',
	version = '1.0',
	description = 'This is a test package',
	ext_modules = [module1]
)
