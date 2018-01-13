// https://github.com/torden
#include <stdio.h>
#include <Python.h>
#include "libmdbm.h"
#include <mdbm.h>
#include <mdbm_log.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>

#ifndef __PYMDBM_HINC__
#define __PYMDBM_HINC__

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

#define MDBM_STAT_TYPE_FETCH    0
#define MDBM_STAT_TYPE_STORE    1
#define MDBM_STAT_TYPE_DELETE   2
#define MDBM_STAT_TYPE_MAX      MDBM_STAT_TYPE_DELETE

#define MDBM_PTYPE_FREE         0 // Page type free
#define MDBM_PTYPE_DATA         1 // Page type data
#define MDBM_PTYPE_DIR          2 // Page type directory
#define MDBM_PTYPE_LOB          3 // Page type large object

#define MDBM_LOG_TO_STDERR      0
#define MDBM_LOG_TO_FILE        1
#define MDBM_LOG_TO_SYSLOG      2

static int loglevel = -1;
/* -- conflict
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
*/
#define CAPTURE_START()
#define CAPTURE_END()

#if !defined(PyModule_AddIntMacro)
#define PyModule_AddIntMacro(module, name){ PyModule_AddIntConstant(module, #name, name); }
#endif

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


#endif // __PYMDBM_HINC__

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

static inline int pymdbm_iter_handler(register MDBMObj *pmdbm_link, MDBM_ITER **piter,  PyObject *previter) {

    PyObject *previter_pageno = NULL;
    PyObject *previter_next = NULL;

    MDBM_ITER_INIT(*piter);

    if (previter != NULL && PyDict_Size(previter) > 0 && PyDict_Check(previter) > 0) { //use the parameter

        previter_pageno = PyDict_GetItemString(previter, "m_pageno");
        if (previter_pageno == NULL) {
            PyErr_SetString(MDBMError, "Error - There was a missing parameter: iter must have a m_pageno field");
            return -1;
        }

        Py_DECREF(previter_pageno);

        previter_next = PyDict_GetItemString(previter, "m_next");
        if (previter_next == NULL) {
            PyErr_SetString(MDBMError, "Error - There was a missing parameter: iter must have a m_next field");
            return -1;
        }

        Py_DECREF(previter_next);

        (*piter)->m_pageno = (mdbm_ubig_t) PyLong_AsLong(previter_pageno);
        (*piter)->m_next = (int) PyLong_AsLong(previter_next);
        return 0;

    } else if (previter == NULL) { //use the global

        (*piter) = &(*pmdbm_link).iter;
        return 0;
    }

    return -2;
}

static inline PyObject *get_iter_dict(MDBM_ITER *arg_iter) {

    int rv = -1;
    PyObject *pretdic = NULL;
    PyObject *pretiter = NULL;

    pretiter = PyDict_New();
    rv = PyDict_SetItemString(pretiter, "m_pageno", Py_BuildValue("i", arg_iter->m_pageno));
    if (rv == -1) {
        Py_DECREF(pretiter);
        PyErr_Format(PyExc_IOError, "mdbm::iter does not make a return value (m_pageno)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretiter, "m_next", Py_BuildValue("i", arg_iter->m_next));
    if (rv == -1) {
        Py_DECREF(pretiter);
        PyErr_Format(PyExc_IOError, "mdbm::iter does not make a return value (m_next)");
        return NULL;
    }

    pretdic = PyDict_New();
    rv = PyDict_SetItemString(pretdic, "iter",  pretiter);
    if (rv == -1) {
        Py_DECREF(pretiter);
        Py_DECREF(pretdic);
        PyErr_Format(PyExc_IOError, "mdbm::fetch_r does not make a return value (iter)");
        _RETURN_FALSE();
    }

    Py_DECREF(pretiter);
    Py_INCREF(pretdic);

    return pretdic;
}

static inline PyObject *get_db_info_dict(mdbm_db_info_t *info) {

    int rv = -1;
    PyObject *pretdbinfo = NULL;

    pretdbinfo = PyDict_New();

    rv = PyDict_SetItemString(pretdbinfo, "db_page_size", Py_BuildValue("i", info->db_page_size));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_page_size)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_num_pages", Py_BuildValue("i", info->db_num_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_num_pages)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_max_pages", Py_BuildValue("i", info->db_max_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_max_pages)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_num_dir_pages", Py_BuildValue("i", info->db_num_dir_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_num_dir_pages)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_dir_width", Py_BuildValue("i", info->db_dir_width));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_dir_width)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_max_dir_shift", Py_BuildValue("i", info->db_max_dir_shift));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_max_dir_shift)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_dir_min_level", Py_BuildValue("i", info->db_dir_min_level));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_dir_min_level)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_dir_max_level", Py_BuildValue("i", info->db_dir_max_level));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_dir_max_level)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_dir_num_nodes", Py_BuildValue("i", info->db_dir_num_nodes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_dir_num_nodes)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_hash_func", Py_BuildValue("i", info->db_hash_func));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_hash_func)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_hash_funcname", Py_BuildValue("i", info->db_hash_funcname));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_hash_funcname)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_spill_size", Py_BuildValue("i", info->db_spill_size));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_spill_size)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretdbinfo, "db_cache_mode", Py_BuildValue("i", info->db_cache_mode));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_info() does not make a return value(dbinfo->db_cache_mode)");
        return NULL;
    }

    return pretdbinfo;
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
        MDBMError = PyErr_NewException("mdbm.Error", PyExc_Exception, NULL);
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

    PyModule_AddStringConstant(m, "__version__", PYMDBM_VERSION);
    PyModule_AddIntConstant(m, "__mdbm_api_version__", MDBM_API_VERSION);

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
    PyModule_AddIntMacro(m, _MDBM_MAGIC);
    PyModule_AddIntMacro(m, _MDBM_MAGIC_NEW);
    PyModule_AddIntMacro(m, _MDBM_MAGIC_NEW2);
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

    PyModule_AddIntMacro(m, MDBM_STAT_TYPE_FETCH);
    PyModule_AddIntMacro(m, MDBM_STAT_TYPE_STORE);
    PyModule_AddIntMacro(m, MDBM_STAT_TYPE_DELETE);
    PyModule_AddIntMacro(m, MDBM_STAT_TYPE_MAX);

    PyModule_AddIntMacro(m, MDBM_PTYPE_FREE);
    PyModule_AddIntMacro(m, MDBM_PTYPE_DATA);
    PyModule_AddIntMacro(m, MDBM_PTYPE_DIR);
    PyModule_AddIntMacro(m, MDBM_PTYPE_LOB);

    PyModule_AddIntMacro(m, MDBM_LOG_TO_STDERR);
    PyModule_AddIntMacro(m, MDBM_LOG_TO_FILE);
    PyModule_AddIntMacro(m, MDBM_LOG_TO_SYSLOG);

