from setuptools import setup, Extension
import os

f = open(os.path.join(os.path.dirname(__file__), 'README.md'))
long_description = f.read()
f.close()

setup_args = dict(
    ext_modules = [
        Extension("mdbm",
        sources = ["src/libmdbm.c"],
        include_dirs = ['/usr/include', '/usr/local/mdbm/include'],
        libraries = ['mdbm'],
        extra_compile_args=['-Wall','-Wno-strict-aliasing', '-march=native'],
        library_dirs = ['/usr/lib64', '/usr/local/mdbm/lib64/'],
    )],
    long_description=long_description,
    long_description_content_type='text/markdown'
)

setup(**setup_args)
