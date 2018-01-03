#!/usr/bin/env python

from distutils.core import setup, Extension

setup(name = "py-mdbm",
    version = "0.1",
    description = 'Python MDBM Wapper',
    author = 'Torden',
    url = 'https://github.com/torden/py-mdbm',
    ext_modules = [Extension("mdbm",
        sources = ["libmdbm.c"],
        include_dirs = ['/usr/include', '/usr/local/mdbm/include'],
        libraries = ['mdbm'],
        library_dirs = ['/usr/lib64', '/usr/local/mdbm/lib64/'],
    )]
)