#if PY_MAJOR_VERSION >= 3
    return m;
#endif

}


// METHODS
PyObject *pymdbm_init_iter(register MDBMObj *pmdbm_link, PyObject *unsed) {

    int rv = -1;
    PyObject *pretiter = NULL;
    MDBM_ITER iter;


    MDBM_ITER_INIT(&iter);

    pretiter = PyDict_New();
    rv = PyDict_SetItemString(pretiter, "m_pageno", Py_BuildValue("i", iter.m_pageno));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_r does not make a return value (iter.m_pageno)");
        _RETURN_FALSE();
    }
    rv = PyDict_SetItemString(pretiter, "m_next", Py_BuildValue("i", iter.m_next));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_r does not make a return value (iter.m_next)");
        _RETURN_FALSE();
    }

    Py_INCREF(pretiter);
    return pretiter;
}

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
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: filepath and flags and mode");
        return NULL;
    }

    MDBMObj *pmdbm_link = NULL;

    pmdbm_link = PyObject_New(MDBMObj, &MDBMType);
    if (pmdbm_link == NULL) {
        PyErr_SetString(MDBMError, "Error - failed to create a mdbm link");
        return NULL;
    }

    //protect : sigfault
    if (flags == (flags | MDBM_O_CREAT) && flags == (flags | MDBM_PROTECT)) {
        Py_DECREF(pmdbm_link);
        PyErr_SetString(MDBMError, "Error - failed to open the MDBM, not support create flags with MDBM_PROTECT");
        return NULL;
    }

    if (flags == (flags | MDBM_O_ASYNC) && flags == (flags | MDBM_O_FSYNC)) {
        Py_DECREF(pmdbm_link);
        PyErr_SetString(MDBMError, "Error - failed to open the MDBM, not support mixed sync flags (MDBM_O_FSYNC, MDBM_O_ASYNC)");
        return NULL;
    }

    if (flags == (flags | MDBM_O_RDONLY) && flags == (flags | MDBM_O_WRONLY)) {
        Py_DECREF(pmdbm_link);
        PyErr_SetString(MDBMError, "Error - failed to open the MDBM, not support mixed access flags (MDBM_O_RDONLY, MDBM_O_WRONLY, MDBM_O_RDWR)");
        return NULL;
    }

    mdbm_log_minlevel(-1);
    MDBM_ITER_INIT(&(*pmdbm_link).iter);

    CAPTURE_START();
    pmdbm_link->pmdbm = mdbm_open(pfn, flags, mode, psize, presize);
    CAPTURE_END();
    if (pmdbm_link->pmdbm == NULL) {
        PyErr_SetFromErrno(MDBMError);
        Py_DECREF(pmdbm_link);
        return NULL;
    }

    return (PyObject *)pmdbm_link;
}

PyObject *pymdbm_dup_handle(register MDBMObj *pmdbm_link, PyObject *args) {

    MDBMObj *pmdbm_dup_link = NULL;
    pmdbm_dup_link = PyObject_New(MDBMObj, &MDBMType);
    if (pmdbm_dup_link == NULL) {
        return NULL;
    }

    CAPTURE_START();
    pmdbm_dup_link->pmdbm = mdbm_dup_handle(pmdbm_link->pmdbm, 0); //flags Reserved for future use
    CAPTURE_END();

    if (pmdbm_dup_link->pmdbm == NULL) {
        PyErr_Format(MDBMError, "Error - failed to returns the duplicated an existing database handle (errno=%d, errmsg=%s)", errno, strerror(errno));
        Py_DECREF(pmdbm_dup_link);
        return NULL;
    }

    Py_INCREF(pmdbm_dup_link);
    return (PyObject *)pmdbm_dup_link;
}

PyObject *pymdbm_log_minlevel(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    int level = -1;

    rv = PyArg_ParseTuple(args, "i", &level);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: string(key)");
        return NULL;
    }

    if (level < MDBM_LOG_OFF || level > MDBM_LOG_DEBUG) {
        PyErr_Format(MDBMError, "Error - mdbm::log_minlevel does not support level(=%d)", level);
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
        PyErr_SetString(MDBMError, "Error - failed to close an existing MDBM");
    }

    _RETURN_NONE();
}

