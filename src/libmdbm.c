#include <stdio.h>
#include <Python.h>
#include "libmdbm.h"
#include <mdbm.h>
#include <mdbm_log.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>


#define MDBM_LOG_OFF            -1
#define MDBM_LOG_EMERGENCY      LOG_EMERG
#define MDBM_LOG_ALERT          LOG_ALERT
#define MDBM_LOG_CRITICAL       LOG_CRIT
#define MDBM_LOG_ERROR          LOG_ERR
#define MDBM_LOG_WARNING        LOG_WARNING
#define MDBM_LOG_NOTICE         LOG_NOTICE
#define MDBM_LOG_INFO           LOG_INFO
#define MDBM_LOG_DEBUG          LOG_DEBUG
#define MDBM_LOG_DEBUG2         LOG_DEBUG+1
#define MDBM_LOG_DEBUG3         LOG_DEBUG+2
#define MDBM_LOG_MAXLEVEL       LOG_DEBUG+3
#define MDBM_LOG_ABORT          LOG_EMERG
#define MDBM_LOG_FATAL          LOG_ALERT


static int loglevel = -1;
static int capture = 0;
static int dev_null;
static int org_stderr;

#define CAPTURE_START() {\
    if (loglevel == -1) {\
		fflush(stderr);\
    	dev_null = open("/dev/null", O_RDWR);\
    	org_stderr = dup(fileno(stderr));\
        dup2(dev_null, fileno(stderr));\
		capture = 1;\
    }\
}

#define CAPTURE_END() {\
    if (capture == 1) {\
		fflush(stderr);\
        dup2(org_stderr, fileno(stderr));\
		close(dev_null);\
		capture = 0;\
    }\
}

// New in version 2.6.
#if !defined(PyModule_AddIntMacro)
#define PyModule_AddIntMacro(module, name){ PyModule_AddIntConstant(module, #name, name); }
#endif

// New in version 2.6.
#if !defined(PyModule_AddStringMacro)
#define PyModule_AddStringMacro(module, name) { PyModule_AddStringConstant(module, #name, name); }
#endif

#if PY_MAJOR_VERSION >= 3
    #define _PYUNICODE PyUnicode_FromFormat
    #define _PYUNICODEV PyUnicode_FromFormt
    #define _PYUNICODE_ANDSIZE PyUnicode_FromStringAndSize
#else
    #define _PYUNICODE  PyString_FromString
    #define _PYUNICODEV  PyString_FromFormatV
    #define _PYUNICODE_ANDSIZE PyString_FromStringAndSize
#endif

#define _RETURN_FUNC(_arg) {\
    Py_INCREF(_arg); \
    return _arg;\
}

#define _RETURN_FALSE() { _RETURN_FUNC(Py_False); }
#define _RETURN_TRUE() { _RETURN_FUNC(Py_True); }
#define _RETURN_NONE() { _RETURN_FUNC(Py_None); }
#define _RETURN_RV_BOOLEN(rv) {\
    if ((int)rv == -1) {\
         _RETURN_FUNC(Py_False);\
    }\
    _RETURN_FUNC(Py_True);\
}

#if PY_MAJOR_VERSION >= 3
    struct module_state {
        PyObject *error;
    };
    #define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))
#endif

