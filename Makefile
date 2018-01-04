DATE				:= $(shell date -u '+%Y-%m-%d-%H%M UTC')
CMD_RM				:=$(shell which rm)
CMD_LN          	:=$(shell which ln)
CMD_AR          	:=$(shell which ar)
CMD_RANLIB      	:=$(shell which ranlib)
CMD_MV          	:=$(shell which mv)
CMD_AWK				:=$(shell which awk)
CMD_SED				:=$(shell which sed)
CMD_VALGRIND		:=$(shell which valgrind)

PYTHON?=/app/python27/bin/python

all: clean build test

build::
	@$(PYTHON) setup.py build_ext --inplace -R/usr/local/mdbm/lib64/ -I/usr/local/mdbm/include/
test::
#	@$(PYTHON) test.py -v
	@$(CMD_VALGRIND) --tool=memcheck --dsymutil=yes --track-origins=yes --show-leak-kinds=all --trace-children=yes --suppressions=.valgrind-python.supp $(PYTHON) -E -tt test.py -v

clean::
	@rm -rf *.out *.bin *.exe *.o *.a *.so test build *core* *.swp *.bak

.PHONY: clean build test all
