PY=/app/python27/bin/python

all: clean build test

build::
	@$(PY) setup.py build_ext --inplace

test::
	@$(PY) test.py

clean::
	@rm -rf *.out *.bin *.exe *.o *.a *.so test build
