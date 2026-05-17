# -*- coding: utf-8 -*-

import platform
from setuptools import setup, Extension
from os.path import join, basename, splitext
from glob import glob

include_dirs = ["/usr/local/include", "/opt/local/include"]
library_dirs = ["/usr/lib", "/usr/lib64", "/usr/local/lib", "/usr/local/lib64", "/opt/local/lib",
                "/usr/lib/aarch64-linux-gnu", "/usr/lib/x86_64-linux-gnu"]
libraries = ["config++"]

# Attempt to find libboost_python.so or some variant by searching through the
# library directories.
# TODO: is there some API for this?
for d in library_dirs:
    if platform.python_version() > '3.0.0':
        libs = glob(join(d, "libboost_python3.so"))
    else:
        libs = glob(join(d, "libboost_python.so"))
    if not(libs):
        libs = glob(join(d, "libboost_python*.so"))
    if libs:
        libname = basename(libs[0])         # basename
        libname = splitext(libname)[0]      # truncate postfix
        libname = libname[3:]               # truncate "lib"
        libraries.append(libname)
        break

# If we were unable to find the shared library go ahead in a default. It might
# be in an unofficial directory and an environment variable has been set that
# will point the compiler to it.
if len(libraries) <= 1:
    libraries.append('boost_python37')

setup(
    name='pylibconfig',
    description="libconfig bindings for Python",
    version="0.2.1",
    author="Sergey S. Gogin",
    author_email="d-x@bk.ru",
    maintainer="cnangel",
    maintainer_email="cnangel@gmail.com",
    keywords="libconfig libconfig++ boost python config configuration",
    license="bsd",
    url="https://github.com/cnangel/python-libconfig",
    ext_modules=[
        Extension(
            "pylibconfig",
            ["src/pylibconfig.cc"],
            include_dirs=include_dirs,
            library_dirs=library_dirs,
            libraries=libraries
        )
    ]
)