PyObject *pymdbm_close_fd(register MDBMObj *pmdbm_link, PyObject *unused) {

    if ( pmdbm_link->pmdbm != NULL ) {
        mdbm_close_fd(pmdbm_link->pmdbm);
        pmdbm_link->pmdbm = NULL;
    } else {
        PyErr_SetString(MDBMError, "Error - failed to close an existing MDBM");
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

PyObject *pymdbm_set_cachemode(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    int cachemode = -1;

    rv = PyArg_ParseTuple(args, "i", &cachemode);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: int(cachemode)");
        _RETURN_FALSE();
    }

    if (cachemode < MDBM_CACHEMODE_NONE || cachemode > MDBM_CACHEMODE_MAX ) {
        PyErr_Format(MDBMError, "Error - mdbm::set_hash does not support cachemode(=%d)", cachemode);
        _RETURN_FALSE();
    }

    CAPTURE_START();
    rv = mdbm_set_cachemode(pmdbm_link->pmdbm, cachemode);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::set_cachemode() does not set cachemode");
        _RETURN_FALSE();
    }
 
    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_get_cachemode(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;

    CAPTURE_START();
    rv = mdbm_get_cachemode(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::get_cachemode() does not obtain the cachemode of current MDBM");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_cachemode_name(register MDBMObj *unused, PyObject *args) {

    int rv = -1;
    int cachemode = -1;
    const char *pcachename = NULL;
    char *pretval = NULL;
    int retval_len = 0;

    rv = PyArg_ParseTuple(args, "i", &cachemode);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: int(cachemode)");
        _RETURN_FALSE();
    }

    CAPTURE_START();
    pcachename = mdbm_get_cachemode_name(cachemode);
    CAPTURE_END();

    if (pcachename == NULL) {
        PyErr_Format(MDBMError, "Error - mdbm::get_cachemode_name() does not set name of cachemode(=%d)", cachemode);
        _RETURN_FALSE();
    }

    retval_len = (int)strlen(pcachename);
    pretval = copy_strptr((char *)pcachename, retval_len);
    if (pretval == NULL) {
        _RETURN_FALSE();
    }

    return _PYUNICODE_ANDSIZE(pretval, retval_len);
}

PyObject *pymdbm_store(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pkey = NULL;
    char *pval = NULL;
    int flags = MDBM_INSERT;

    int rv = -1;
    datum key = {0x00,};
    datum val = {0x00,};

    static char *pkwlist[] = {"key", "val", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "ss|i", pkwlist, &pkey, &pval, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: key and value");
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

    if (rv == MDBM_STORE_ENTRY_EXISTS  && flags == (flags | MDBM_INSERT)) { //the key already exists
        PyErr_Format(MDBMError, "Error - the key(=%s) already exists", pkey);
        return NULL;
    }

    if (rv != MDBM_STORE_SUCCESS) {
        _RETURN_FALSE();
    }


    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_store_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pkey = NULL;
    char *pval = NULL;
    int flags = MDBM_INSERT;
    PyObject *previter = NULL;
    PyObject *pretdic = NULL;

    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;

    int rv = -1;
    datum key = {0x00,};
    datum val = {0x00,};

    static char *pkwlist[] = {"key", "val", "flags", "iter", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "ss|iO", pkwlist, &pkey, &pval, &flags, &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: key and value");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv < 0 && rv != -2) { //optional previter
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);
    val.dptr = pval;
    val.dsize = (int)strlen(pval);

    CAPTURE_START();
    rv = mdbm_store_r(pmdbm_link->pmdbm, &key, &val, (int)flags, parg_iter);
    CAPTURE_END();

    if (rv == MDBM_STORE_ENTRY_EXISTS  && flags == (flags | MDBM_INSERT)) { //the key already exists
        PyErr_Format(MDBMError, "Error - the key(=%s) already exists", pkey);
        return NULL;
    }

    if (rv != MDBM_STORE_SUCCESS) {
        _RETURN_FALSE();
    }

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    Py_INCREF(pretdic);
    return pretdic;
}


PyObject *pymdbm_fetch(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;

    int rv = -1;
    datum key = {0x00,};
    datum val = {0x00,};

    rv = PyArg_ParseTuple(args, "s", &pkey);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: string(key)");
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
}


PyObject *pymdbm_fetch_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pkey = NULL;

    int rv = -1;
    datum key = {0x00,};
    datum val = {0x00,};

    PyObject *previter = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;

    PyObject *pretdic = NULL;

    static char *pkwlist[] = {"key", "iter", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|O", pkwlist, &pkey, &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv < 0 && rv != -2) { //optional previter
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_fetch_r(pmdbm_link->pmdbm, &key, &val, parg_iter);
    CAPTURE_END();

    if (rv == -1) {
        _RETURN_FALSE();
    }

    if (val.dptr == NULL) {
        _RETURN_FALSE();
    }

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "val", _PYUNICODE_ANDSIZE(val.dptr, val.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_r does not make a return value (val)");
        _RETURN_FALSE();
    }

    Py_INCREF(pretdic);

    return pretdic;
}

PyObject *pymdbm_fetch_dup_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pkey = NULL;

    int rv = -1;
    datum key = {0x00,};
    datum val = {0x00,};
    PyObject *previter = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;

    PyObject *pretdic = NULL;

    static char *pkwlist[] = {"key", "iter", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|O", pkwlist, &pkey, &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key) and dict(iter{m_pageno,mnext})");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv < 0 && rv != -2) { //optional previter
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_fetch_dup_r(pmdbm_link->pmdbm, &key, &val, parg_iter);
    CAPTURE_END();

    if (rv == -1) {
        _RETURN_FALSE();
    }

    if (val.dptr == NULL) {
        _RETURN_FALSE();
    }

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "val", _PYUNICODE_ANDSIZE(val.dptr, val.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_dup_r does not make a return value (val)");
        _RETURN_FALSE();
    }

    Py_INCREF(pretdic);

    return pretdic;
}

PyObject *pymdbm_fetch_info(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pkey = NULL;

    int rv = -1;
    char locbuf[8192] = {0x00,};
    datum key = {0x00,};
    datum val = {0x00,};
    datum buf = {0x00,};

    struct mdbm_fetch_info info = {0x00,};
    PyObject *previter = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;

    PyObject *pretdic = NULL;
    PyObject *pretinfo = NULL;

    PyObject *x_flags, *x_cna, *x_cat;

    static char *pkwlist[] = {"key", "iter", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|O", pkwlist, &pkey, &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key) and dict(iter{m_pageno,mnext})");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter, previter);
    if (rv < 0 && rv != -2) { //optional previter
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    //init.
    val.dptr = 0;
    val.dsize = 0;

    //not use
    //if not set, see the realloc(): invalid pointer: 0x00007feddb90c7d8 
    buf.dptr = locbuf;
    buf.dsize = 8192; //small object

    CAPTURE_START();
    rv = mdbm_fetch_info(pmdbm_link->pmdbm, &key, &val, &buf, &info, parg_iter);
    CAPTURE_END();

    if (rv == -1) {
        _RETURN_FALSE();
    }

    if (val.dptr == NULL) {
        _RETURN_FALSE();
    }

    //make a fetch_info

    x_flags = PyLong_FromLong(info.flags);
    if (x_flags == NULL) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (info.flags)");
        Py_DECREF(x_flags);
        return NULL;
    }

    x_cna = PyLong_FromLong(info.cache_num_accesses);
    if (x_cna == NULL) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (info.cache_num_accesses)");
        Py_DECREF(x_cna);
        return NULL;
    }

    x_cat = PyLong_FromLong(info.cache_num_accesses);
    if (x_cat == NULL) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (info.cache_access_time)");
        Py_DECREF(x_cat);
        return NULL;
    }

    Py_INCREF(x_flags);
    Py_INCREF(x_cna);
    Py_INCREF(x_cat);

    pretinfo = PyDict_New();
    rv = PyDict_SetItemString(pretinfo, "flags", x_flags);
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (info{flags})");
        Py_DECREF(pretinfo);
        return NULL;
    }

    rv = PyDict_SetItemString(pretinfo, "cache_num_accesses", x_cna);
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (info{cache_num_accesses})");
        Py_DECREF(pretinfo);
        return NULL;
    }

    rv = PyDict_SetItemString(pretinfo, "cache_access_time", x_cat);
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (info{cache_access_time})");
        Py_DECREF(pretinfo);
        return NULL;
    }

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "val", _PYUNICODE_ANDSIZE(val.dptr, val.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (val)");
        Py_DECREF(pretdic);
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "info", pretinfo);
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::fetch_info does not make a return value (val)");
        Py_DECREF(pretdic);
        _RETURN_FALSE();
    }

    Py_DECREF(pretinfo);
    Py_INCREF(pretdic);

    return pretdic;
}


