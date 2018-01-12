#!/usr/bin/env python
# coding=utf-8
from distutils.core import setup, Extension

setup(name = "py-mdbm",
    version = "0.0.2",
    author="torden",
    author_email="torden@myself.com",
    description="Just! Python interface for Y! MDBM, Python MDBM Wapper",
    license="MIT",
    keywords="database nosql key-value",
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
