#ifndef __LIBMYPY_H__
#define __LIBMYPY_H__

#include <Python.h>
#include <mdbm.h>
#include <mdbm_log.h>


typedef struct {
    PyObject_HEAD
    MDBM *pmdbm;
} MDBMObj;

PyObject *MDBMError = NULL;
static int loglevel = -1;
static int dev_null, org_stdout, org_stderr;

PyObject *pymdbm_open(PyObject *self, PyObject *args);
PyObject *pymdbm_close(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_close_fd(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_sync(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_fsync(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_preload(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_compress_tree(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_truncate(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_purge(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_store(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_fetch(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_get_page(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_delete(register MDBMObj *pmdbm_link, PyObject *args);


PyObject *pymdbm_get_hash(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_set_hash(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_get_alignment(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_limit_size(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_version(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_size(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_get_page_size(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_lock(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_unlock(register MDBMObj *pmdbm_link, PyObject *unused);


PyObject *pymdbm_first(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_next(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_firstkey(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_nextkey(register MDBMObj *pmdbm_link, PyObject *unused);



PyObject *pymdbm_islocked(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_isowned(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_lock_reset(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_delete_lockfiles(register MDBMObj *pmdbm_link, PyObject *args);

PyObject *pymdbm_check(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_chk_page(register MDBMObj *pmdbm_link, PyObject *args);
PyObject *pymdbm_chk_all_page(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_count_records(register MDBMObj *pmdbm_link, PyObject *unused);
PyObject *pymdbm_count_pages(register MDBMObj *pmdbm_link, PyObject *unused);

PyObject *pymdbm_get_errno(register MDBMObj *pmdbm_link, PyObject *unused);

/*
mdbm_protect
mdbm_chk_error
mdbm_set_cachemode
mdbm_get_cachemode
mdbm_get_cachemode_name

mdbm_lock_pages
mdbm_unlock_pages
mdbm_get_hash_value
*/

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
