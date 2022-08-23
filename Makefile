MDBM_PATH 			?=/usr/local/mdbm/
CMD_PYTHON			?=$(shell which python)
CMD_PIP				?=$(shell which pip)
CMD_PYTEST			?=$(shell which pytest)
TODAY				:=$(shell date -u '+%Y-%m-%d-%H%M UTC')
CMD_ECHO			:=$(shell which echo)
CMD_RM				:=$(shell which rm)
CMD_LN				:=$(shell which ln)
CMD_AR				:=$(shell which ar)
CMD_RANLIB			:=$(shell which ranlib)
CMD_MV				:=$(shell which mv)
CMD_AWK				:=$(shell which awk)
CMD_SED				:=$(shell which sed)
CMD_VALGRIND		:=$(shell which valgrind)
CMD_PANDOC			:=$(shell which pandoc)

CFLAGS				?="-Wl,-rpath=/usr/local/mdbm/lib64/"
PY_VER=$(shell $(CMD_PYTHON) -c "import sys;t='{v[0]}'.format(v=list(sys.version_info[:2]));sys.stdout.write(t)")

all: clean build test

init::
	@$(CMD_ECHO)  -e "\033[1;40;32mInstall Packages.\033[01;m\x1b[0m"
ifeq ($(PY_VER),2)
	@$(CMD_PIP) install --upgrade pip
	@$(CMD_PIP) install --upgrade -r for-benchmark-py26_or_higher-requirements.txt
else
	@$(CMD_PIP) install --upgrade pip
	@$(CMD_PIP) install --upgrade build
endif
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

build::
	@$(CMD_ECHO)  -e "\033[1;40;32mBuild Source.\033[01;m\x1b[0m"
ifeq ($(PY_VER),2)
	@CFLAGS=$(CFLAGS) $(CMD_PYTHON) setup.py2 build_ext
	@$(CMD_SUDO) $(CMD_PYTHON) setup.py2 install
else
	@CFLAGS=$(CFLAGS) $(CMD_PYTHON) -m build
endif
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

dev::
	@$(CMD_ECHO)  -e "\033[1;40;32mBuild Source.\033[01;m\x1b[0m"
ifeq ($(PY_VER),2)
	@CFLAGS=$(CFLAGS) $(CMD_PYTHON) setup.py2 build_ext --inplace
else
	@CFLAGS=$(CFLAGS) $(CMD_PYTHON) -m build
endif
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

sdist::
	@$(CMD_ECHO)  -e "\033[1;40;32mDist install to pypi.\033[01;m\x1b[0m"
ifeq ($(PY_VER),2)
	@$(CMD_PYTHON) setup.py2 sdist upload -r pypi
else
	@$(CMD_PYTHON) -m twine upload --repository pypi dist/* --verbose
endif
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

test::
ifeq ($(PY_VER),2)
	@$(CMD_ECHO)  -e "\033[1;40;32mUnit-Testing.\033[01;m\x1b[0m"
	@CFLAGS=$(CFLAGS) $(CMD_PYTHON) -E tests/test.py -v
else
	@$(CMD_ECHO)  -e "\033[1;40;32mUnit-Testing.\033[01;m\x1b[0m"
	@CFLAGS=$(CFLAGS) $(CMD_PYTHON) -E tests/test.py -v
endif
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

testleak::
	@$(CMD_ECHO)  -e "\033[1;40;32mCheckt the Memory Leak.\033[01;m\x1b[0m"
ifeq ($(PY_VER),2)
	@CFLAGS=$(CFLAGS) $(CMD_VALGRIND) --tool=memcheck --suppressions=tests/.valgrind-python.supp $(CMD_PYTHON) -E -tt tests/test.py -v
else
	@$(CMD_ECHO)  -e "\033[1;40;32mUnit-Testing.\033[01;m\x1b[0m"
	@CFLAGS=$(CFLAGS) $(CMD_VALGRIND) --tool=memcheck --suppressions=tests/.valgrind-python3.supp $(CMD_PYTHON) -E -tt tests/test.py -v
endif
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"


clean::
	@$(CMD_ECHO)  -e "\033[1;40;32mRemoving Crumbs.\033[01;m\x1b[0m"
ifeq ($(PY_VER),2)
	@$(CMD_PYTHON) setup.py2 clean
else
	@$(CMD_RM) -rf dist
endif
	@rm -rf *.out *.bin *.exe *.o *.a *.so test build dist *core* *.swp *.bak .benchmarks .cache __py*__
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"

benchmark::
	@$(CMD_ECHO)  -e "\033[1;40;32mBenchmark Testing.\033[01;m\x1b[0m"
	@$(CMD_PYTEST) tests/test_benchmark.py -m store_loop_10000 
	@$(CMD_PYTEST) tests/test_benchmark.py -m store_loop_100000 
	@$(CMD_PYTEST) tests/test_benchmark.py -m store_loop_1000000 
	@$(CMD_PYTEST) tests/test_benchmark.py -m random_fetch_loop_10000 
	@$(CMD_PYTEST) tests/test_benchmark.py -m random_fetch_loop_100000 
	@$(CMD_PYTEST) tests/test_benchmark.py -m random_fetch_loop_1000000
	@$(CMD_ECHO) -e "\033[1;40;36mDone\033[01;m\x1b[0m"
readme::
	@$(CMD_PANDOC) -f markdown -t plain README.md > README.txt
#	@$(CMD_PANDOC) -f markdown -t rst -o README.rst README.md

setenv::
	@$(CMD_ECHO) -e "\033[1;40;32mSet Debug mode enviroment..\033[01;m"
	@$(CMD_SUDO) sysctl -e -q -w kernel.core_pattern="/tmp/%e.core.%u" kernel.suid_dumpable=1 fs.suid_dumpable=1
	@$(CMD_SUDO) sysctl -q -w kernel.core_uses_pid=1;
	@$(CMD_ECHO) -e "\033[1;40;36mComplete\033[01;m"

.PHONY: clean build sdit test all benchmark init readme dev setenv