PyMethodDef mdbm_methods[] = {
	{"log_minlevel", (PyCFunction)pymdbm_log_minlevel, METH_VARARGS, 
		"log_minlevel(MDBM_LOG_XXX)"
			"Set the minimum logging level. Lower priority messages are discarded"
			"Default : MDBM_LOG_OFF"
	},

	{"open", (PyCFunction)pymdbm_open, METH_VARARGS | METH_KEYWORDS, 
		"open(path, flags, mode, [psize, presize])"
			"Creates and/or opens a database."
	},
	{"close", (PyCFunction)pymdbm_close, METH_NOARGS, 
		"close()"
			"Creates and/or opens a database."
	},
	{"close_fd", (PyCFunction)pymdbm_close_fd, METH_NOARGS, 
		"close_fd()"
			"Closes an MDBM's underlying file descriptor."
	},
	{"sync", (PyCFunction)pymdbm_sync, METH_NOARGS, 
		"sync()"
			"msync's all pages to disk asynchronously."
	},
	{"fsync", (PyCFunction)pymdbm_fsync, METH_NOARGS, 
		"fsync()"
			"fsync's an MDBM.  Syncs all pages to disk synchronously."
	},
	{"preload", (PyCFunction)pymdbm_preload, METH_NOARGS, 
		"preload()"
			"Preload mdbm: Read every 4k bytes to force all pages into memory"
	},
	{"compress_tree", (PyCFunction)pymdbm_compress_tree, METH_NOARGS, 
		"compress_tree()"
			"Compresses the existing MDBM directory."
			"Attempts to rebalance the directory and to compress the db to a smaller size."
	},
	{"truncate", (PyCFunction)pymdbm_truncate, METH_NOARGS, 
		"truncate()"
			"Truncates the MDBM to single empty page"
	},
	{"purge", (PyCFunction)pymdbm_purge, METH_NOARGS, 
		"purge()"
			"Purges (removes) all entries from an MDBM."
			"This does not change the MDBM's configuration or general structure."
	},
	{"store", (PyCFunction)pymdbm_store, METH_VARARGS | METH_KEYWORDS, 
		"store(key, val, [flags])"
			"Stores the record specified by the key and val parameters."
	},
	{"fetch", (PyCFunction)pymdbm_fetch, METH_VARARGS, 
		"fetch(key)"
			"Fetches the record specified by the key argument"
	},
	{"get_page", (PyCFunction)pymdbm_get_page, METH_VARARGS, 
		"get_page(key)"
			"Gets the MDBM page number for a given key."
			"The key does not actually have to exist."
	},
	{"delete", (PyCFunction)pymdbm_delete, METH_VARARGS, 
		"delete(key)"
			"Deletes a specific record."
	},

	{"get_hash", (PyCFunction)pymdbm_get_hash, METH_NOARGS, 
		"get_hash()"
			"Gets the MDBM's hash function identifier."
	},
	{"set_hash", (PyCFunction)pymdbm_set_hash, METH_VARARGS, 
		"set_hash(key)"
			"Sets the hashing function for a given MDBM."
			"The hash function must be set before storing anything to the db "
			"(this is not enforced, but entries stored before the hash change will become inaccessible if the hash function is changed)."
			""
			"NOTE:"
			"\tsetting the hash must be be done at creation time, or when there is no data in an MDBM."
			"\tChanging the hash function when there is existing data"
			"\twill result in not being able access that data in the future."
	},
	{"get_alignment", (PyCFunction)pymdbm_get_alignment, METH_NOARGS, 
		"get_alignment()"
			"Gets the MDBM's record byte-alignment."
			"return Alignment mask."
			"\t0 - 8-bit alignment     (mdbm.MDBM_ALIGN_8_BITS)"
			"\t1 - 16-bit alignment    (mdbm.MDBM_ALIGN_16_BITS)"
			"\t3 - 32-bit alignment    (mdbm.MDBM_ALIGN_32_BITS)"
			"\t7 - 64-bit alignment    (mdbm.MDBM_ALIGN_64_BITS)"
	},
	{"get_limit_size", (PyCFunction)pymdbm_get_limit_size, METH_NOARGS, 
		"get_limit_size()"
			"Gets the MDBM's size limit."
			"Returns the limit set for the size of the db using the limit_size_v3 routine."
			"return database size limit"
			"\t0 No limit is set"
			"\tTotal number of bytes for maximum database size, including header and directory"
	},
	{"get_version", (PyCFunction)pymdbm_get_version, METH_NOARGS, 
		"get_version()"
			"Gets the on-disk format version number of an MDBM."
			"return On-disk file format version number"
			"\t2 - MDBM V2"
			"\t3 - MDBM V3"
	},
	{"get_size", (PyCFunction)pymdbm_get_size, METH_NOARGS, 
		"get_size()"
			"Gets the current MDBM's size."
			"return Size of database in bytes."
	},
	{"get_page_size", (PyCFunction)pymdbm_get_page_size, METH_NOARGS, 
		"get_page_size()"
			"Get the MDBM's page size."
			"return Number of bytes in a database page"
	},

	{"get_lockmode", (PyCFunction)pymdbm_get_lockmode, METH_NOARGS,
		"get_lockmode()"
			"Gets the MDBM's lock mode."
			"\t0                      - Exclusive locking"
			"\tMDBM_OPEN_NOLOCK       - No locking"
			"\tMDBM_PARTITIONED_LOCKS - Partitioned locking"
			"\tMDBM_RW_LOCKS          - Shared (read-write) locking"
	},
	{"lock", (PyCFunction)pymdbm_lock, METH_NOARGS, 
		"lock()"
			"Locks the database for exclusive access by the caller."
			"The lock is nestable, so a caller already holding the lock may call lock() again "
			"as long as an equal number of calls to unlock() are made to release the lock."
	},
	{"unlock", (PyCFunction)pymdbm_unlock, METH_NOARGS, 
		"unlock()"
			"Locks the database for exclusive access by the caller."
			"The lock is nestable, so a caller already holding the lock may call lock() again "
			"as long as an equal number of calls to unlock() are made to release the lock."
	},


	{"trylock", (PyCFunction)pymdbm_trylock, METH_NOARGS,
		"trylock()"
			"Attempts to exclusively lock an MDBM."
	},
	{"lock_shared", (PyCFunction)pymdbm_lock_shared, METH_NOARGS,
		"lock_shared()"
			"Locks the database for shared access by readers, excluding access to writers."
			"This is multiple-readers, one writer (MROW) locking."
			"The database must be opened with the MDBM_RW_LOCKS flag to enable shared locks."
			"Use unlock() to release a shared lock."
			""
			"Write access (ex. store() and delete()) must not be done in the context of a shared lock."
			"Write access must be done in the context of an exclusive lock (lock() and \ref unlock())."
			""
			"The lock is nestable, "
			"so a caller already holding the lock may call lock_shared() "
			"again as long as an equal number of calls to unlock() are made to release the lock."
	},
	{"trylock_shared", (PyCFunction)pymdbm_trylock_shared, METH_NOARGS,
		"trylock_shared()"
			"Locks the database for shared access by readers, excluding access to writers."
			"This is the non-blocking version of lock_shared()"
			""
			"This is MROW locking. The database must be opened with the MDBM_RW_LOCKS flag to enable shared locks."
			""
			"Write access (ex. store() and delete()) must not be done in the context of a shared lock."
			"Write access must be done in the context of an exclusive lock (lock() and unlock())."
	},
	{"lock_pages", (PyCFunction)pymdbm_lock_pages, METH_NOARGS,
		"lock_pages"
			"Locks MDBM data pages into memory."
			"When running MDBM as root, lock_pages() will expand the amount of RAM "
			"that can be locked to infinity using setrlimit(RLIMIT_MEMLOCK)."
			"When not running as root, lock_pages() will expand the amount of RAM "
			"that can be locked up to the maximum allowed (retrieved using getrlimit(MEMLOCK),"
			"and normally a very small amount), and if the MDBM is larger than the amount of RAM that can be locked,"
			"a warning will be logged but lock_pages() will return 0 for success."
	},
	{"unlock_pages", (PyCFunction)pymdbm_unlock_pages, METH_NOARGS,
		"unlock_pages"
			"Releases MDBM data pages from always staying in memory"
	},

	{"plock", (PyCFunction)pymdbm_plock, METH_VARARGS | METH_KEYWORDS,
		"plock(key, [flags]) #flags Ignored. Reserved for future use"
			"Locks a specific partition in the database for exclusive access by the caller."
			"The lock is nestable, so a caller already holding "
			"the lock may call plock() again as long as an equal number of calls to punlock()"
			"are made to release the lock."
	},

	{"punlock", (PyCFunction)pymdbm_punlock, METH_VARARGS | METH_KEYWORDS,
		"punlock(key, [flags]) #flags Ignored. Reserved for future use"
			"Unlocks a specific partition in the database, releasing exclusive access by the caller."
			"If the caller has called plock() multiple times in a row,an equal number of unlock calls are required."
			"See plock() for usage."
	},

	{"tryplock", (PyCFunction)pymdbm_tryplock, METH_VARARGS | METH_KEYWORDS,
		"tryplock(key, [flags]) #flags Ignored. Reserved for future use"
			"Tries to locks a specific partition in the database for exclusive access by the caller."
			"The lock is nestable, so a caller already holding the lock may call plock() "
			"again as long as an equal number of calls to punlock() are made to release the lock."
			"See plock() for usage."
	},

	{"lock_smart", (PyCFunction)pymdbm_lock_smart, METH_VARARGS | METH_KEYWORDS,
		"lock_smart(key, [MDBM_PARTITIONED_LOCKS|MDBM_RW_LOCKS|MDBM_O_RDWR|])"
			"Perform either partition, shared or exclusive locking based on the"
			"locking-related flags supplied to open()."
			"NOTE:"
			"\tmdbm::open(MDBM_PARTITIONED_LOCKS) means mdbm::lock_smart() calls mdbm::plock(), database has been corrupted."
			"\tmdbm::open(MDBM_PARTITIONED_LOCKS) means mdbm::lock_smart() calls mdbm::plock(),"
			"\tmdbm::open(MDBM_RW_LOCKS) means mdbm::lock_smart() calls mdbm::lock_shared() for reads, and"
			"\tmdbm::open() for all other locking flags or for writes, mdbm::lock_smart() will call mdbm_lock."
	},

	{"trylock_smart", (PyCFunction)pymdbm_trylock_smart, METH_VARARGS | METH_KEYWORDS,
		"trylock_smart(key, [MDBM_PARTITIONED_LOCKS|MDBM_RW_LOCKS|MDBM_O_RDWR|])"
			"Attempts to lock an MDBM based on the locking flags supplied to open()."
			"NOTE:"
			"\tmdbm::open(MDBM_PARTITIONED_LOCKS) means mdbm::trylock_smart() calls mdbm::tryplock(),"
			"\tmdbm::open(MDBM_RW_LOCKS) means mdbm::trylock_smart() calls mdbm::trylock_shared() for reads, and"
			"\tmdbm::pen() for all other locking flags or for writes, mdbm::trylock_smart() will call mdbm::trylock()."
	},

	{"unlock_smart", (PyCFunction)pymdbm_unlock_smart, METH_VARARGS | METH_KEYWORDS,
		"unlock_smart(key, [MDBM_PARTITIONED_LOCKS|MDBM_RW_LOCKS|MDBM_O_RDWR|])"
			"NOTE:"
			"\tUnlock an MDBM based on the locking flags supplied to open()."
			"\tmdbm::open(MDBM_PARTITIONED_LOCKS) means mdbm::lock_smart() calls mdbm::punlock(),otherwise calls mdbm::unlock()"
	},
	{"first", (PyCFunction)pymdbm_first, METH_NOARGS, 
		"first()"
			"Returns the first key/value pair from the database."
			"The order that records are returned is not specified."
	},
	{"next", (PyCFunction)pymdbm_next, METH_NOARGS, 
		"next()"
			"Returns the next key/value pair from the database."
			"The order that records are returned is not specified."
	},
	{"firstkey", (PyCFunction)pymdbm_firstkey, METH_NOARGS, 
		"firstkey()"
			"Returns the first key from the database."
			"The order that records are returned is not specified."
	},
	{"nextkey", (PyCFunction)pymdbm_nextkey, METH_NOARGS, 
		"nextkey()"
			"Returns the next key pair from the database."
			"The order that records are returned is not specified."
	},
	{"islocked", (PyCFunction)pymdbm_islocked, METH_NOARGS, 
		"islocked()"
			"returns whether or not mdbm is locked by another process or thread."
	},
	{"isowned", (PyCFunction)pymdbm_isowned, METH_NOARGS, 
		"isowned()"
			"Returns whether or not MDBM is currently locked (owned) by the calling process."
			"em Owned MDBMs have multiple nested locks in place."
	},
	{"lock_reset", (PyCFunction)pymdbm_lock_reset, METH_VARARGS, 
		"lock_reset(mdbm_file_path)"
			"Resets the global lock ownership state of a database."
			"*USE THIS FUNCTION WITH EXTREME CAUTION!*"
			"The global lock ownership state of an MDBM is visible to other"
			"processes and is reset system-wide.  Resetting a lock state while other"
			"threads/processes are accessing this database might cause those operations to fail."
	},
	{"delete_lockfiles", (PyCFunction)pymdbm_delete_lockfiles, METH_VARARGS, 
		"delete_lockfiles(mdbm_file_path)"
			"Removes all lockfiles associated with an MDBM file."
			"USE THIS FUNCTION WITH EXTREME CAUTION!"
			"NOTE: This is only intended to clean-up lockfiles when removing an MDBM file."
			"This function can only be used when all processes that access"
			"the MDBM whose locks are being deleted are not running."
			"Calling it on an MDBM still in use will cause corruption and undefined behavior."
			"Deleting lockfiles resets lock ownership and locking mode (exclusive/partition/shared)."
	},
	{"check", (PyCFunction)pymdbm_check, METH_VARARGS | METH_KEYWORDS , 
		"check(level, verbose)"
			"Checks an MDBM's integrity, and displays info. on standard output."
			"\tlevel : between 0 and 10"
			"\tverbose : True or False"
	},
	{"chk_page", (PyCFunction)pymdbm_chk_page, METH_VARARGS, 
		"chk_all_page(page_num)"
			"Checks the specified page for errors."
			"It will print errors found on the page, "
			"including bad key size, bad val size, and bad offsets of various fields."
			"Ref : See get_page()"
	},
	{"chk_all_page", (PyCFunction)pymdbm_chk_all_page, METH_VARARGS, 
		"chk_all_page()"
			"Checks the database for errors."
			"It will report same as mdbm_chk_page for all pages in the database."
			"See chk_page() to determine if errors detected in the database."
	},
	{"count_records", (PyCFunction)pymdbm_count_records, METH_VARARGS, 
		"count_records()"
			"Counts the number of records in an MDBM."
	},
	{"count_pages", (PyCFunction)pymdbm_count_pages, METH_VARARGS, 
		"count_pages()"
			"Counts the number of pages used by an MDBM."
			"returns Number of pages: Count of all directory+data+LargeObject pages used by an MDBM."
	},
	{"get_errno", (PyCFunction)pymdbm_get_errno, METH_VARARGS, 
		"get_errno()"
			"Returns the value of internally saved errno."
			"Contains the value of errno that is set during some lock failures."
			"Under other circumstances, mdbm_get_errno will not return the actual value of the errno variable."
	},
	{"protect", (PyCFunction)pymdbm_protect, METH_VARARGS,
		"protect(([MDBM_PROT_NONE|MDBM_PROT_READ|MDBM_PROT_WRITE|MDBM_PROT_ACCESS|MDBM_PROT_NOACCESS]))"
			"Sets all database pages to protect permission."
			"This function is for advanced users only."
			"Users that want to use the built-in protect feature "
			"should specify MDBM_PROTECT in their mdbm::open flags."
			"NOTE:"
			"\tRHEL is unable to set MDBM_PROT_WRITE without MDBM_PROT_READ, so specifying"
			"\tMDBM_PROT_WRITE does not protect against reads."
			"Values for protect mask:"
			"\tMDBM_PROT_NONE     - no access"
			"\tMDBM_PROT_READ     - read access"
			"\tMDBM_PROT_WRITE    - write access"
			"\tMDBM_PROT_ACCESS   - all access"
			"\tMDBM_PROT_NOACCESS - no access (same as MDBM_PROT_NONE)"
	},

	{0,0}
};

