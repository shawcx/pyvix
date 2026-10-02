#!/usr/bin/env python3

# Project metadata lives in pyproject.toml; this file only describes the
# extension, whose VIX paths and libraries differ per platform.

import os
import platform
import subprocess

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


# the Fusion dylib has a bare install name, so point the extension at its
# absolute path inside the app bundle
class build_ext(_build_ext):
    def build_extension(self, ext):
        super().build_extension(ext)
        if 'Darwin' == platform.system():
            subprocess.run([
                'install_name_tool', '-change',
                'lib%s.dylib' % VIX_LIB,
                '%s/lib%s.dylib' % (VIX_PATH, VIX_LIB),
                self.get_ext_fullpath(ext.name),
                ], check=True)


setuptools.setup(
    cmdclass = {'build_ext': build_ext},
    ext_modules = [
        setuptools.Extension(
            'pyvix',
            ['src/pyvix.c', 'src/pyvix_host.c', 'src/pyvix_vm.c'],
            depends         = ['src/pyvix.h'],
            define_macros   = defines,
            include_dirs    = include_dirs,
            library_dirs    = library_dirs,
            libraries       = libs,
            )
        ],
    )
