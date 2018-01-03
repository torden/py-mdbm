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

#define CAPTURE_START() {\
    dev_null = open("/dev/null", O_WRONLY);\
    org_stdout = dup(STDOUT_FILENO);\
    org_stderr = dup(STDERR_FILENO);\
    if (loglevel == -1) {\
        dup2(dev_null, STDOUT_FILENO);\
        dup2(dev_null, STDERR_FILENO);\
    }\
}

#define CAPTURE_END() {\
    close(dev_null);\
    if (loglevel == -1) {\
        dup2(org_stdout, STDOUT_FILENO);\
        dup2(org_stderr, STDERR_FILENO);\
    }\
}


#if !defined(PyModule_AddIntMacro) // New in verison 2.0
#define PyModule_AddIntMacro(module, name)      PyModule_AddIntConstant(module, #name, name);
#endif
#if !defined(PyModule_AddStringMacro) // New in version 2.6
#define PyModule_AddStringMacro(module, name)   PyModule_AddStringConstant(module, #name, name);
#endif

PyMethodDef mdbm_methods[] = {
    {"open", (PyCFunction)pymdbm_open, METH_VARARGS, 
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
    {"store", (PyCFunction)pymdbm_store, METH_VARARGS, 
        "store(key, val, [flags])"
        "Stores the record specified by the key and val parameters."
    },
    {"fetch", (PyCFunction)pymdbm_fetch, METH_VARARGS, 
        "fetch(key)"
        "Fetches the record specified by the key argument"
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

static PyObject *mdbm_getattr(MDBMObj *pmdbm_link, char *name) {
    return Py_FindMethod(mdbm_methods, (PyObject *)pmdbm_link, name);
}

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
    Py_TPFLAGS_DEFAULT,             /*tp_xxx4*/
};

#if PY_MAJOR_VERSION >= 3

PyModuleDef mdbm_mod = {
    PyModuleDef_HEAD_INIT,
    mdbmmod_name,
    mdbmmod_docs,
    -1,
    mdbm_methods,
    NULL,
    NULL,
    NULL,
    NULL
};



PyMODINIT_FUNC PyInit_mdbm(void) {
    return PyModule_Create(&mdbm_mod);
}

#else

PyMODINIT_FUNC initmdbm(void) {

    PyObject *m = NULL;
    PyObject *d = NULL;
    PyObject *s = NULL;

    m = Py_InitModule3(mdbmmod_name, mdbm_methods, mdbmmod_docs);
    if (m == NULL) {
        return;
    }

    if (MDBMError == NULL) {
        MDBMError = PyErr_NewException("mdbm.error", NULL, NULL);
    }

    d = PyModule_GetDict(m);

    s = PyString_FromString("The MDBM");
    if (s != NULL) {
        PyDict_SetItemString(d, "library", s);
        Py_DECREF(s);
    }

    if (MDBMError != NULL) {
        PyDict_SetItemString(d, "error", MDBMError);
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

}

#endif


// METHODS
PyObject *pymdbm_open(PyObject *self, PyObject *args) {

    const char *pfn = NULL;
    int flags = 0;
    int mode = 0;
    int psize = 0;
    int presize = 0;

    int rv = -1;

    rv = PyArg_ParseTuple(args, "sii|ii", &pfn, &flags, &mode, &psize, &presize);
    if (!rv) {
        PyErr_SetString(MDBMError, "required filepath and flags and mode");
        return NULL;
    }

    MDBMObj *pmdbm_link = NULL;

    pmdbm_link = PyObject_New(MDBMObj, &MDBMType);
    if (pmdbm_link == NULL) {
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

PyObject *pymdbm_close(register MDBMObj *pmdbm_link, PyObject *unused) {

    if ( pmdbm_link->pmdbm != NULL ) {
        mdbm_close(pmdbm_link->pmdbm);
        pmdbm_link->pmdbm = NULL;
    } else {
        PyErr_SetString(MDBMError, "failed to close an existing MDBM");
    }

    Py_INCREF(Py_None);
 
    return Py_None;
}

PyObject *pymdbm_close_fd(register MDBMObj *pmdbm_link, PyObject *unused) {

    if ( pmdbm_link->pmdbm != NULL ) {
        mdbm_close_fd(pmdbm_link->pmdbm);
        pmdbm_link->pmdbm = NULL;
    } else {
        PyErr_SetString(MDBMError, "failed to close an existing MDBM");
    }

    Py_INCREF(Py_None);
    return Py_None;
}



PyObject *pymdbm_sync(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_sync(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {

        Py_INCREF(Py_False);
        return Py_False;
    }
 
    Py_INCREF(Py_True);
    return Py_True;
}


PyObject *pymdbm_fsync(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_fsync(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {

        Py_INCREF(Py_False);
        return Py_False;
    }
 
    Py_INCREF(Py_True);
    return Py_True;
}

PyObject *pymdbm_preload(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_preload(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {

        Py_INCREF(Py_False);
        return Py_False;
    }
 
    Py_INCREF(Py_True);
    return Py_True;
}

PyObject *pymdbm_compress_tree(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_compress_tree(pmdbm_link->pmdbm);
    CAPTURE_END();
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *pymdbm_truncate(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_truncate(pmdbm_link->pmdbm);
    CAPTURE_END();
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *pymdbm_purge(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_purge(pmdbm_link->pmdbm);
    CAPTURE_END();
    Py_INCREF(Py_None);
    return Py_None;
}

PyObject *pymdbm_store(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;
    char *pval = NULL;
    int flags = MDBM_INSERT;

    int rv = -1;
    datum key, val;

    rv = PyArg_ParseTuple(args, "ss|i", &pkey, &pval, &flags);
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

    if (rv == -1) {
        Py_INCREF(Py_False);
        return Py_False;
    }
 
    Py_INCREF(Py_True);
    return Py_True;
}

PyObject *pymdbm_fetch(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;

    int rv = -1;
    datum key, val;

    rv = PyArg_ParseTuple(args, "s", &pkey);
    if (!rv) {
        PyErr_SetString(MDBMError, "required key and value");
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    val = mdbm_fetch(pmdbm_link->pmdbm, key);
    CAPTURE_END();

    if (val.dptr == NULL) {
        Py_INCREF(Py_False);
        return Py_False;
    }
 
    return PyString_FromStringAndSize(val.dptr, val.dsize);
}