PyObject *pymdbm_get_magic_number(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    uint32_t magic = 0;

    CAPTURE_START();
    rv = mdbm_get_magic_number(pmdbm_link->pmdbm, &magic);
    CAPTURE_END();

    switch(rv) {

    case -3:
        PyErr_SetString(MDBMError, "Error - mdbm::get_magic_number() cannot read all of the magic number");
        _RETURN_FALSE();
        break;
 
    case -2:
        PyErr_SetString(MDBMError, "Error - mdbm::get_magic_number() cannot read data, db is truncated (empty)");
        _RETURN_FALSE();
        break;

    case -1:
        PyErr_SetString(MDBMError, "Error - mdbm::get_magic_number() cannot read db");
        _RETURN_FALSE();
        break;

    default:
        break;
    }

    return Py_BuildValue("l", magic);
}


PyObject *pymdbm_get_page(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;
    mdbm_ubig_t rv = -1;
    datum key = {0x00,};

    rv = PyArg_ParseTuple(args, "s", &pkey);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: string(key)");
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
 
    return Py_BuildValue("l", rv);
}

PyObject *pymdbm_delete(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pkey = NULL;

    int rv = -1;
    datum key = {0x00,};

    rv = PyArg_ParseTuple(args, "s", &pkey);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: string(key)");
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

PyObject *pymdbm_delete_r(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    PyObject *previter = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;

    rv = PyArg_ParseTuple(args, "O", &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: dic(iter{m_pageno,m_next})");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv == -2) {
        PyErr_SetString(MDBMError, "Error - failed to read to dic(iter{m_pageno,m_next})");
        return NULL;
    }

    if (rv < 0) {
        return NULL;
    }


    CAPTURE_START();
    rv = mdbm_delete_r(pmdbm_link->pmdbm, parg_iter);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_get_hash(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_get_hash(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::get_hash() does not obtain the current MDBM's hash func code");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_set_hash(register MDBMObj *pmdbm_link, PyObject *args) {

    int hash = -1; 
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &hash);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: key and value");
        _RETURN_FALSE();
    }
    if (hash < MDBM_HASH_CRC32 || hash > MDBM_MAX_HASH) {
        PyErr_Format(MDBMError, "Error - mdbm::set_hash does not support hash(=%d)", hash);
        _RETURN_FALSE();
    }

    CAPTURE_START();
    rv = mdbm_set_hash(pmdbm_link->pmdbm, hash);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_setspillsize(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    int size = -1;

    rv = PyArg_ParseTuple(args, "i", &size);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: size");
        _RETURN_FALSE();
    }
    if (size < 0) {
        PyErr_Format(MDBMError, "Error - mdbm::setspillsize does not support size(=%d)", size);
        _RETURN_FALSE();
    }

    CAPTURE_START();
    rv = mdbm_setspillsize(pmdbm_link->pmdbm, size);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::setspillsize() does not set the size of item data value which will be put on the large-object heap rather then inline");
        _RETURN_FALSE();
    }
 
    _RETURN_RV_BOOLEN(rv);
}


