MDBM_PATH 			?=/usr/local/mdbm/
CMD_PYTHON			?=$(shell which python)
CMD_PIP				?=$(shell which pip)
CMD_PYTEST			?=$(shell which pytest)
DATE				:=$(shell date -u '+%Y-%m-%d-%H%M UTC')
CMD_ECHO			:=$(shell which echo)
CMD_RM				:=$(shell which rm)
CMD_LN          	:=$(shell which ln)
CMD_AR          	:=$(shell which ar)
CMD_RANLIB      	:=$(shell which ranlib)
CMD_MV          	:=$(shell which mv)
CMD_AWK				:=$(shell which awk)
CMD_SED				:=$(shell which sed)
CMD_VALGRIND		:=$(shell which valgrind)


PY_VER=$(shell $(CMD_PYTHON) -c "import sys;t='{v[0]}'.format(v=list(sys.version_info[:2]));sys.stdout.write(t)")

all: clean build test

init::
	@$(CMD_ECHO)  -e "\033[1;40;32mInstall Packages.\033[01;m\x1b[0m"
	@$(CMD_PIP) install --upgrade -r requirements.txt
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

build::
	@$(CMD_ECHO)  -e "\033[1;40;32mBuild Source.\033[01;m\x1b[0m"
	@$(CMD_PYTHON) setup.py build_ext --inplace 
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

test::
ifeq ($(GOLANGV16_OVER),2)
	@$(CMD_ECHO)  -e "\033[1;40;32mTesting Memory Leak and Unit-test.\033[01;m\x1b[0m"
	@$(CMD_VALGRIND) --tool=memcheck --dsymutil=yes --track-origins=yes --show-leak-kinds=all --trace-children=yes --suppressions=.valgrind-python.supp $(CMD_PYTHON) -E -tt test.py -v
else
	@$(CMD_ECHO)  -e "\033[1;40;32mUnit-Testing.\033[01;m\x1b[0m"
#	@$(CMD_VALGRIND) --tool=memcheck --suppressions=.valgrind-python3.supp $(PYTHON) -E test.py -v
	@$(CMD_PYTHON) -E test.py -v
endif
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

clean::
	@$(CMD_ECHO)  -e "\033[1;40;32mRemoving Crumbs.\033[01;m\x1b[0m"
	@rm -rf *.out *.bin *.exe *.o *.a *.so test build *core* *.swp *.bak
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

benchmark::
	@$(CMD_ECHO)  -e "\033[1;40;32mBenchmark Testing.\033[01;m\x1b[0m"
	@$(CMD_PYTEST) test_benchmark.py -m loop_10000
	@$(CMD_PYTEST) test_benchmark.py -m loop_100000
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"


.PHONY: clean build test all benchmark init
