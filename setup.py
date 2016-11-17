#!/usr/bin/env python3

import sys
import os
import platform

import setuptools

if 'Windows' == platform.system():
    if 'AMD64' == platform.machine():
        vixpath = os.path.join(os.getenv('PROGRAMFILES(x86)'), 'VMware\\VMware VIX')
        libs = ['Vix64AllProducts','kernel32','user32','advapi32','ole32','oleaut32','ws2_32','shell32']
    else:
        vixpath = os.path.join(os.getenv('PROGRAMFILES'), 'VMware\\VMware VIX')
        libs = ['VixAllProducts','kernel32','user32','advapi32','ole32','oleaut32','ws2_32','shell32']

    defines = [('WIN32', None)]
    include_dirs = [vixpath]
    library_dirs = [vixpath]
elif 'Darwin' == platform.system():
    defines = []
    include_dirs = ['/Applications/VMware Fusion.app/Contents/Public/include']
    library_dirs = ['/Applications/VMware Fusion.app/Contents/Public']
    libs = ['vixAllProducts', 'dl']
else:
    defines = []
    include_dirs = ['/usr/include/vmware-vix']
    library_dirs = ['/usr/lib/vmware-vix/lib']
    libs = ['vixAllProducts', 'dl']

setuptools.setup(
    name             = 'pyvix',
    author           = 'Matthew Oertle',
    author_email     = 'moertle@gmail.com',
    version          = '0.3',
    license          = 'MIT',
    url              = 'http://oertle.org/pyvix',
    description      = 'Python bindings for VIX library.',
    long_description = open('README.rst').read(),
    ext_modules = [
        setuptools.Extension(
            'pyvix',
            ['src/pyvix.c', 'src/pyvix_host.c', 'src/pyvix_vm.c'],
            define_macros = defines,
            include_dirs  = include_dirs,
            library_dirs  = library_dirs,
            libraries     = libs
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