char mdbmmod_docs[] = "The MDBM module.";
char mdbmmod_name[] = "mdbm";

static void mdbm_dealloc(register MDBMObj *pmdbm_link) {
    if ( pmdbm_link->pmdbm != NULL ) {
        mdbm_close(pmdbm_link->pmdbm);
    }
    PyObject_Del(pmdbm_link);
}

static inline char *copy_strptr(char *dptr, int dsize) {

    char *pretval = NULL;

    pretval = (char *) PyMem_Malloc(dsize+1);
    if (pretval == NULL) {
		PyErr_NoMemory();
        return NULL;
    }

    memset(pretval, 0x00, dsize+1);

    strncpy(pretval, dptr, dsize);
    if (pretval == NULL) {
        return NULL;
    }

    return pretval;
}

#if PY_MAJOR_VERSION <= 2
static PyObject *mdbm_getattr(MDBMObj *pmdbm_link, char *name) {

    if (pmdbm_link->pmdbm == NULL) {
        PyErr_SetString(PyExc_RuntimeError, "not an exists open the mdbm");
        return NULL;
    }
    return Py_FindMethod(mdbm_methods, (PyObject *)pmdbm_link, name);

}
#endif


#if PY_MAJOR_VERSION >= 3
static int mdbm_traverse(PyObject *m, visitproc visit, void *arg) {
    Py_VISIT(GETSTATE(m)->error);
    return 0;
}

