#!/usr/bin/env python3

import sys
import os
import platform

import setuptools
from setuptools.command.build_ext import build_ext as _build_ext

if 'Windows' == platform.system():
    if 'AMD64' == platform.machine():
        vixpath = os.path.join(os.getenv('PROGRAMFILES(x86)'), 'VMware\\VMware VIX')
        libs = ['Vix64AllProductsDyn','kernel32','user32','advapi32','ole32','oleaut32','ws2_32','shell32']
    else:
        vixpath = os.path.join(os.getenv('PROGRAMFILES'), 'VMware\\VMware VIX')
        libs = ['VixAllProductsDyn','kernel32','user32','advapi32','ole32','oleaut32','ws2_32','shell32']

    defines = [('WIN32', None)]
    include_dirs = [vixpath]
    library_dirs = [vixpath]
elif 'Darwin' == platform.system():
    VIX_PATH = '/Applications/VMware Fusion.app/Contents/Public'
    VIX_LIB = 'vixAllProducts'
    defines = []
    include_dirs = [VIX_PATH + '/include']
    library_dirs = [VIX_PATH]
    libs = [VIX_LIB]
elif 'Linux' == platform.system():
    defines = []
    include_dirs = ['/usr/include/vmware-vix']
    library_dirs = ['/usr/lib/vmware-vix/lib']
    libs = ['vixAllProducts', 'dl']


# hack to run install_name_tool to fix path to vix library
class build_ext(_build_ext):
    def build_extension(self, ext):
        _build_ext.build_extension(self, ext)
        if 'Darwin' == platform.system():
            ext_name = self.get_ext_fullpath(ext.name)
            fmt = 'install_name_tool -change lib%s.dylib "%s/lib%s.dylib" %s'
            os.system(fmt % (VIX_LIB, VIX_PATH, VIX_LIB, ext_name))


setuptools.setup(
    cmdclass = {'build_ext': build_ext},
    name             = 'pyvix',
    author           = 'Matthew Oertle',
    author_email     = 'moertle@gmail.com',
    version          = '0.4.3',
    license          = 'MIT',
    url              = 'https://github.com/moertle/pyvix',
    description      = 'Python bindings for VIX library.',
    long_description = open('README.rst').read(),
    ext_modules = [
        setuptools.Extension(
            'pyvix',
            ['src/pyvix.c', 'src/pyvix_host.c', 'src/pyvix_vm.c'],
            define_macros   = defines,
            include_dirs    = include_dirs,
            library_dirs    = library_dirs,
            libraries       = libs,
            )
        ],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Environment :: Console',
        'Intended Audience :: Developers',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        ]
    )