PyObject *pymdbm_get_alignment(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_get_alignment(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::get_alignment() does not obtain the MDBM's record byte-alignment.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_set_alignment(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    int align = -1;

    rv = PyArg_ParseTuple(args, "i", &align);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: alignment(align)");
        _RETURN_FALSE();
    }
    if (align < MDBM_ALIGN_8_BITS || align > MDBM_ALIGN_64_BITS) {
        PyErr_Format(MDBMError, "Error - mdbm::set_alignment does not support align(=%d)", align);
        _RETURN_FALSE();
    }


    CAPTURE_START();
    rv = mdbm_set_alignment(pmdbm_link->pmdbm, align);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::set_alignment() does not set the MDBM's record byte-alignment.");
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

PyObject *pymdbm_limit_dir_size(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    int pages = -1;

    rv = PyArg_ParseTuple(args, "i", &pages);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: pages");
        _RETURN_FALSE();
    }

    CAPTURE_START();
    rv = mdbm_limit_dir_size(pmdbm_link->pmdbm, pages);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::limit_dir_size() does not set limits the internal page directory size to a number of pages");
        _RETURN_FALSE();
    }
 
    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_get_version(register MDBMObj *pmdbm_link, PyObject *unused) {

    uint32_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_version(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == 0) {
        PyErr_SetString(MDBMError, "Error - mdbm::get_version() does not obtain the on-disk format version number of an MDBM.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("l", rv);
}

PyObject *pymdbm_get_size(register MDBMObj *pmdbm_link, PyObject *unused) {

    uint64_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_size(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == 0) {
        PyErr_SetString(MDBMError, "Error - mdbm::get_size() does not obtain the current MDBM's size");
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
        PyErr_SetString(MDBMError, "Error - mdbm::get_page_size() does not obtain the current MDBM's size");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_lockmode(register MDBMObj *pmdbm_link, PyObject *unused) {

    uint32_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_lockmode(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == (uint32_t)-1) {
        PyErr_SetString(MDBMError, "Error - mdbm::get_lockmode() does not obtain the current MDBM's lock info.");
        _RETURN_FALSE();
    }
 
    return Py_BuildValue("l", rv);
}


PyObject *pymdbm_lock(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    CAPTURE_START();
    rv = mdbm_lock(pmdbm_link->pmdbm);
    CAPTURE_END();
    if (rv == -1) {
        PyErr_SetString(MDBMError, "Error - mdbm::lock() does not obtain the current MDBM's lock info.");
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
        PyErr_SetString(MDBMError, "Error - mdbm::unlock() does not obtain the current MDBM's lock info.");
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
        PyErr_SetString(MDBMError, "Error - mdbm::trylock() does not obtain the current MDBM's lock info.");
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
        PyErr_SetString(MDBMError, "Error - mdbm::lock_shared() does not obtain the current MDBM's lock info.");
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
        PyErr_SetString(MDBMError, "Error - mdbm::trylock_shared() does not obtain the current MDBM's lock info.");
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
        PyErr_SetString(MDBMError, "Error - mdbm::lock_pages() does not obtain the current MDBM's lock info.");
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
        PyErr_SetString(MDBMError, "Error - mdbm::unlock_pages() does not obtain the current MDBM's lock info.");
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
            PyErr_SetString(MDBMError, "Error - mdbm::islocked() does not obtain the current MDBM's lock.");
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
            PyErr_SetString(MDBMError, "Error - mdbm::islocked() does not obtain the current MDBM's lock owner.");
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
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: string(file_path)");
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
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: string(file_path)");
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

    datum key = {0x00,};
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

    datum key = {0x00,};
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

PyObject *pymdbm_first_r(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    PyObject *previter = NULL;
    PyObject *pretdic = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;
    kvpair kv;
    char *pretkey = NULL;
    char *pretval = NULL;

    rv = PyArg_ParseTuple(args, "|O", &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: dic(iter{m_pageno,m_next})");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv == -2) {
        PyErr_SetString(MDBMError, "Error - failed to read to dic(iter{m_pageno,m_next})");
        return NULL;
    }

    if (rv < 0) {
        return NULL;
    }

    CAPTURE_START();
    kv = mdbm_first_r(pmdbm_link->pmdbm, parg_iter);
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

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "key", _PYUNICODE_ANDSIZE(kv.key.dptr, kv.key.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::first_r does not make a return value (key)");
        _RETURN_FALSE();
    }


    rv = PyDict_SetItemString(pretdic, "val", _PYUNICODE_ANDSIZE(kv.val.dptr, kv.val.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::first_r does not make a return value (val)");
        _RETURN_FALSE();
    }

    Py_INCREF(pretdic);

    return pretdic;
}

PyObject *pymdbm_next_r(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    PyObject *previter = NULL;
    PyObject *pretdic = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;
    kvpair kv;
    char *pretkey = NULL;
    char *pretval = NULL;

    rv = PyArg_ParseTuple(args, "|O", &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: dic(iter{m_pageno,m_next})");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv == -2) {
        PyErr_SetString(MDBMError, "Error - failed to read to dic(iter{m_pageno,m_next})");
        return NULL;
    }

    if (rv < 0) {
        return NULL;
    }

    CAPTURE_START();
    kv = mdbm_next_r(pmdbm_link->pmdbm, parg_iter);
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

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "key", _PYUNICODE_ANDSIZE(kv.key.dptr, kv.key.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::next_r does not make a return value (key)");
        _RETURN_FALSE();
    }


    rv = PyDict_SetItemString(pretdic, "val", _PYUNICODE_ANDSIZE(kv.val.dptr, kv.val.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::next_r does not make a return value (val)");
        _RETURN_FALSE();
    }

    Py_INCREF(pretdic);

    return pretdic;
}

PyObject *pymdbm_firstkey_r(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    PyObject *previter = NULL;
    PyObject *pretdic = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;
    datum key = {0x00,};
    char *pretkey = NULL;

    rv = PyArg_ParseTuple(args, "|O", &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: dic(iter{m_pageno,m_next})");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv == -2) {
        PyErr_SetString(MDBMError, "Error - failed to read to dic(iter{m_pageno,m_next})");
        return NULL;
    }

    if (rv < 0) {
        return NULL;
    }

    CAPTURE_START();
    key = mdbm_firstkey_r(pmdbm_link->pmdbm, parg_iter);
    CAPTURE_END();

    if (key.dptr == NULL) {
        _RETURN_FALSE();
    }

    pretkey = copy_strptr(key.dptr, key.dsize);
    if (pretkey == NULL) {
        _RETURN_FALSE();
    }

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "key", _PYUNICODE_ANDSIZE(key.dptr, key.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::firstkey_r does not make a return value (key)");
        _RETURN_FALSE();
    }

    Py_INCREF(pretdic);
    return pretdic;
}

PyObject *pymdbm_nextkey_r(register MDBMObj *pmdbm_link, PyObject *args) {

    int rv = -1;
    PyObject *previter = NULL;
    PyObject *pretdic = NULL;
    MDBM_ITER arg_iter;
    MDBM_ITER *parg_iter = &arg_iter;
    datum key = {0x00,}; 
    char *pretkey = NULL;

    rv = PyArg_ParseTuple(args, "|O", &previter);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: dic(iter{m_pageno,m_next})");
        return NULL;
    }

    //pass to iter handler
    rv = pymdbm_iter_handler(pmdbm_link, &parg_iter,  previter);
    if (rv == -2) {
        PyErr_SetString(MDBMError, "Error - failed to read to dic(iter{m_pageno,m_next})");
        return NULL;
    }

    if (rv < 0) {
        return NULL;
    }

    CAPTURE_START();
    key = mdbm_nextkey_r(pmdbm_link->pmdbm, parg_iter);
    CAPTURE_END();

    if (key.dptr == NULL) {
        _RETURN_FALSE();
    }

    pretkey = copy_strptr(key.dptr, key.dsize);
    if (pretkey == NULL) {
        _RETURN_FALSE();
    }

    //make a return value include iter
    pretdic = get_iter_dict(parg_iter);
    if (pretdic == NULL) {
        _RETURN_FALSE();
    }

    rv = PyDict_SetItemString(pretdic, "key", _PYUNICODE_ANDSIZE(key.dptr, key.dsize));
    if (rv == -1) {
        PyErr_Format(PyExc_IOError, "mdbm::nextkey_r does not make a return value (key)");
        _RETURN_FALSE();
    }

    Py_INCREF(pretdic);
    return pretdic;
}

PyObject *pymdbm_clean(register MDBMObj *pmdbm_link, PyObject *args) {

    int pagenum = -1;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &pagenum);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: int(pagenum)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_clean(pmdbm_link->pmdbm, pagenum, 0); // flags Ignored
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_check(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    int level = -1;
    int verbose = -1;

    int rv = -1;

    static char *pkwlist[] = {"level", "verbose", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "i|i", pkwlist, &level, &verbose);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: level(10 >= level >= 0), [verbose(True | False)]");
        return NULL;
    }

    /*
    MDBM_CHECK_HEADER    //< Check MDBM header for integrity
    MDBM_CHECK_CHUNKS    //< Check MDBM header and chunks (page structure)
    MDBM_CHECK_DIRECTORY //< Check MDBM header, chunks, and directory
    MDBM_CHECK_ALL       //< Check MDBM header, chunks, directory, and data
    */
    if (level < 0 || level > 10) {
        PyErr_Format(MDBMError, "Error - mdbm::check does not support level(=%d)", level);
        return NULL;
    }

    if (verbose < 0 || verbose > 1) {
        PyErr_Format(MDBMError, "Error - mdbm::check does not support verbose(=%d)", verbose);
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
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: int(pagenum)");
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

    return Py_BuildValue("l", rv);
}

PyObject *pymdbm_get_errno(register MDBMObj *pmdbm_link, PyObject *unused) {

    mdbm_ubig_t rv = -1;
    CAPTURE_START();
    rv = mdbm_get_errno(pmdbm_link->pmdbm);
    CAPTURE_END();

    return Py_BuildValue("l", rv);
}

PyObject *pymdbm_plock(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pkey = NULL;
    int flags = 0;

    datum key = {0x00,};
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "Error - mdbm::plock does not support flags(=%d)", flags);
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

    datum key = {0x00,};
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags); 
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "Error - mdbm::punlock does not support flags(=%d)", flags);
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

    datum key ={0x00,};
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "Error - mdbm::tryplock does not support flags(=%d)", flags);
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

    datum key = {0x00,};
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "Error - mdbm::lock_smart does not support flags(=%d)", flags);
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

    datum key = {0x00,};
    int rv = -1;

    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags); 
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "Error - mdbm::trylock_smart does not support flags(=%d)", flags);
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

    datum key = {0x00,};
    int rv = -1;

    
    static char *pkwlist[] = {"key", "flags", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "s|i", pkwlist, &pkey, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    if (flags != 0 && flags != MDBM_PARTITIONED_LOCKS && flags != MDBM_RW_LOCKS && flags != MDBM_O_RDWR) {
        PyErr_Format(MDBMError, "Error - mdbm::unlock_smart does not support flags(=%d)", flags);
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
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key)");
        return NULL;
    }

    if (protect < MDBM_PROT_NONE || protect > MDBM_PROT_ACCESS) {
        PyErr_Format(MDBMError, "Error - mdbm::protect does not support protect(=%d)", protect);
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_protect(pmdbm_link->pmdbm, protect);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_replace_db(register MDBMObj *pmdbm_link, PyObject *args) {

    char *pnewfile = NULL;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "s", &pnewfile);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(new_file_path)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_replace_db(pmdbm_link->pmdbm, pnewfile);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_replace_file(register MDBMObj *unused, PyObject *args, PyObject *kwds) {

    char *poldfile = NULL;
    char *pnewfile = NULL;
    int rv = -1;

    static char *pkwlist[] = {"oldfile", "newfile", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "ss", pkwlist, &poldfile, &pnewfile);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(oldfile) and str(newfile)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_replace_file(poldfile, pnewfile);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_fcopy(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds) {

    char *pnewfile = NULL;
    int flags = 0;
    int rv = -1;
    int fd = -1;

    rv = PyArg_ParseTuple(args, "s|i", &pnewfile, &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(newfile)");
        return NULL;
    }

    fd = open(pnewfile, O_RDWR | O_CREAT | O_TRUNC, 0644);

    CAPTURE_START();
    rv = mdbm_fcopy(pmdbm_link->pmdbm, fd, flags);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_get_hash_value(register MDBMObj *unused, PyObject *args, PyObject *kwds) {

    int rv = -1;
    char *pkey = NULL;
    int hashfunc = -1;
    datum key = {0x00,};
    uint32_t hashval = -1;

    static char *pkwlist[] = {"key", "hashfunc", NULL};
    rv = PyArg_ParseTupleAndKeywords(args, kwds, "si", pkwlist, &pkey, &hashfunc);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: str(key) and mdbm.MDBM_HASH_XXX");
        return NULL;
    }

    if (hashfunc < MDBM_HASH_CRC32 || hashfunc > MDBM_MAX_HASH) {
        PyErr_Format(MDBMError, "Error - mdbm::get_hash_value does not support hashfunc(=%d)", hashfunc);
        return NULL;
    }

    //make a datum
    key.dptr = pkey;
    key.dsize = (int)strlen(pkey);

    CAPTURE_START();
    rv = mdbm_get_hash_value(key, hashfunc, &hashval);
    CAPTURE_END();

    if (rv == -1) {
        _RETURN_FALSE();
    }

    return Py_BuildValue("l", hashval);
}

PyObject *pymdbm_pre_split(register MDBMObj *pmdbm_link, PyObject *args) {

    mdbm_ubig_t split_size = 0;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &split_size);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: int(N)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_pre_split(pmdbm_link->pmdbm, split_size);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

PyObject *pymdbm_dump_all_page(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_dump_all_page(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_NONE();
}

PyObject *pymdbm_dump_page(register MDBMObj *pmdbm_link, PyObject *args) {

    int pagenum = 0;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &pagenum);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: int(pagenum)");
        return NULL;
    }

    CAPTURE_START();
    mdbm_dump_page(pmdbm_link->pmdbm, pagenum);
    CAPTURE_END();

    _RETURN_NONE();
}

PyObject *pymdbm_get_stats(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    mdbm_stats_t s = {0x00,};
    PyObject *pretstats = NULL;


    CAPTURE_START();
    rv = mdbm_get_stats(pmdbm_link->pmdbm, &s, sizeof(s));
    CAPTURE_END();

    if (rv == -1) {
        _RETURN_FALSE();
    }

    pretstats = PyDict_New();

    rv = PyDict_SetItemString(pretstats, "size", Py_BuildValue("i", s.s_size));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_size)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "num_entries", Py_BuildValue("i", s.s_num_entries));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_num_entries)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_num_entries", Py_BuildValue("i", s.s_large_num_entries));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_num_entries)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_num_free_entries", Py_BuildValue("i", s.s_large_num_free_entries));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_num_free_entries)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "cache_mode", Py_BuildValue("i", s.s_cache_mode));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_cache_mode)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "page_size", Py_BuildValue("i", s.s_page_size));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_page_size)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "pages_used", Py_BuildValue("i", s.s_pages_used));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_pages_used)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_pages_used", Py_BuildValue("i", s.s_large_pages_used));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_pages_used)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "page_count", Py_BuildValue("i", s.s_page_count));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_page_count)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_page_count", Py_BuildValue("i", s.s_large_page_count));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_page_count)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_page_size", Py_BuildValue("i", s.s_large_page_size));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_page_size)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_max_size", Py_BuildValue("i", s.s_large_max_size));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_max_size)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_min_size", Py_BuildValue("i", s.s_large_min_size));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_min_size)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_max_free", Py_BuildValue("i", s.s_large_max_free));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_max_free)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "min_level", Py_BuildValue("i", s.s_min_level));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_min_level)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "max_level", Py_BuildValue("i", s.s_max_level));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_max_level)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "bytes_used", Py_BuildValue("i", s.s_bytes_used));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_bytes_used)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_bytes_used", Py_BuildValue("i", s.s_large_bytes_used));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_bytes_used)");
        return NULL;
    }
    rv = PyDict_SetItemString(pretstats, "large_threshold", Py_BuildValue("i", s.s_large_threshold));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_stats() does not make a return value (stats.s.s_large_threshold)");
        return NULL;
    }

    Py_INCREF(pretstats);
    return pretstats;
}