static int mdbm_clear(PyObject *m) {
    Py_CLEAR(GETSTATE(m)->error);
    return 0;
}
#endif



#if PY_MAJOR_VERSION >= 3
static PyTypeObject MDBMType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "mdbm",
    sizeof(MDBMObj),
    0,
    (destructor)mdbm_dealloc,       /*tp_dealloc*/
    0,                              /*tp_print*/
    0,                              /*tp_getattr*/
    0,                              /*tp_setattr*/
    0,                              /*tp_reserved*/
    0,                              /*tp_repr*/
    0,                              /*tp_as_number*/
    0,                              /*tp_as_sequence*/
    0,                              /*tp_as_mapping*/
    0,                              /*tp_hash*/
    0,                              /*tp_call*/
    0,                              /*tp_str*/
    0,                              /*tp_getattro*/
    0,                              /*tp_setattro*/
    0,                              /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,             /*tp_flags*/
    0,                              /*tp_doc*/
    mdbm_traverse,                  /*tp_traverse*/
    mdbm_clear,                     /*tp_clear*/
    0,                              /*tp_richcompare*/
    0,                              /*tp_weaklistoffset*/
    0,                              /*tp_iter*/
    0,                              /*tp_iternext*/
    mdbm_methods,                   /*tp_methods*/
};

static struct PyModuleDef MDBMModule = {
    PyModuleDef_HEAD_INIT,
    "mdbm",
    NULL,
    -1,
    mdbm_methods,
    NULL,
    NULL,
    NULL,
    NULL
};



#else
static PyTypeObject MDBMType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "mdbm",
    sizeof(MDBMObj),
    0,
    (destructor)mdbm_dealloc,       /*tp_dealloc*/
    0,                              /*tp_print*/
    (getattrfunc)mdbm_getattr,      /*tp_getattr*/
    0,                              /*tp_setattr*/
    0,                              /*tp_compare*/
    0,                              /*tp_repr*/
    0,                              /*tp_as_number*/
    0,                              /*tp_as_sequence*/
    0,                              /*tp_as_mapping*/
    0,                              /*tp_hash*/
    0,                              /*tp_call*/
    0,                              /*tp_str*/
    0,                              /*tp_getattro*/
    0,                              /*tp_setattro*/
    0,                              /*tp_as_buffer*/
    Py_TPFLAGS_DEFAULT,             /*tp_flags*/
    "https://github.com/torden/py-mdbm", /*tp_docs*/
};
#endif


