from distutils.core import setup, Extension

module1 = Extension('_msi_read_stream', sources=['_msi_read_stream.c'], libraries=['msi'])

setup(
	name = 'MSIReadStream',
	version = '1.0',
	description = 'This is a test package',
	ext_modules = [module1]
)