PyObject *pymdbm_get_db_info(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    mdbm_db_info_t info = {0x00,};
    PyObject *pretdbinfo = NULL;

    CAPTURE_START();
    rv = mdbm_get_db_info(pmdbm_link->pmdbm, &info);
    CAPTURE_END();

    if (rv == -1) {
        _RETURN_FALSE();
    }

    pretdbinfo = get_db_info_dict(&info);

    Py_INCREF(pretdbinfo);
    return pretdbinfo;
}

PyObject *pymdbm_get_stat_counter(register MDBMObj *pmdbm_link, PyObject *args) {

    mdbm_stat_type type = 0;
    mdbm_counter_t value = 0;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &type);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: type");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_get_stat_counter(pmdbm_link->pmdbm, type, &value);
    CAPTURE_END();
    if (rv == -1) {
        _RETURN_FALSE();
    }

    return Py_BuildValue("i", value);
}

PyObject *pymdbm_get_stat_time(register MDBMObj *pmdbm_link, PyObject *args) {

    mdbm_stat_type type = 0;
    time_t value = 0;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &type);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: type");
        return NULL;
    }

    if (type != MDBM_STAT_TYPE_FETCH && type != MDBM_STAT_TYPE_STORE && type != MDBM_STAT_TYPE_DELETE) {
        PyErr_Format(MDBMError, "Error - mdbm::get_stat_time does not support type(=%d)", type);
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_get_stat_time(pmdbm_link->pmdbm, type, &value);
    CAPTURE_END();
    if (rv == -1) {
        _RETURN_FALSE();
    }

    return Py_BuildValue("s", ctime(&value));
}


