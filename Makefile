DATE				:=$(shell date -u '+%Y-%m-%d-%H%M UTC')
CMD_RM				:=$(shell which rm)
CMD_LN          	:=$(shell which ln)
CMD_AR          	:=$(shell which ar)
CMD_RANLIB      	:=$(shell which ranlib)
CMD_MV          	:=$(shell which mv)
CMD_AWK				:=$(shell which awk)
CMD_SED				:=$(shell which sed)
CMD_VALGRIND		:=$(shell which valgrind)

PYTHON?=`which python`

PY_VER=$(shell $(PYTHON) -c "import sys;t='{v[0]}'.format(v=list(sys.version_info[:2]));sys.stdout.write(t)")

all: clean build test

build::
	@$(PYTHON) -V
	@$(PYTHON) setup.py build_ext --inplace -L/usr/local/mdbm/lib64/ -I/usr/local/mdbm/include/
test::
ifeq ($(GOLANGV16_OVER),2)
	@$(CMD_VALGRIND) --tool=memcheck --dsymutil=yes --track-origins=yes --show-leak-kinds=all --trace-children=yes --suppressions=.valgrind-python.supp $(PYTHON) -E -tt test.py -v
else
#	@$(CMD_VALGRIND) --tool=memcheck --suppressions=.valgrind-python3.supp $(PYTHON) -E test.py -v
	@$(PYTHON) -E test.py -v
endif

clean::
	@rm -rf *.out *.bin *.exe *.o *.a *.so test build *core* *.swp *.bak

.PHONY: clean build test all
