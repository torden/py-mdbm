PYTHON?=/app/python27/bin/python

all: clean build test

build::
	@$(PYTHON) setup.py build_ext --inplace -R/usr/local/mdbm/lib64/ -I/usr/local/mdbm/include/

test::
	@$(PYTHON) test.py

clean::
	@rm -rf *.out *.bin *.exe *.o *.a *.so test build