PyObject *pymdbm_reset_stat_operations(register MDBMObj *pmdbm_link, PyObject *unused) {

    CAPTURE_START();
    mdbm_reset_stat_operations(pmdbm_link->pmdbm);
    CAPTURE_END();

    _RETURN_NONE();
}

PyObject *pymdbm_enable_stat_operations(register MDBMObj *pmdbm_link, PyObject *args) {

    int flags = 0;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: flags");
        return NULL;
    }

    if (flags != MDBM_STATS_BASIC && flags != MDBM_STATS_TIMED &&
        flags != (MDBM_STATS_BASIC | MDBM_STATS_TIMED) && flags != 0) {
        PyErr_Format(MDBMError, "Error - mdbm::enable_stat_operations does not support flags(=%d)", flags);
        return NULL;
    }


    CAPTURE_START();
    rv = mdbm_enable_stat_operations(pmdbm_link->pmdbm, flags);
    CAPTURE_END();
    if (rv == -1) {
        _RETURN_FALSE();
    }

    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_set_stat_time_func(register MDBMObj *pmdbm_link, PyObject *args) {

    int flags = 0;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "i", &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: flags");
        return NULL;
    }

    if (flags != MDBM_CLOCK_TSC && flags != MDBM_CLOCK_STANDARD) {
        PyErr_Format(MDBMError, "Error - mdbm::set_stat_time_func does not support flags(=%d)", flags);
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_set_stat_time_func(pmdbm_link->pmdbm, flags);
    CAPTURE_END();
    if (rv == -1) {
        _RETURN_FALSE();
    }

    return Py_BuildValue("i", rv);
}

PyObject *pymdbm_get_db_stats(register MDBMObj *pmdbm_link, PyObject *args) {

    int flags = 0;
    int rv = -1;
    int i = 0;
    mdbm_db_info_t info = {0x00,};
    mdbm_stat_info_t stats = {0x00,};

    int listlen = -1;

    PyObject *pretval = NULL;
    PyObject *pretdbinfo = NULL;
    PyObject *pretstatsinfo = NULL;
    PyObject *pretbucket = NULL;
    PyObject *ptempbucket = NULL;


    rv = PyArg_ParseTuple(args, "i", &flags);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: flags");
        return NULL;
    }

    if (flags != MDBM_STAT_NOLOCK && flags != MDBM_ITERATE_NOLOCK) {
        PyErr_Format(MDBMError, "Error - mdbm::get_db_stats does not support flags(=%d)", flags);
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_get_db_stats(pmdbm_link->pmdbm, &info, &stats, flags);
    CAPTURE_END();
    if (rv == -1) {
        _RETURN_FALSE();
    }

    listlen = sizeof(stats.buckets)/sizeof(stats.buckets[0]);

    //mdbm_db_info_t to PyDict
    pretdbinfo = get_db_info_dict(&info);

    //mdbm_stat_info_t to PyDict
    pretstatsinfo = PyDict_New();
    rv = PyDict_SetItemString(pretstatsinfo, "flags", Py_BuildValue("k", stats.flags));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.flags)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "num_active_entries", Py_BuildValue("k", stats.num_active_entries));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.num_active_entries)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "num_active_lob_entries", Py_BuildValue("k", stats.num_active_lob_entries));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.num_active_lob_entries)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "sum_key_bytes", Py_BuildValue("k", stats.sum_key_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.sum_key_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "sum_lob_val_bytes", Py_BuildValue("k", stats.sum_lob_val_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.sum_lob_val_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "sum_normal_val_bytes", Py_BuildValue("k", stats.sum_normal_val_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.sum_normal_val_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "sum_overhead_bytes", Py_BuildValue("k", stats.sum_overhead_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.sum_overhead_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "min_entry_bytes", Py_BuildValue("l", stats.min_entry_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.min_entry_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "max_entry_bytes", Py_BuildValue("l", stats.max_entry_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.max_entry_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "min_key_bytes", Py_BuildValue("l", stats.min_key_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.min_key_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "max_key_bytes", Py_BuildValue("l", stats.max_key_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.max_key_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "min_val_bytes", Py_BuildValue("l", stats.min_val_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.min_val_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "max_val_bytes", Py_BuildValue("l", stats.max_val_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.max_val_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "min_lob_bytes", Py_BuildValue("l", stats.min_lob_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.min_lob_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "max_lob_bytes", Py_BuildValue("l", stats.max_lob_bytes));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.max_lob_bytes)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "max_page_used_space", Py_BuildValue("l", stats.max_page_used_space));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.max_page_used_space)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "max_data_pages", Py_BuildValue("l", stats.max_data_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.max_data_pages)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "num_free_pages", Py_BuildValue("l", stats.num_free_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.num_free_pages)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "num_active_pages", Py_BuildValue("l", stats.num_active_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.num_active_pages)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "num_normal_pages", Py_BuildValue("l", stats.num_normal_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.num_normal_pages)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "num_oversized_pages", Py_BuildValue("l", stats.num_oversized_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.num_oversized_pages)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "num_lob_pages", Py_BuildValue("l", stats.num_lob_pages));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.num_lob_pages)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "max_page_entries", Py_BuildValue("l", stats.max_page_entries));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.max_page_entries)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstatsinfo, "min_page_entries", Py_BuildValue("l", stats.min_page_entries));
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.min_page_entries)");
        return NULL;
    }

    pretbucket = PyList_New(0);
    for (i=0; i<listlen; i++) {

        ptempbucket = PyDict_New();

        rv = PyDict_SetItemString(ptempbucket, "num_pages", Py_BuildValue("l", stats.buckets[i].num_pages));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].num_pages)", i);
            return NULL;
        }

        rv = PyDict_SetItemString(ptempbucket, "min_bytes", Py_BuildValue("l", stats.buckets[i].min_bytes));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].min_bytes)", i);
            return NULL;
        }

        rv = PyDict_SetItemString(ptempbucket, "max_bytes", Py_BuildValue("l", stats.buckets[i].max_bytes));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].max_bytes)", i);
            return NULL;
        }

        rv = PyDict_SetItemString(ptempbucket, "min_free_bytes", Py_BuildValue("l", stats.buckets[i].min_free_bytes));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].min_free_bytes)", i);
            return NULL;
        }

        rv = PyDict_SetItemString(ptempbucket, "max_free_bytes", Py_BuildValue("l", stats.buckets[i].max_free_bytes));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].max_free_bytes)", i);
            return NULL;
        }

        rv = PyDict_SetItemString(ptempbucket, "sum_entries", Py_BuildValue("k", stats.buckets[i].sum_entries));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].sum_entries)", i);
            return NULL;
        }

        rv = PyDict_SetItemString(ptempbucket, "sum_bytes", Py_BuildValue("k", stats.buckets[i].sum_bytes));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].sum_bytes)", i);
            return NULL;
        }

        rv = PyDict_SetItemString(ptempbucket, "sum_free_bytes", Py_BuildValue("k", stats.buckets[i].sum_free_bytes));
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d].sum_free_bytes)", i);
            return NULL;
        }

        rv = PyList_Append(pretbucket, ptempbucket);
        if (rv == -1) {
            Py_DECREF(pretdbinfo);
            Py_DECREF(pretstatsinfo);
            Py_DECREF(ptempbucket);
            Py_DECREF(pretbucket);
            PyErr_Format(PyExc_IOError, "mdbm::get_db_stat() doest not make a resturn value(stats.buckets[%d])", i);
            return NULL;
        }

        Py_DECREF(ptempbucket);
    }

    // list of bucket into the statinfo
    rv = PyDict_SetItemString(pretstatsinfo, "buckets", pretbucket);
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        Py_DECREF(pretbucket);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(stats.buckets)");
        return NULL;
    }

    // make a return value.
    pretval = PyDict_New();
    rv = PyDict_SetItemString(pretval, "db", pretdbinfo);
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        Py_DECREF(pretbucket);
        Py_DECREF(pretval);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(returnValue.db)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretval, "stats", pretstatsinfo);
    if (rv == -1) {
        Py_DECREF(pretdbinfo);
        Py_DECREF(pretstatsinfo);
        Py_DECREF(pretbucket);
        Py_DECREF(pretval);
        PyErr_Format(PyExc_IOError, "mdbm::get_db_stats() does not make a return value(returnValue.stats)");
        return NULL;
    }

    Py_DECREF(pretdbinfo);
    Py_DECREF(pretstatsinfo);
    Py_DECREF(pretbucket);

    Py_INCREF(pretval);
    return pretval;
}

