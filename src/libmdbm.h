#ifndef __LIBMYPY_H__
#define __LIBMYPY_H__

#include <Python.h>
#include <mdbm.h>
#include <mdbm_log.h>


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

PyObject *pymdbm_store(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_store_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_fetch(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_fetch_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_fetch_dup_r(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_delete(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_delete_r(register MDBMObj *pmdbm_link, PyObject *args);

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

PyObject *pymdbm_check(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);
PyObject *pymdbm_chk_page(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_chk_all_page(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_count_records(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_count_pages(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_get_errno(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_protect(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_replace_db(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_replace_file(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);

PyObject *pymdbm_get_hash_value(register MDBMObj *pmdbm_link, PyObject *args, PyObject *kwds);

#if PY_MAJOR_VERSION >= 3
PyObject *pymdbm__enter(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm__exit(register MDBMObj *pmdbm_link, PyObject *args);
#endif


#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
