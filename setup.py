# -*- coding: utf-8 -*-

from setuptools import setup, Extension
from os.path import join, basename, splitext
from glob import glob

include_dirs = ["/usr/local/include", "/opt/local/include"]
library_dirs = ["/usr/lib", "/usr/lib64", "/usr/local/lib", "/usr/local/lib64", "/opt/local/lib"]
libraries = ["config++"]

# lookup library TODO: is there some API for this?
#for d in library_dirs:
#    libs = glob(join(d, "libboost_python.so"))
#    if not(libs):
#        libs = glob(join(d, "libboost_python*.so"))
#    if libs:
#        libname = basename(libs[0])         # basename
#        libname = splitext(libname)[0]      # truncate postfix
#        libname = libname[3:]               # truncate "lib"
#        libraries.append(libname)
#        break

# check that we really found boost
#assert(len(libraries) > 1)

setup(
    name='pylibconfig',
    description="libconfig bindings for Python",
    version="0.0.2",
    author="Sergey S. Gogin",
    author_email="d-x@bk.ru",
    maintainer="cnangel",
    maintainer_email="cnangel@gmail.com",
    keywords="libconfig libconfig++ boost python config configuration",
    test_suite="tests",
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