PyObject *pymdbm_get_window_stats(register MDBMObj *pmdbm_link, PyObject *unused) {

    int rv = -1;
    mdbm_window_stats_t stats = {0x00,};
    PyObject *pretstats = NULL;

    CAPTURE_START();
    rv = mdbm_get_window_stats(pmdbm_link->pmdbm, &stats, sizeof(stats));
    CAPTURE_END();
    if (rv == -1) {
        _RETURN_FALSE();
    }

    //mdbm_window_stats_t to PyDict
    pretstats = PyDict_New();
    rv = PyDict_SetItemString(pretstats, "num_reused", Py_BuildValue("k", stats.w_num_reused));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_window_stats() does not make a return value(windows_stats.num_reused)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstats, "num_remapped", Py_BuildValue("k", stats.w_num_remapped));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_window_stats() does not make a return value(windows_stats.num_remapped)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstats, "window_size", Py_BuildValue("l", stats.w_window_size));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_window_stats() does not make a return value(windows_stats.window_size)");
        return NULL;
    }

    rv = PyDict_SetItemString(pretstats, "max_window_used", Py_BuildValue("l", stats.w_max_window_used));
    if (rv == -1) {
        Py_DECREF(pretstats);
        PyErr_Format(PyExc_IOError, "mdbm::get_window_stats() does not make a return value(windows_stats.max_window_used)");
        return NULL;
    }

    Py_INCREF(pretstats);
    return pretstats;
}

PyObject *pymdbm_select_log_plugin(register MDBMObj *unused, PyObject *args) {

    int type = 0;
    int rv = -1;
	char buf[10] = {0x00,};

    rv = PyArg_ParseTuple(args, "i", &type);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: int(type)");
        return NULL;
    }

	if (type < MDBM_LOG_TO_STDERR || type > MDBM_LOG_TO_SYSLOG) {
        PyErr_Format(MDBMError, "Error - mdbm::select_log_plugin does not support type(=%d)", type);
        return NULL;
    }

	switch (type) {

		case MDBM_LOG_TO_STDERR:
			strncpy(buf, "stderr", 9);
			break;
		case MDBM_LOG_TO_FILE:
			strncpy(buf, "file", 9);
			break;

		case MDBM_LOG_TO_SYSLOG:
			strncpy(buf, "syslog", 9);
			break;

		default:
        	PyErr_Format(MDBMError, "Bug - Failed to run to mdbm::select_log_plugin");
        return NULL;

	}

    CAPTURE_START();
    rv = mdbm_select_log_plugin((const char *)&buf);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}


PyObject *pymdbm_set_log_filename(register MDBMObj *unused, PyObject *args) {

    char *plogfile = NULL;
    int rv = -1;

    rv = PyArg_ParseTuple(args, "s", &plogfile);
    if (!rv) {
        PyErr_SetString(MDBMError, "Error - There was a missing parameter: logfile)");
        return NULL;
    }

    CAPTURE_START();
    rv = mdbm_set_log_filename((const char *)plogfile);
    CAPTURE_END();

    _RETURN_RV_BOOLEN(rv);
}

// -------------------------------------------------------------------



#if PY_MAJOR_VERSION >= 3
// - for context manager
PyObject *pymdbm__enter(register MDBMObj *pmdbm_link, PyObject *unused) {
    Py_INCREF(pmdbm_link);
    return (PyObject*)pmdbm_link;
}

PyObject *pymdbm__exit(register MDBMObj *pmdbm_link, PyObject *args) {

    PyObject* exc_type, *exc_value, *exc_tb;

     if (!PyArg_ParseTuple(args, "OOO", &exc_type, &exc_value, &exc_tb)) {
        _RETURN_NONE();
    }

    if (exc_type == Py_None && exc_value == Py_None && exc_tb == Py_None) {
        PyObject_CallMethod((PyObject*)pmdbm_link, (char *)"sync", "");
        PyObject_CallMethod((PyObject*)pmdbm_link, (char *)"close", "");
    } else {
        // - debug
        //PyErr_Format(MDBMError, "Error - mdbm::__exit__ does not work");
        _RETURN_NONE();
    }

    Py_INCREF(Py_False);
    return Py_False;
}

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
