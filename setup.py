#!/usr/bin/env python
# coding=utf-8
import os
from distutils.core import setup, Extension
#from mdbm import __version__


# read a desc
f = open(os.path.join(os.path.dirname(__file__), 'README.txt'))
long_description = f.read()
f.close()

setup(name = "py-mdbm",
    #version = __version__,
    version = "0.0.3",
    author="Torden",
    author_email="torden@myself.com",
    maintainer='Torden',
    maintainer_email='torden@myself.com',
    keywords=['mdbm', 'key-value store', 'pymdbm', 'py-mdbm', 'database', 'nosql', 'key-value'],
    description="The Python interface to the MDBM key-value store.",
    long_description=long_description,
    license="MIT",
    url="https://github.com/torden/py-mdbm",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "Intended Audience :: System Administrators",
        "License :: OSI Approved :: MIT License",
        "Topic :: Database",
        "Topic :: Database :: Database Engines/Servers",
        "Topic :: Utilities",
        "Programming Language :: C",
        "Programming Language :: Python :: 2.6",
        "Programming Language :: Python :: 2.7",
        "Programming Language :: Python :: 3.3",
        "Programming Language :: Python :: 3.4",
        "Programming Language :: Python :: 3.5",
        "Programming Language :: Python :: 3.6",
        "Programming Language :: Python :: Implementation :: PyPy",
        "Operating System :: POSIX :: Linux"
    ],
    ext_modules = [Extension("mdbm",
        sources = ["src/libmdbm.c"],
        include_dirs = ['/usr/include', '/usr/local/mdbm/include'],
        libraries = ['mdbm'],
        extra_compile_args=['-Wall','-Wno-strict-aliasing', '-march=native'],
        library_dirs = ['/usr/lib64', '/usr/local/mdbm/lib64/'],
    )]
)