#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_mdbm(void) {
#else
PyMODINIT_FUNC initmdbm(void) {
#endif

    PyObject *m = NULL;
    PyObject *d = NULL;
    PyObject *s = NULL;

#if PY_MAJOR_VERSION <= 2
    MDBMType.ob_type = &PyType_Type; //FIX #3: SIGSEG on python2.6
    m = Py_InitModule3(mdbmmod_name, mdbm_methods, mdbmmod_docs);
    if (m == NULL) {
        return;
    }

#else
    if (PyType_Ready(&MDBMType) < 0) {
        return NULL;
    }

    m = PyModule_Create(&MDBMModule);
    if (m == NULL) {
        return NULL;
    }
#endif

    d = PyModule_GetDict(m);
    if (MDBMError == NULL) {
        MDBMError = PyErr_NewException("mdbm.error", PyExc_IOError, NULL);
		Py_INCREF(MDBMError);
    }

    s = _PYUNICODE("The MDBM");
    if (s != NULL) {
        PyDict_SetItemString(d, "library", s);
        Py_DECREF(s);
    }

    if (MDBMError != NULL) {
        PyDict_SetItemString(d, "error", MDBMError);
    }

    if (PyErr_Occurred()) {
        Py_DECREF(m);
        m = NULL;
    }

    PyModule_AddIntMacro(m, MDBM_LOG_OFF);
    PyModule_AddIntMacro(m, MDBM_LOG_EMERGENCY);
    PyModule_AddIntMacro(m, MDBM_LOG_ALERT);
    PyModule_AddIntMacro(m, MDBM_LOG_CRITICAL);
    PyModule_AddIntMacro(m, MDBM_LOG_ERROR);
    PyModule_AddIntMacro(m, MDBM_LOG_WARNING);
    PyModule_AddIntMacro(m, MDBM_LOG_NOTICE);
    PyModule_AddIntMacro(m, MDBM_LOG_INFO);
    PyModule_AddIntMacro(m, MDBM_LOG_DEBUG);
    PyModule_AddIntMacro(m, MDBM_LOG_DEBUG2);
    PyModule_AddIntMacro(m, MDBM_LOG_DEBUG3);
    PyModule_AddIntMacro(m, MDBM_LOG_MAXLEVEL);
    PyModule_AddIntMacro(m, MDBM_LOG_ABORT);
    PyModule_AddIntMacro(m, MDBM_LOG_FATAL);


    PyModule_AddIntMacro(m, MDBM_KEYLEN_MAX);
    PyModule_AddIntMacro(m, MDBM_VALLEN_MAX);
    PyModule_AddIntMacro(m, MDBM_LOC_NORMAL);
    PyModule_AddIntMacro(m, MDBM_LOC_ARENA);
    PyModule_AddIntMacro(m, MDBM_O_RDONLY);
    PyModule_AddIntMacro(m, MDBM_O_WRONLY);
    PyModule_AddIntMacro(m, MDBM_O_RDWR);
    PyModule_AddIntMacro(m, MDBM_O_ACCMODE);
    PyModule_AddIntMacro(m, MDBM_O_CREAT);
    PyModule_AddIntMacro(m, MDBM_O_TRUNC);
    PyModule_AddIntMacro(m, MDBM_O_FSYNC);
    PyModule_AddIntMacro(m, MDBM_O_ASYNC);
    PyModule_AddIntMacro(m, MDBM_O_DIRECT);
    PyModule_AddIntMacro(m, MDBM_NO_DIRTY);
    PyModule_AddIntMacro(m, MDBM_SINGLE_ARCH);
    PyModule_AddIntMacro(m, MDBM_OPEN_WINDOWED);
    PyModule_AddIntMacro(m, MDBM_PROTECT);
    PyModule_AddIntMacro(m, MDBM_DBSIZE_MB);
    PyModule_AddIntMacro(m, MDBM_STAT_OPERATIONS);
    PyModule_AddIntMacro(m, MDBM_LARGE_OBJECTS);
    PyModule_AddIntMacro(m, MDBM_PARTITIONED_LOCKS);
    PyModule_AddIntMacro(m, MDBM_RW_LOCKS);
    PyModule_AddIntMacro(m, MDBM_ANY_LOCKS);
    PyModule_AddIntMacro(m, MDBM_CREATE_V3);
    PyModule_AddIntMacro(m, MDBM_OPEN_NOLOCK);
    PyModule_AddIntMacro(m, MDBM_DEMAND_PAGING);
    PyModule_AddIntMacro(m, MDBM_DBSIZE_MB_OLD);
    PyModule_AddIntMacro(m, MDBM_COPY_LOCK_ALL);
    PyModule_AddIntMacro(m, MDBM_SAVE_COMPRESS_TREE);
    PyModule_AddIntMacro(m, MDBM_ALIGN_8_BITS);
    PyModule_AddIntMacro(m, MDBM_ALIGN_16_BITS);
    PyModule_AddIntMacro(m, MDBM_ALIGN_32_BITS);
    PyModule_AddIntMacro(m, MDBM_ALIGN_64_BITS);
    PyModule_AddIntMacro(m, MDBM_MAGIC);
    PyModule_AddIntMacro(m, MDBM_FETCH_FLAG_DIRTY);
    PyModule_AddIntMacro(m, MDBM_INSERT);
    PyModule_AddIntMacro(m, MDBM_REPLACE);
    PyModule_AddIntMacro(m, MDBM_INSERT_DUP);
    PyModule_AddIntMacro(m, MDBM_MODIFY);
    PyModule_AddIntMacro(m, MDBM_STORE_MASK);
    PyModule_AddIntMacro(m, MDBM_RESERVE);
    PyModule_AddIntMacro(m, MDBM_CLEAN);
    PyModule_AddIntMacro(m, MDBM_CACHE_ONLY);
    PyModule_AddIntMacro(m, MDBM_CACHE_REPLACE);
    PyModule_AddIntMacro(m, MDBM_CACHE_MODIFY);
    PyModule_AddIntMacro(m, MDBM_STORE_SUCCESS);
    PyModule_AddIntMacro(m, MDBM_STORE_ENTRY_EXISTS);
    PyModule_AddIntMacro(m, MDBM_ENTRY_DELETED);
    PyModule_AddIntMacro(m, MDBM_ENTRY_LARGE_OBJECT);
    PyModule_AddIntMacro(m, MDBM_ITERATE_ENTRIES);
    PyModule_AddIntMacro(m, MDBM_ITERATE_NOLOCK);
    PyModule_AddIntMacro(m, MDBM_LOCKMODE_UNKNOWN);
    PyModule_AddIntMacro(m, MDBM_CHECK_HEADER);
    PyModule_AddIntMacro(m, MDBM_CHECK_CHUNKS);
    PyModule_AddIntMacro(m, MDBM_CHECK_DIRECTORY);
    PyModule_AddIntMacro(m, MDBM_CHECK_ALL);
    PyModule_AddIntMacro(m, MDBM_PROT_NONE);
    PyModule_AddIntMacro(m, MDBM_PROT_READ);
    PyModule_AddIntMacro(m, MDBM_PROT_WRITE);
    PyModule_AddIntMacro(m, MDBM_PROT_NOACCESS);
    PyModule_AddIntMacro(m, MDBM_PROT_ACCESS);
    PyModule_AddIntMacro(m, MDBM_CLOCK_STANDARD);
    PyModule_AddIntMacro(m, MDBM_CLOCK_TSC);
    PyModule_AddIntMacro(m, MDBM_STATS_BASIC);
    PyModule_AddIntMacro(m, MDBM_STATS_TIMED);
    PyModule_AddIntMacro(m, MDBM_STAT_CB_INC);
    PyModule_AddIntMacro(m, MDBM_STAT_CB_SET);
    PyModule_AddIntMacro(m, MDBM_STAT_CB_ELAPSED);
    PyModule_AddIntMacro(m, MDBM_STAT_CB_TIME);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_FETCH);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_STORE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_DELETE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_LOCK);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_FETCH_UNCACHED);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_GETPAGE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_GETPAGE_UNCACHED);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_CACHE_EVICT);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_CACHE_STORE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_PAGE_STORE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_PAGE_DELETE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_SYNC);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_FETCH_NOT_FOUND);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_FETCH_ERROR);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_STORE_ERROR);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_DELETE_FAILED);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_FETCH_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_STORE_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_DELETE_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_FETCH_TIME);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_STORE_TIME);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_DELETE_TIME);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_FETCH_UNCACHED_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_GETPAGE_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_GETPAGE_UNCACHED_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_CACHE_EVICT_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_CACHE_STORE_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_PAGE_STORE_VALUE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_PAGE_DELETE_VALUE);
    PyModule_AddIntMacro(m, MDBM_STAT_TAG_SYNC_LATENCY);
    PyModule_AddIntMacro(m, MDBM_STAT_DELETED);
    PyModule_AddIntMacro(m, MDBM_STAT_KEYS);
    PyModule_AddIntMacro(m, MDBM_STAT_VALUES);
    PyModule_AddIntMacro(m, MDBM_STAT_PAGES_ONLY);
    PyModule_AddIntMacro(m, MDBM_STAT_NOLOCK);
    PyModule_AddIntMacro(m, MDBM_STAT_BUCKETS);
    PyModule_AddIntMacro(m, MDBM_CACHEMODE_NONE);
    PyModule_AddIntMacro(m, MDBM_CACHEMODE_LFU);
    PyModule_AddIntMacro(m, MDBM_CACHEMODE_LRU);
    PyModule_AddIntMacro(m, MDBM_CACHEMODE_GDSF);
    PyModule_AddIntMacro(m, MDBM_CACHEMODE_MAX);
    PyModule_AddIntMacro(m, MDBM_CACHEMODE_EVICT_CLEAN_FIRST);
    PyModule_AddIntMacro(m, MDBM_CACHEMODE_BITS);
    PyModule_AddIntMacro(m, MDBM_MINPAGE);
    PyModule_AddIntMacro(m, MDBM_PAGE_ALIGN);
    PyModule_AddIntMacro(m, MDBM_MAXPAGE);
    PyModule_AddIntMacro(m, MDBM_PAGESIZ);
    PyModule_AddIntMacro(m, MDBM_MIN_PSHIFT);
    PyModule_AddIntMacro(m, MDBM_MAX_SHIFT);
    PyModule_AddIntMacro(m, MDBM_HASH_CRC32);
    PyModule_AddIntMacro(m, MDBM_HASH_EJB);
    PyModule_AddIntMacro(m, MDBM_HASH_PHONG);
    PyModule_AddIntMacro(m, MDBM_HASH_OZ);
    PyModule_AddIntMacro(m, MDBM_HASH_TOREK);
    PyModule_AddIntMacro(m, MDBM_HASH_FNV);
    PyModule_AddIntMacro(m, MDBM_HASH_STL);
    PyModule_AddIntMacro(m, MDBM_HASH_MD5);
    PyModule_AddIntMacro(m, MDBM_HASH_SHA_1);
    PyModule_AddIntMacro(m, MDBM_HASH_JENKINS);
    PyModule_AddIntMacro(m, MDBM_HASH_HSIEH);
    PyModule_AddIntMacro(m, MDBM_MAX_HASH);
    PyModule_AddIntMacro(m, MDBM_CONFIG_DEFAULT_HASH);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif

}


