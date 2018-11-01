from distutils.core import setup, Extension, DEBUG
import numpy
from sysconfig import get_paths

sfc_module = Extension(
	'cpython_module',
	sources = ['example.cpp'],
	include_dirs=[get_paths()['include'], numpy.get_include()],
	language='c++')

setup(name = 'cpython_module', version = '1.01',
    description = 'Python C++ extension',
    ext_modules = [sfc_module]
    )
