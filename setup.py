from setuptools import setup, Extension

setup_args = dict(
    ext_modules = [
        Extension("mdbm",
        sources = ["src/libmdbm.c"],
        include_dirs = ['/usr/include', '/usr/local/mdbm/include'],
        libraries = ['mdbm'],
        extra_compile_args=['-Wall','-Wno-strict-aliasing', '-march=native'],
        library_dirs = ['/usr/lib64', '/usr/local/mdbm/lib64/'],
    )]
)

setup(**setup_args)