// METHODS
PyObject *pymdbm_open(PyObject *self, PyObject *args, PyObject *kwds) {

    const char *pfn = NULL;
    int flags = 0;
    int mode = 0;
    int psize = 0;
    int presize = 0;

    int rv = -1;


    static char *pkwlist[] = {"path", "flags", "mode", "psize", "presize", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "sii|ii", pkwlist, &pfn, &flags, &mode, &psize, &presize);
    if (!rv) {
        PyErr_SetString(MDBMError, "required filepath and flags and mode");
        return NULL;
    }

    MDBMObj *pmdbm_link = NULL;

    pmdbm_link = PyObject_New(MDBMObj, &MDBMType);
    if (pmdbm_link == NULL) {
        return NULL;
    }

	//protect : sigfault
	if (flags == (flags | MDBM_O_CREAT) && flags == (flags | MDBM_PROTECT)) {
	    PyErr_SetString(MDBMError, "failed to open the MDBM, not support create flags with MDBM_PROTECT");
        return NULL;
	}

	if (flags == (flags | MDBM_O_ASYNC) && flags == (flags | MDBM_O_FSYNC)) {
	    PyErr_SetString(MDBMError, "failed to open the MDBM, not support mixed sync flags (MDBM_O_FSYNC, MDBM_O_ASYNC)");
        return NULL;
	}

	if (flags == (flags | MDBM_O_RDONLY) && flags == (flags | MDBM_O_WRONLY)) {
	    PyErr_SetString(MDBMError, "failed to open the MDBM, not support mixed access flags (MDBM_O_RDONLY, MDBM_O_WRONLY, MDBM_O_RDWR)");
        return NULL;
	}

    CAPTURE_START();
    pmdbm_link->pmdbm = mdbm_open(pfn, flags, mode, psize, presize);
    CAPTURE_END();
    if (pmdbm_link->pmdbm == NULL) {
        PyErr_SetFromErrno(MDBMError);
        Py_DECREF(pmdbm_link);
    }

    return (PyObject *)pmdbm_link;
}

PyObject *pymdbm_log_minlevel(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    int level = -1;

    rv = PyArg_ParseTuple(args, "i", &level);
    if (!rv) {
        PyErr_SetString(MDBMError, "required string(key)");
        return NULL;
    }

	if (level < MDBM_LOG_OFF || level > MDBM_LOG_DEBUG) {
        PyErr_Format(MDBMError, "mdbm::log_minlevel does not support level(=%d)", level);
        return NULL;
	}

    loglevel = level;

    CAPTURE_START();
    mdbm_log_minlevel(level);
    CAPTURE_END();

    _RETURN_NONE();
}

PyObject *pymdbm_close(register MDBMObj *pmdbm_link, PyObject *unused) {

    if ( pmdbm_link->pmdbm != NULL ) {
        mdbm_close(pmdbm_link->pmdbm);
        pmdbm_link->pmdbm = NULL;
    } else {
        PyErr_SetString(MDBMError, "failed to close an existing MDBM");
    }

    _RETURN_NONE();
}

PyObject *pymdbm_close_fd(register MDBMObj *pmdbm_link, PyObject *unused) {

    if ( pmdbm_link->pmdbm != NULL ) {
        mdbm_close_fd(pmdbm_link->pmdbm);
        pmdbm_link->pmdbm = NULL;
    } else {
        PyErr_SetString(MDBMError, "failed to close an existing MDBM");
    }

    _RETURN_NONE();
}



