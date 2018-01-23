#ifndef __PYMDBM_H__
#define __PYMDBM_H__

#include <Python.h>
#include <mdbm.h>
#include <mdbm_log.h>

#define PYMDBM_VERSION "0.1.0"

typedef struct {
    PyObject_HEAD
    MDBM *pmdbm;
    MDBM_ITER iter;
} MDBMObj;

typedef struct {
    PyObject_HEAD
    MDBM_ITER *piter;
} MDBMIter;

PyObject *MDBMError = NULL;
PyObject *pymdbm_init_iter(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_open(PyObject *self, PyObject *args, PyObject *kwds);
PyObject *pymdbm_dup_handle(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_log_minlevel(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_close(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_close_fd(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_sync(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_fsync(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_preload(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_compress_tree(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_truncate(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_purge(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_set_cachemode(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_cachemode(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_cachemode_name(register MDBMObj *unused, PyObject *args);

PyObject *pymdbm_store(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_store_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_fetch(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_fetch_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_fetch_dup_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_fetch_info(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_delete(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_delete_r(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_get_magic_number(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_page(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_hash(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_set_hash(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_setspillsize(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_alignment(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_set_alignment(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_limit_size(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_limit_dir_size(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_version(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_size(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_page_size(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_lock(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_unlock(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_get_lockmode(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_trylock(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_lock_shared(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_trylock_shared(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_lock_pages(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_unlock_pages(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_plock(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_punlock(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_tryplock(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_lock_smart(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_trylock_smart(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_unlock_smart(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);

PyObject *pymdbm_islocked(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_isowned(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_lock_reset(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_delete_lockfiles(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_first(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_next(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_firstkey(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_nextkey(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_first_r(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_next_r(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_firstkey_r(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_nextkey_r(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_clean(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_check(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_chk_page(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_chk_all_page(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_count_records(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_count_pages(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_get_errno(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_protect(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_replace_db(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_replace_file(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_fcopy(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);

PyObject *pymdbm_get_hash_value(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_pre_split(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_dump_all_page(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_dump_page(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_get_stats(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_db_info(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_get_stat_counter(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_stat_time(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_reset_stat_operations(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_enable_stat_operations(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_set_stat_time_func(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_db_stats(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_window_stats(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_select_log_plugin(register MDBMObj *unused, PyObject *args);
PyObject *pymdbm_set_log_filename(register MDBMObj *unused, PyObject *args);


#if PY_MAJOR_VERSION >= 3
PyObject *pymdbm__enter(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm__exit(register MDBMObj *pmdbm_link, PyObject *args);
#endif

PyMethodDef mdbm_methods[] = {
#if PY_MAJOR_VERSION >= 3
    {"__enter__", (PyCFunction)pymdbm__enter, METH_NOARGS, 
        "__enter__"
            "For context manager."
    },
    {"__exit__", (PyCFunction)pymdbm__exit, METH_VARARGS, 
        "__exit__"
            "For context manager."
    },
#endif
    {"log_minlevel", (PyCFunction)pymdbm_log_minlevel, METH_VARARGS, 
        "log_minlevel(MDBM_LOG_XXX)"
            "Set the minimum logging level. Lower priority messages are discarded"
            "Default : MDBM_LOG_OFF"
    },
    {"init_iter", (PyCFunction)pymdbm_init_iter, METH_NOARGS, 
        "init_iter()"
            "Returns Initialized an MDBM iterator."
    },
    {"open", (PyCFunction)pymdbm_open, METH_VARARGS | METH_KEYWORDS, 
        "open(path, flags, mode, [psize, presize])"
            "Creates and/or opens a database."
            "PARAM :"
            "\t if psize not specified or 0, API sets the 4096"
            "\t if presize not specified, API will be increased to the next psize multiple."
    },
    {"dup_handle", (PyCFunction)pymdbm_dup_handle, METH_NOARGS,
        "dup_handle()"
            "Duplicate an existing database handle."
            "The advantage of dup'ing a handle over doing a separate open() "
            "is that dup's handle share the same virtual page mapping within the process space (saving memory)."
            "Threaded applications should use pthread_mutex_lock and unlock around calls to dup_handle()."
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

    {"set_cachemode", (PyCFunction)pymdbm_set_cachemode, METH_VARARGS, 
        "set_cachemode(cachemode)"
            "Manage the database as a cache."
            "set_cachemode() must be called before data is inserted."
            "Tracking metadata is stored with each entry which allows"
            "MDBM to do cache eviction via LRU, LFU, and GDSF (greedy-dual-size-frequency)."
            "MDBM also supports clean/dirty tracking and"
            "the application can supply a callback (see set_backingstore(NOT SUPPORT))"
            "which is called by MDBM when a dirty entry is about to be evicted allowing"
            "the application to sync the entry to a backing store or perform some other"
            "type of `clean` operation."
            ""
            "NOTE: V3 API"
            ""
            "Values for cachemode:"
            "\t- MDBM_CACHEMODE_NONE - no cache mode"
            "\t- MDBM_CACHEMODE_LFU  - least frequently used"
            "\t- MDBM_CACHEMODE_LRU  - least recently used"
            "\t- MDBM_CACHEMODE_GDSF - greedy dual-size frequency"
    },

    {"get_cachemode", (PyCFunction)pymdbm_get_cachemode, METH_NOARGS, 
        "get_cachemode()"
            "Returns the current cache style of the database."
            "See the cachemode parameter in set_cachemode() for the valid values."
            ""
            "NOTE: V3 API"
            ""
            "\t- MDBM_CACHEMODE_NONE - no cache mode"
            "\t- MDBM_CACHEMODE_LFU  - least frequently used"
            "\t- MDBM_CACHEMODE_LRU  - least recently used"
            "\t- MDBM_CACHEMODE_GDSF - greedy dual-size frequency"
    },

    {"get_cachemode_name", (PyCFunction)pymdbm_get_cachemode_name, METH_VARARGS, 
        "get_cachemode_name(cachemode)"
            "Returns the cache mode as a string. See set_cachemode()"
    },

    {"store", (PyCFunction)pymdbm_store, METH_VARARGS | METH_KEYWORDS, 
        "store(key, val, [flags])"
            "Stores the record specified by the key and val parameters."
            "PARAM :"
            "\t Values for flags mask:"
            "\t   - MDBM_INSERT - Operation will fail if a record with the same key"
            "\t     already exists in the database."
            "\t   - MDBM_REPLACE - A record with the same key will be replaced."
            "\t     If the key does not exist, the record will be created."
            "\t   - MDBM_INSERT_DUP - allows multiple records with the same key to be inserted."
            "\t     Fetching a record with the key will return only one of the duplicate"
            "\t     records, and which record is returned is not defined."
            "\t   - MDBM_MODIFY - Store only if matching entry already exists."
            "\t   - MDBM_RESERVE - Reserve space; value not copied (\ref mdbm_store_r only)"
            "\t   - MDBM_CACHE_ONLY  - Perform store only in the Cache, not in Backing Store."
            "\t   - MDBM_CACHE_MODIFY  - Update Cache only if key exists; update the Backing Store"
            "\t"
            "\t Insertion with flag MDBM_MODIFY set will fail if the key does not already exist."
    },
    {"store_r", (PyCFunction)pymdbm_store_r, METH_VARARGS | METH_KEYWORDS, 
        "store_r(key, val, [iter{m_pageno,m_next}], [flags])"
            "Stores the record specified by the key and val parameters."
            "PARAM : "
            "\tIf iter not specified, API uses the global iter"
            "\tIf flags not specified, API uses the MDBM_INSERT"
    },

    {"fetch", (PyCFunction)pymdbm_fetch, METH_VARARGS, 
        "fetch(key)"
            "Fetches the record specified by the key argument"
    },
    {"fetch_r", (PyCFunction)pymdbm_fetch_r, METH_VARARGS | METH_KEYWORDS, 
        "fetch(key, [iter{m_pageno,m_next}])"
            "A record can be updated in-place by fetching the value"
            "PARAM : "
            "\tIf iter not specified, API uses the global iter"
    },
    {"fetch_dup_r", (PyCFunction)pymdbm_fetch_dup_r, METH_VARARGS | METH_KEYWORDS, 
        "fetch_dup_r(key, [iter{m_pageno,m_next}])"
            "Fetches the next value for a key inserted via store() with the MDBM_INSERT_DUP flag set."
            "The order of values returned by iterating via this function is not guaranteed to be the same order as the values were inserted."
            ""
            "As with any db iteration, record insertion and deletion during iteration may cause the iteration to skip and/or repeat records."
            "Calling this function with an iterator initialized via init_iter() will cause this function to return the first value for the given key."
    },
    {"fetch_info", (PyCFunction)pymdbm_fetch_info, METH_VARARGS | METH_KEYWORDS, 
        "fetch_info(key, [iter{m_pageno,m_next}])"
            "Fetches and copies the record specified by the key argument."
            "If such a record exists in the database, the size and location are stored in the datum pointed to by val."
            "If no matching record exists, a null val is returned."
            ""
            "fetch_info() is only supported by MDBM version 3 or higher."
            ""
            "Note that a record can be updated in-place by fetching the value datum,"
            "casting the dptr as appropriate, and updating the record."
            "However, the update must not change the size of the record."
    },

    {"get_magic_number", (PyCFunction)pymdbm_get_magic_number, METH_NOARGS, 
        "get_magic_number()"
            "Gets the magic number from an MDBM."
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
    {"delete_r", (PyCFunction)pymdbm_delete_r, METH_VARARGS, 
        "delete_r([iter{m_pageno,m_next}])"
            "Deletes the record currently addressed by the iter argument."
            "After deletion, the key and/or value returned by the iterating function is no longer valid."
            "Calling next_r() on the iterator will return the key/value"
            "for the entry following the entry that was deleted."
            "PARAM : "
            "\tIf iter not specified, API uses the global iter"
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
    {"set_alignment", (PyCFunction)pymdbm_set_alignment, METH_VARARGS, 
        "set_alignment()"
            "Sets a database's byte-size alignment for keys and values within a page."
            "This feature is useful for hardware/memory architectures "
            "that incur a performance penalty for unaligned accesses."
            "Later (2006+) i386 and x86 architectures do not need special byte alignment,"
            "and should use the default of 8-bit alignment."
    },

    {"get_limit_size", (PyCFunction)pymdbm_get_limit_size, METH_NOARGS, 
        "get_limit_size()"
            "Gets the MDBM's size limit."
            "Returns the limit set for the size of the db using the limit_size_v3 routine."
            "return database size limit"
            "\t0 No limit is set"
            "\tTotal number of bytes for maximum database size, including header and directory"
    },

    {"limit_dir_size", (PyCFunction)pymdbm_limit_dir_size, METH_VARARGS, 
        "limit_dir_size(pages)"
            "Limits the internal page directory size to a number of pages."
            "The number of pages is rounded up to a power of 2."
    },

    {"setspillsize", (PyCFunction)pymdbm_setspillsize, METH_VARARGS, 
        "setspillsize(size)"
            "Sets the size of item data value which will be put on the large-object heap rather than inline."
            "The spill size can be changed at any point after the db has been created."
            "However, it's a recommended practice to set the spill size at creation time."
            ""
            "NOTE: The database has to be opened with the MDBM_LARGE_OBJECTS flag for spillsize to take effect."
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
    {"first_r", (PyCFunction)pymdbm_first_r, METH_VARARGS, 
        "first_r([iter{m_pageno,m_next}])"
            "Returns the first key/value pair from the database."
            "The order that records are returned is not specified."
            "PARAM : "
            "\tIf iter not specified, API uses the global iter"
    },
    {"next_r", (PyCFunction)pymdbm_next_r, METH_VARARGS, 
        "next_r([iter{m_pageno,m_next}])"
            "Fetches the next record in an MDBM."
            "Returns the next key/value pair from the db, based on the iterator."
            "PARAM : "
            "\tIf iter not specified, API uses the global iter"
    },

    {"firstkey_r", (PyCFunction)pymdbm_firstkey_r, METH_VARARGS, 
        "firstkey_r()"
            "Fetches the first key in an MDBM."
            "Initializes the iterator, and returns the first key from the db."
            "Subsequent calls to next_r() or nextkey_r() with this iterator will loop through the entire db."
            "PARAM : "
            "\tIf iter not specified, API uses the global iter"
    },
    {"nextkey_r", (PyCFunction)pymdbm_nextkey_r, METH_VARARGS, 
        "nextkey_r()"
            "Fetches the next key in an MDBM."
            "Returns the next key from the db."
            "Subsequent calls to next_r() or nextkey_r() with this iterator will loop through the entire db."
            "PARAM : "
            "\tIf iter not specified, API uses the global iter"
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
    {"clean", (PyCFunction)pymdbm_clean, METH_VARARGS, 
        "check(pagenum)"
            "Mark entries clean/re-usable in the database for the specified page."
            "If pagenum is -1, then clean all pages."
            "It relies on the user provided callback function set via set_cleanfunc(NOT SUPPROT) to determine"
            "re-usability/cleanliness of an entry. To be clean means an entry can be re-used to store new data."
            ""
            "NOTE: V3 API"
            ""
            "pagenum:"
            "\tpagenum Page number to start cleaning. If < 0, then clean all pages in the database."
    },
    {"check", (PyCFunction)pymdbm_check, METH_VARARGS | METH_KEYWORDS , 
        "check(level, verbose)"
            "Checks an MDBM's integrity, and displays info. on standard output."
            "level : "
            "\tMDBM_CHECK_HEADER    < Check MDBM header for integrity"
            "\tMDBM_CHECK_CHUNKS    < Check MDBM header and chunks (page structure)"
            "\tMDBM_CHECK_DIRECTORY < Check MDBM header, chunks, and directory"
            "\tMDBM_CHECK_ALL       < Check MDBM header, chunks, directory, and data"
            "verbose : True or False"
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
    {"replace_db", (PyCFunction)pymdbm_replace_db, METH_VARARGS, 
        "replace_db(new_file_path)"

            "Atomically replaces the database currently in oldfile db with the new database in newfile."
            "The old database is locked while the new database is renamed from newfile to oldfile "
            "and the old database is marked as having been replaced."
            "This causes all processes that have the old database open to reopen the new database on their next access."
            "Only database files of the same version may be specified for oldfile and new file."
            "For example, mix and matching of v2 and v3 with oldfile and newfile is not allowed."
            "NOTE : This function will delete the old file; and rename the new file."

    },
    {"replace_file", (PyCFunction)pymdbm_replace_file, METH_VARARGS | METH_KEYWORDS, 
        "replace_file(oldfile, newfile)"
            "Atomically replaces an old database in oldfile with a new database in newfile."
            "oldfile is deleted, and newfile is renamed to oldfile."
            ""
            "The old database is locked (if the MDBM were opened with locking) while the"
            "new database is renamed from newfile to oldfile, and the old database"
            "is marked as having been replaced."
            "The marked old database causes all processes that have "
            "the old database open to reopen using the new database on their next access."
            ""
            "Only database files of the same version may be specified for oldfile and newfile."
            "For example, mixing and matching of v2 and v3 for oldfile and newfile is not allowed."
            ""
            "mdbm_replace_file may be used if the MDBM is opened with locking "
            "or without locking (using mdbm_open flag MDBM_OPEN_NOLOCK),"
            "and without per-access locking, if all accesses are read (fetches) accesses across all programs that open that MDBM."
            "If there are any write (store/delete) accesses, you must open the MDBM with locking, and you must lock around all operations (fetch, store, delete, iterate)."
    },
    {"fcopy", (PyCFunction)pymdbm_fcopy, METH_VARARGS | METH_KEYWORDS, 
        "fcopy(newfile, [flags])"
            "Copies the contents of a database to an open file handle."
            "WARN"
            "\twill truncate an existing file (newfile)"
            "Values for flags mask:"
            "\tMDBM_COPY_LOCK_ALL - Whether lock for the duration of the copy."
            "\t\tFor a consistent snapshot of the entire database, this flag must be used."
            "\t\tOtherwise, consistency will only be on a per-page level."
    },
 
    {"get_hash_value", (PyCFunction)pymdbm_get_hash_value, METH_VARARGS | METH_KEYWORDS, 
        "get_hash_value(key, hashfunc)"
            "Given a hash function code, get the hash value for the given key."
            "See set_hash() for the list of valid hash function codes."
            "PARAM:"
            "Values for hashfunc:"
            "\t- MDBM_HASH_CRC32   - Table based 32bit CRC"
            "\t- MDBM_HASH_EJB     - From hsearch"
            "\t- MDBM_HASH_PHONG   - Congruential hash"
            "\t- MDBM_HASH_OZ      - From sdbm"
            "\t- MDBM_HASH_TOREK   - From BerkeleyDB"
            "\t- MDBM_HASH_FNV     - Fowler/Vo/Noll hash (DEFAULT)"
            "\t- MDBM_HASH_STL     - STL string hash"
            "\t- MDBM_HASH_MD5     - MD5"
            "\t- MDBM_HASH_SHA_1   - SHA_1"
            "\t- MDBM_HASH_JENKINS - Jenkins string"
            "\t- MDBM_HASH_HSIEH   - Hsieh SuperFast"
    },
    {"pre_split", (PyCFunction)pymdbm_pre_split, METH_VARARGS,
        "pre_split(N)"
            "Forces a db to split, creating N pages."
            "Must be called before any data is inserted."
            "If N is not a multiple of 2, it will be rounded up."
            "The N Target number of pages post split."
            "If N is not larger than the initial size (ex., 0),"
            "a split will not be done and a success status is returned."
    }, 
    {"dump_all_page", (PyCFunction)pymdbm_dump_all_page, METH_NOARGS,
        "dump_all_page()"
            "Dumps information for all pages, in version-specific format, to standard output."
    }, 
    {"dump_page", (PyCFunction)pymdbm_dump_page, METH_VARARGS,
        "dump_page(pagenum)"
            "Dumps specified page's information, in version-specific format, to standard output."
    }, 
    {"get_stats", (PyCFunction)pymdbm_get_stats, METH_NOARGS,
        "get_stats()"
            "Gets a a stats block with individual stat values."
    }, 
    {"get_db_info", (PyCFunction)pymdbm_get_db_info, METH_NOARGS,
        "get_stats()"
            "Gets configuration information about a database."
    }, 
    {"get_stat_counter", (PyCFunction)pymdbm_get_stat_counter, METH_VARARGS,
        "get_stat_counter(type)"
            "Gets the number of operations performed for a stat type."
            "Values for type:"
            "\t- MDBM_STAT_TYPE_FETCH  - For fetch* operations"
            "\t- MDBM_STAT_TYPE_STORE  - For store* operations"
            "\t- MDBM_STAT_TYPE_DELETE - For delete* operations"
            "Stat operations must be enabled for operations to be tracked."
            "Use enable_stat_operations() to enable this feature."
            "Once enabled, statistics are persisted in the MDBM and are not reset on close()."
            "Use program `mdbm_stat -H' to display stat operation metrics stored in the header."
    }, 
    {"get_stat_time", (PyCFunction)pymdbm_get_stat_time, METH_VARARGS,
        "get_stat_time(type)"
            "Gets the last time when an type operation was performed."
            "Values for  type:"
            "\t- MDBM_STAT_TYPE_FETCH  - For fetch* operations"
            "\t- MDBM_STAT_TYPE_STORE  - For store* operations"
            "\t- MDBM_STAT_TYPE_DELETE - For delete* operations"
            "Stat operations must be enabled for operations to be tracked."
            "Use enable_stat_operations() to enable this feature."
            "Once enabled, statistics are persisted in the MDBM and are not reset on close()."
            "Use program `mdbm_stat -H' to display stat operation metrics stored in the header."
    },
    {"reset_stat_operations", (PyCFunction)pymdbm_reset_stat_operations, METH_NOARGS,
        "reset_stat_operations()"
            "Resets the stat counter and last-time performed for fetch, store, and remove operations."
            "Stat operations must be enabled for operations to be tracked."
            "Use enable_stat_operations() to enable this feature."
            "If stat operations are not enabled, using this function will merely reset and already cleared storage."
            "Use program `mdbm_stat -H' to display stat operation metrics stored in the header."
    },
    {"enable_stat_operations", (PyCFunction)pymdbm_enable_stat_operations, METH_VARARGS,
        "enable_stat_operations(flags)"
            "Enables and disables gathering of stat counters and/or last-time performed for fetch, store, and remove operations."
            ""
            "Enables stat operations so that we can track one or both of:"
            "1. Operations counters fetch, store and remove."
            "2. Last timestamp when a fetch, store or delete was performed."
            ""
            "flags = MDBM_STATS_BASIC  enables gathering only the stats counters."
            "flags = MDBM_STATS_TIMED  enables gathering only the stats timestamps."
            "flags = (MDBM_STATS_BASIC | MDBM_STATS_TIMED)  enables both the stats counters and timestamps."
            "flags = 0  disables gathering of stats counters and timestamps."
    }, 
    {"set_stat_time_func", (PyCFunction)pymdbm_set_stat_time_func, METH_VARARGS,
        "set_stat_time_func(flags)"
            "Tells the MDBM library whether to use TSC (CPU TimeStamp Counters)"
            "for timing the performance of fetch, store and delete operations."
            "The standard behavior of timed stat operations is to use clock_gettime(MONOTONIC)"
            "flags == MDBM_CLOCK_TSC       Enables use of TSC"
            "flags == MDBM_CLOCK_STANDARD  Disables use of TSC"
    }, 
    {"get_db_stats", (PyCFunction)pymdbm_get_db_stats, METH_VARARGS,
        "get_db_stats(flags)"
            "Gets overall database stats."
            "Values for flags mask:"
            "\t- MDBM_STAT_NOLOCK    - Do not lock for overall operation"
            "\t- MDBM_ITERATE_NOLOCK - Do no lock for page-based iteration"
    },  
    {"get_window_stats", (PyCFunction)pymdbm_get_window_stats, METH_NOARGS,
        "get_window_stats()"
            "Used to retrieve statistics about windowing usage on the associated database."
            "The V3 statistics structure is as follows.  It may be extended in later versions,"
            "in which case the s_size parameter should be set to the sizeof the struct used for that version."
            "NOTE: V3 API"
    },  

    {"select_log_plugin", (PyCFunction)pymdbm_select_log_plugin, METH_VARARGS,
        "select_log_plugin(type)"
            "select a logging plugin by name, default type is stderr"
            "type:"
            "\tMDBM_LOG_TO_STDERR        STDERR (default)"
            "\tMDBM_LOG_TO_FILE          FILE (must be use with set_log_filename())"
            "\tMDBM_LOG_TO_SYSLOG        SYSLOG"
    },
    {"set_log_filename", (PyCFunction)pymdbm_set_log_filename, METH_VARARGS,
        "set_log_filename(file)"
            "select a filename for the `file` log plugin."
    },
    {0,0}
};

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
