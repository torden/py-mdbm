PYTHON?=/app/python27/bin/python

all: clean build test

build::
	@$(PYTHON) setup.py build_ext --inplace

test::
	@$(PYTHON) test.py

clean::
	@rm -rf *.out *.bin *.exe *.o *.a *.so test build