PyObject *pymdbm_sync(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_sync(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}


PyObject *pymdbm_fsync(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_fsync(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_preload(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_preload(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_compress_tree(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_compress_tree(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_NONE();
}

PyObject *pymdbm_truncate(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_truncate(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_NONE();
}

PyObject *pymdbm_purge(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_purge(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_NONE();
}

PyObject *pymdbm_store(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pkey = NULL;
    char *pval = NULL;
    int flags = MDBM_INSERT;

    int rv = -1;
    datum key, val;

    static char *pkwlist[] = {"key", "val", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "ss|i", pkwlist, &pkey, &pval, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "required key and value");
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);
    val.dptr = pval;
    val.dsize = (int)strlen(pval);

    CAPTURE_START();
    rv = mdbm_store(pmdbm_link->pmdbm, key, val, (int)flags);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}


PyObject *pymdbm_fetch(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;

    int rv = -1;
    datum key, val;

    rv = PyArg_ParseTuple(args, "s", &pkey);
    if (!rv) {
        PyErr_SetString(MDBMError, "required string(key)");
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    val = mdbm_fetch(pmdbm_link->pmdbm, key);
    CAPTURE_END();

    if (val.dptr == NULL) {
        _RETURN_FALSE();
    }
 
    return _PYUNICODE_ANDSIZE(val.dptr, val.dsize);
    //return Py_BuildValue("(u)", _PYUNICODE_ANDSIZE(val.dptr, val.dsize));
}

PyObject *pymdbm_get_page(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;
    mdbm_ubig_t rv = -1;
    datum key;

    rv = PyArg_ParseTuple(args, "s", &pkey);
    if (!rv) {
        PyErr_SetString(MDBMError, "required string(key)");
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_get_page(pmdbm_link->pmdbm, &key);
    CAPTURE_END();

    if (rv == (mdbm_ubig_t)-1) {
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_delete(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;

    int rv = -1;
    datum key;

    rv = PyArg_ParseTuple(args, "s", &pkey);
    if (!rv) {
        PyErr_SetString(MDBMError, "required string(key)");
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_delete(pmdbm_link->pmdbm, key);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_get_hash(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_get_hash(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::get_hash() does not obtain the current MDBM's hash func code");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_set_hash(register MDBMObj *pmdbm_link, PyObject *args) {

    int hash = -1; 
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &hash);
    if (!rv) {
        PyErr_SetString(MDBMError, "required key and value");
        _RETURN_FALSE();
    }
    if (hash < MDBM_HASH_CRC32 || hash > MDBM_MAX_HASH) {
        PyErr_Format(MDBMError, "mdbm::set_hash does not support hash(=%d)", hash);
        _RETURN_FALSE();
    }

    CAPTURE_START();
    rv = mdbm_set_hash(pmdbm_link->pmdbm, hash);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_get_alignment(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_get_alignment(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::get_alignment() does not obtain the MDBM's record byte-alignment.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_limit_size(register MDBMObj *pmdbm_link, PyObject *unused) {

    uint64_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_limit_size(pmdbm_link->pmdbm);
    CAPTURE_END();
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_version(register MDBMObj *pmdbm_link, PyObject *unused) {

    uint32_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_version(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == 0) {
        PyErr_SetString(MDBMError, "mdbm::get_version() does not obtain the on-disk format version number of an MDBM.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_size(register MDBMObj *pmdbm_link, PyObject *unused) {

    uint64_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_size(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == 0) {
        PyErr_SetString(MDBMError, "mdbm::get_size() does not obtain the current MDBM's size");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_page_size(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_get_page_size(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::get_page_size() does not obtain the current MDBM's size");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_lockmode(register MDBMObj *pmdbm_link, PyObject *unused) {

    uint32_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_lockmode(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::get_lockmode() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}


PyObject *pymdbm_lock(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_lock(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::lock() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_unlock(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_unlock(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::unlock() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_trylock(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_trylock(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::trylock() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_lock_shared(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_lock_shared(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::lock_shared() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_trylock_shared(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_trylock_shared(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::trylock_shared() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_lock_pages(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_lock_pages(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::lock_pages() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_unlock_pages(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_unlock_pages(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "mdbm::unlock_pages() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_islocked(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_islocked(pmdbm_link->pmdbm);
    CAPTURE_END();
	switch(rv) {
		case 0:
			_RETURN_FALSE();
			break;
		case 1:
			_RETURN_TRUE();
			break;
		default:
        	PyErr_SetString(MDBMError, "mdbm::islocked() does not obtain the current MDBM's lock.");
			break;
    }

	_RETURN_NONE();
}

PyObject *pymdbm_isowned(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_isowned(pmdbm_link->pmdbm);
    CAPTURE_END();
	switch(rv) {
		case 0:
			_RETURN_FALSE();
			break;
		case 1:
			_RETURN_TRUE();
			break;
		default:
        	PyErr_SetString(MDBMError, "mdbm::islocked() does not obtain the current MDBM's lock owner.");
			break;
    }

	_RETURN_NONE();
}

PyObject *pymdbm_lock_reset(register MDBMObj *pmdbm_link, PyObject *args) {

	char *pfn = NULL;
    int rv = -1;
	int flags = 0;

    rv = PyArg_ParseTuple(args, "s", &pfn);
    if (!rv) {
        PyErr_SetString(MDBMError, "required string(file_path)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_lock_reset(pfn, flags); // flags Reserved for future use, and must be 0.
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_delete_lockfiles(register MDBMObj *pmdbm_link, PyObject *args) {

	char *pfn = NULL;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "s", &pfn);
    if (!rv) {
        PyErr_SetString(MDBMError, "required string(file_path)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_delete_lockfiles(pfn);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}



PyObject *pymdbm_first(register MDBMObj *pmdbm_link, PyObject *unused) {

    kvpair kv;
    PyObject *retval;
    char *pretkey;
    char *pretval;

    CAPTURE_START();
    kv = mdbm_first(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (kv.key.dptr == NULL || kv.val.dptr == NULL) {
        _RETURN_FALSE();
    }

    pretkey = copy_strptr(kv.key.dptr, kv.key.dsize);
    if (pretkey == NULL) {
        _RETURN_FALSE();
    }

    pretval = copy_strptr(kv.val.dptr, kv.val.dsize);
    if (pretval == NULL) {
        _RETURN_FALSE();
    }

    retval = PyTuple_New(2);
    PyTuple_SetItem(retval, 0, _PYUNICODE(pretkey));
    PyTuple_SetItem(retval, 1, _PYUNICODE(pretval));

	PyMem_Free(pretkey);
	PyMem_Free(pretval);

    return retval;
}

PyObject *pymdbm_next(register MDBMObj *pmdbm_link, PyObject *unused) {

    kvpair kv;
    PyObject *retval;
    char *pretkey;
    char *pretval;

    CAPTURE_START();
    kv = mdbm_next(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (kv.key.dptr == NULL || kv.val.dptr == NULL) {
        _RETURN_FALSE();
    }

    pretkey = copy_strptr(kv.key.dptr, kv.key.dsize);
    if (pretkey == NULL) {
        _RETURN_FALSE();
    }

    pretval = copy_strptr(kv.val.dptr, kv.val.dsize);
    if (pretval == NULL) {
        _RETURN_FALSE();
    }

    retval = PyTuple_New(2);
    PyTuple_SetItem(retval, 0, _PYUNICODE(pretkey));
    PyTuple_SetItem(retval, 1, _PYUNICODE(pretval));
	
	PyMem_Free(pretkey);
	PyMem_Free(pretval);
    return retval;
}

PyObject *pymdbm_firstkey(register MDBMObj *pmdbm_link, PyObject *unused) {

    datum key;
    PyObject *retval;
    char *pretkey;

    CAPTURE_START();
    key = mdbm_firstkey(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (key.dptr == NULL) {
        _RETURN_FALSE();
    }

    pretkey = copy_strptr(key.dptr, key.dsize);
    if (pretkey == NULL) {
        _RETURN_FALSE();
    }


    retval = PyTuple_New(1);
    PyTuple_SetItem(retval, 0, _PYUNICODE(pretkey));

	PyMem_Free(pretkey);
	//Py_DECREF(retval);

    return retval;
}

PyObject *pymdbm_nextkey(register MDBMObj *pmdbm_link, PyObject *unused) {

    datum key;
    PyObject *retval;
    char *pretkey;

    CAPTURE_START();
    key = mdbm_nextkey(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (key.dptr == NULL) {
        _RETURN_FALSE();
    }

    pretkey = copy_strptr(key.dptr, key.dsize);
    if (pretkey == NULL) {
        _RETURN_FALSE();
    }


    retval = PyTuple_New(1);
    PyTuple_SetItem(retval, 0, _PYUNICODE(pretkey));
	PyMem_Free(pretkey);
	//Py_DECREF(retval);

    return retval;
}

PyObject *pymdbm_check(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

	int level = -1;
	int verbose = -1;

    int rv = -1;

    static char *pkwlist[] = {"level", "verbose", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "i|i", pkwlist, &level, &verbose);
    if (!rv) {
        PyErr_SetString(MDBMError, "required level(10 >= level >= 0), [verbose(True | False)]");
        return NULL;
    }

	if (level < 0 || level > 10) {
        PyErr_Format(MDBMError, "mdbm::check does not support level(=%d)", level);
        return NULL;
	}

	if (verbose < 0 || verbose > 1) {
        PyErr_Format(MDBMError, "mdbm::check does not support verbose(=%d)", verbose);
        return NULL;
	}

    CAPTURE_START();
    rv = mdbm_check(pmdbm_link->pmdbm, level, verbose);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_chk_page(register MDBMObj *pmdbm_link, PyObject *args) {

	int pagenum = -1;

    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &pagenum);
    if (!rv) {
        PyErr_SetString(MDBMError, "required int(pagenum)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_chk_page(pmdbm_link->pmdbm, pagenum);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_chk_all_page(register MDBMObj *pmdbm_link, PyObject *unused) {

	int rv = -1;
    CAPTURE_START();
    rv = mdbm_chk_all_page(pmdbm_link->pmdbm);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_count_records(register MDBMObj *pmdbm_link, PyObject *unused) {

	uint64_t rv = -1;
    CAPTURE_START();
    rv = mdbm_count_records(pmdbm_link->pmdbm);
    CAPTURE_END();

    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_count_pages(register MDBMObj *pmdbm_link, PyObject *unused) {

	mdbm_ubig_t rv = -1;
    CAPTURE_START();
    rv = mdbm_count_pages(pmdbm_link->pmdbm);
    CAPTURE_END();

    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_errno(register MDBMObj *pmdbm_link, PyObject *unused) {

	mdbm_ubig_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_errno(pmdbm_link->pmdbm);
    CAPTURE_END();

    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_plock(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

	char *pkey = NULL;
	int flags = 0;

    datum key;
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "required str(key)");
        return NULL;
    }

	if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "mdbm::plock does not support flags(=%d)", flags);
        return NULL;
	}

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_plock(pmdbm_link->pmdbm, &key, flags);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_punlock(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

	char *pkey = NULL;
	int flags = 0;

    datum key;
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags); 
    if (!rv) {
        PyErr_SetString(MDBMError, "required str(key)");
        return NULL;
    }

	if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "mdbm::punlock does not support flags(=%d)", flags);
        return NULL;
	}

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_punlock(pmdbm_link->pmdbm, &key, flags);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_tryplock(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

	char *pkey = NULL;
	int flags = 0;

    datum key;
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "required str(key)");
        return NULL;
    }

	if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "mdbm::tryplock does not support flags(=%d)", flags);
        return NULL;
	}

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_tryplock(pmdbm_link->pmdbm, &key, flags);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_lock_smart(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

	char *pkey = NULL;
	int flags = 0;

    datum key;
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "required str(key)");
        return NULL;
    }

	if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "mdbm::lock_smart does not support flags(=%d)", flags);
        return NULL;
	}

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_lock_smart(pmdbm_link->pmdbm, &key, flags);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_trylock_smart(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

	char *pkey = NULL;
	int flags = 0;

    datum key;
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags); 
    if (!rv) {
        PyErr_SetString(MDBMError, "required str(key)");
        return NULL;
    }

	if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "mdbm::trylock_smart does not support flags(=%d)", flags);
        return NULL;
	}

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_trylock_smart(pmdbm_link->pmdbm, &key, flags);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_unlock_smart(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

	char *pkey = NULL;
	int flags = 0;

    datum key;
    int rv = -1;

    
    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "required str(key)");
        return NULL;
    }

	if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "mdbm::unlock_smart does not support flags(=%d)", flags);
        return NULL;
	}

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_unlock_smart(pmdbm_link->pmdbm, &key, flags);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_protect(register MDBMObj *pmdbm_link, PyObject *args) {

	int protect = -1;

    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &protect);
    if (!rv) {
        PyErr_SetString(MDBMError, "required str(key)");
        return NULL;
    }

	if (protect < MDBM_PROT_NONE || protect > MDBM_PROT_ACCESS) {
        PyErr_Format(MDBMError, "mdbm::protect does not support protect(=%d)", protect);
        return NULL;
	}

    CAPTURE_START();
    rv = mdbm_protect(pmdbm_link->pmdbm, protect);
    CAPTURE_END();

	_RETURN_RV_BOOLEN(rv);
}

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
