

PY-MDBM


-   Py-mdbm is a Python binds to Yahoo! MDBM C API.
-   MDBM is a super-fast memory-mapped key/value store.
-   MDBM is an ndbm work-alike hashed database library based on sdbm
    which is based on Per-Aake Larson’s Dynamic Hashing algorithms.
-   MDBM is a high-performance, memory-mapped hash database similar to
    the homegrown libhash.
-   The records stored in a mdbm database may have keys and values of
    arbitrary and variable lengths.

    Build Stats      PHP-mdbm ver.       Y! mdbm ver.
  ---------------- ------------------ ------------------
   [Build Status]   [GitHub version]   [GitHub version]


API

Currently Supported APIs

the following is list of support api on now.

  -------------------------------------------------------------------
  Group                             API
  --------------------------------- ---------------------------------
  File Management                   mdbm_open, mdbm_close, mdbm_sync,
                                    mdbm_fsync, mdbm_close_fd,
                                    _~~mdbm_replace_db,
                                    mdbm_replace_file,
                                    mdbm_dup_handle, mdbm_pre_split,
                                    mdbm_fcopy~~_

  Configuration                     mdbm_get_version, mdbm_get_size,
                                    mdbm_get_page_size,
                                    mdbm_get_limit_size,
                                    mdbm_get_hash,
                                    _~~mdbm_get_magic_number,
                                    mdbm_setspillsize,
                                    mdbm_get_alignment,
                                    mdbm_set_alignment,
                                    mdbm_limit_size_v3,
                                    mdbm_limit_dir_size,
                                    mdbm_set_window_size~~_

  Record Access                     mdbm_fetch, mdbm_delete,
                                    mdbm_store, _~~mdbm_fetch_r~~,
                                    ~~mdbm_fetch_buf~~,
                                    ~~mdbm_fetch_dup_r~~,
                                    ~~mdbm_fetch_str~~,
                                    ~~mdbm_fetch_info~~,
                                    ~~mdbm_delete_r~~,
                                    ~~mdbm_delete_str~~,
                                    ~~mdbm_store_r~~,
                                    ~~mdbm_store_str~~_

  Record Iteration                  mdbm_first, mdbm_next,
                                    mdbm_firstkey, mdbm_nextkey,
                                    _~~mdbm_first_r~~,
                                    ~~mdbm_next_r~~,
                                    ~~mdbm_firstkey_r~~,
                                    ~~mdbm_nextkey_r~~,
                                    ~~mdbm_iterate~~_

  Locking                           mdbm_islocked, mdbm_isowned,
                                    mdbm_lock, mdbm_unlock,
                                    mdbm_lock_reset,
                                    mdbm_delete_lockfiles,
                                    mdbm_get_lockmode, mdbm_trylock,
                                    mdbm_plock, mdbm_punlock,
                                    mdbm_tryplock, mdbm_lock_shared,
                                    mdbm_trylock_shared,
                                    mdbm_lock_smart,
                                    mdbm_trylock_smart,
                                    mdbm_unlock_smart

  Data Management                   mdbm_compress_tree,
                                    mdbm_truncate, mdbm_purge,
                                    _~~mdbm_prune,
                                    mdbm_set_cleanfunc, mdbm_clean~~_

  Data Integrity                    mdbm_check, mdbm_chk_all_page,
                                    mdbm_chk_page, mdbm_protect,
                                    _~~mdbm_chk_error~~_

  Data Display                      _~~mdbm_dump_all_page~~,
                                    ~~mdbm_dump_page~~_

  Statistics                        mdbm_count_records,
                                    mdbm_count_pages,
                                    _~~mdbm_get_stat_counter,
                                    mdbm_get_stat_time,
                                    mdbm_reset_stat_operations,
                                    mdbm_enable_stat_operations,
                                    mdbm_set_stat_time_func,
                                    mdbm_get_stat_name,
                                    mdbm_set_stats_func,
                                    mdbm_get_stats, mdbm_get_db_info,
                                    mdbm_chunk_iterate,
                                    mdbm_get_db_stats,
                                    mdbm_get_window_stats~~_

  Cache and Backing Store           _~~mdbm_set_cachemode,
                                    mdbm_get_cachemode,
                                    mdbm_get_cachemode_name,
                                    mdbm_set_backingstore~~_

  Import and Export                 _~~mdbm_cdbdump_to_file,
                                    mdbm_cdbdump_trailer_and_close,
                                    mdbm_cdbdump_add_record,
                                    mdbm_dbdump_to_file,
                                    mdbm_dbdump_trailer_and_close,
                                    mdbm_dbdump_add_record,
                                    mdbm_dbdump_export_header,
                                    mdbm_dbdump_import_header,
                                    mdbm_dbdump_import,
                                    mdbm_cdbdump_import~~_

  Miscellaneous                     mdbm_preload, mdbm_get_errno,
                                    mdbm_get_page, mdbm_lock_pages,
                                    mdbm_unlock_pages,
                                    _~~mdbm_get_hash_value~~_
  -------------------------------------------------------------------

Deprecated APIs

  -----------------------------------------------------
  _API_             _STATUS_          _COMMENT_
  ----------------- ----------------- -----------------
  mdbm_save         DEPRECATED        mdbm_save is only
                                      supported for V2
                                      MDBMs.

  mdbm_restore      DEPRECATED        mdbm_restore is
                                      only supported
                                      for V2 MDBMs.

  mdbm_sethash      DEPRECATED        Legacy version of
                                      mdbm_set_hash()
                                      This function has
                                      inconsistent
                                      naming, an error
                                      return value. It
                                      will be removed
                                      in a future
                                      version.
  -----------------------------------------------------

Only a V2 implementation

  --------------------------------------------------------------
  _API_                _STATUS_             _COMMENT_
  -------------------- -------------------- --------------------
  mdbm_stat_all_page   V3 not supported     There is only a V2
                                            implementation. V3
                                            not currently
                                            supported.

  mdbm_stat_header     V3 not supported     There is only a V2
                                            implementation. V3
                                            not currently
                                            supported.
  --------------------------------------------------------------


Support two compatibility version

Python

  _Version_              _Support_   _Test_
  ---------------------- ----------- --------
  Python 2.6.x ~ 2.7.x   yes         always
  Python 3.0.x ~ 3.x.x   yes         always
  PyPy                   yes         always
  PyPy3                  yes         always

MDBM

  _branch or release ver._   _Support_   _Test_   _Comment_
  -------------------------- ----------- -------- -----------
  master                     yes         always   
  4.x                        yes         always   


Install

MDBM

-   Ubuntu : See the pre-build packages
-   RHEL (CentOS) : See the pre-build packages

py-mdbm

Download

    git clone https://github.com/torden/py-mdbm

Build and Test

    cd py-mdbm
    pip install -r requirements.txt
    CMD_PYTHON=/app/python/bin/python make


Example

See the Source Code for more details

Creating and populating a database

    import mdbm
    import random

    print("[*] Creating and populating a database")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    flags = flags | mdbm.MDBM_O_CREAT
    flags = flags | mdbm.MDBM_LARGE_OBJECTS
    flags = flags | mdbm.MDBM_ANY_LOCKS
    flags = flags | mdbm.MDBM_O_TRUNC
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)
    for i in range(0, 65535):
        k = str(i)
        v = str(random.randrange(0, 65535))

        rv = dbm.store(k, v, mdbm.MDBM_INSERT)
        if not rv:
            print("[-] failed to data store to ", path)
            break

    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")

Fetching records in-place

    import mdbm
    import random

    print("[*] Fetching records in-place")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)
    dbm.preload()

    print("|-------|-------|")
    print("|  key  |  val  |")
    print("|-------|-------|")

    for i in range(0, 10):

        k = str(random.randrange(0, 65534))
        orgval = dbm.fetch(k)
        if not orgval:
            print("[-] failed to fetch value of %s in mdbm" % k)
            break

        print("|%07s|%07s|" % (k, orgval))

    print("|-------|-------|")
    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")

Fetching and updating records in-place

    import mdbm
    import random

    print("[*] Fetching and updating records in-place")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)
    for i in range(0, 65535):
        k = str(i)
        v = str(random.randrange(0, 65535))

        orgval = dbm.fetch(k)
        if not orgval:
            print("[-] failed to fetch value of %s in mdbm" % k)
            break

        print("[=] key(%s) : replace val(%s) to '%s' : " % (k, orgval, v)),

        rv = dbm.store(k, v, mdbm.MDBM_REPLACE)
        if not rv:
            print("FAIL")
            break
        print("DONE")

    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")

Deleting records in-place

    import mdbm
    import random

    print("[*] Deleting records in-place")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)

    for i in range(0, 10):

        k = str(random.randrange(0, 65534))

        rv = dbm.delete(k)
        if not rv:
            print("[-] failed to delete an record, key=%s" % k)

        v = dbm.fetch(k)
        if v:
            print("[-] failed to delete an record, key=%s, val=%s" % (k,v))
            break

    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")

Iterating over all records

    import mdbm
    import random

    print("[*] Iterating over all records")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)

    print("|-------|-------|")
    print("|  key  |  val  |")
    print("|-------|-------|")

    kv = dbm.first()

    print("|%07s|%07s|" % kv)

    while kv:

        print("|%07s|%07s|" % kv)

        kv = dbm.next()

    print("|-------|-------|")
    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")

Iterating over all keys

    import mdbm
    import random

    print("[*] Iterating over all records")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)

    print("|-------|")
    print("|  key  |")
    print("|-------|")

    k = dbm.firstkey()

    print("|%07s|" % k)

    while k:

        print("|%07s|" % k)

        k = dbm.nextkey()

    print("|-------|")
    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")


Benchmark

The following is results of Py-mdbm vs AnyDBM vs SQLite3 vs Kyotocabinet
benchmarks for simple data storing and random fetching in them.

See the Source Code for more details

Spec

Host

  Type   Spec
  ------ --------------
  CPU    Inte i7
  RAM    DDR4 32G
  HDD    Nvme M.2 SSD

VM

  Type           Spec
  -------------- ---------------------------------------------------------
  Machine        VM(VirtualBox)
  OS             Ubuntu 17.10 (Artful Aardvark)
  CPU            2 vCore
  RAM            8G
  AnyDBM         Berkeley DB (Hash, version 9, native byte-order) format
  Mdbm           893f7a8 on 26 Jul, MDBM V3 format
  SQLite         V3
  Kyotocabinet   1.2.76, kch

Command

    make benchmark

10,000 INSERTs

_NOTE :_ sqlite3 uses executemany

    platform linux2 -- Python 2.7.9, pytest-3.3.2, py-1.5.2, pluggy-0.6.0
    benchmark: 3.1.1 (defaults: timer=time.time disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)

    ------------------------------------------------------------------------------------------- benchmark: 4 tests -------------------------------------------------------------------------------------------
    Name (time in ms)                          Min                 Max                Mean            StdDev              Median                IQR            Outliers      OPS            Rounds  Iterations
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_10000                  35.6030 (1.0)       37.6830 (1.0)       35.9839 (1.0)      0.3624 (1.0)       35.9355 (1.0)       0.1595 (1.0)           2;3  27.7902 (1.0)          28           1
    test_kyotocabinet_kch_store_10000      45.0001 (1.26)      73.1311 (1.94)      63.6352 (1.77)     4.6661 (12.88)     63.6289 (1.77)      2.4445 (15.33)         2;2  15.7146 (0.57)         24           1
    test_anydbm_store_10000               175.3871 (4.93)     192.1310 (5.10)     180.4350 (5.01)     7.4633 (20.60)    175.4749 (4.88)     10.4155 (65.30)         1;0   5.5422 (0.20)          5           1
    test_sqlite3_store_10000              234.0219 (6.57)     250.5929 (6.65)     239.2666 (6.65)     7.2336 (19.96)    235.2481 (6.55)     10.4381 (65.44)         1;0   4.1794 (0.15)          5           1
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

100,000 INSERTs

_NOTE :_ sqlite3 uses executemany

    ----------------------------------------------------------------------------------------------- benchmark: 4 tests ----------------------------------------------------------------------------------------------
    Name (time in ms)                             Min                   Max                  Mean            StdDev                Median               IQR            Outliers     OPS            Rounds  Iterations
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_100000                   355.6280 (1.0)        356.9429 (1.0)        356.3805 (1.0)      0.4842 (1.0)        356.4131 (1.0)      0.5462 (1.0)           2;0  2.8060 (1.0)           5           1
    test_kyotocabinet_kch_store_100000       423.9750 (1.19)       433.8830 (1.22)       428.0958 (1.20)     4.1842 (8.64)       428.6020 (1.20)     6.8427 (12.53)         1;0  2.3359 (0.83)          5           1
    test_sqlite3_store_100000                600.6498 (1.69)       625.6468 (1.75)       615.9640 (1.73)     9.2206 (19.04)      617.4102 (1.73)     7.2821 (13.33)         2;1  1.6235 (0.58)          5           1
    test_anydbm_store_100000               1,516.9890 (4.27)     1,525.4049 (4.27)     1,518.7171 (4.26)     3.7389 (7.72)     1,517.0460 (4.26)     2.1866 (4.00)          1;1  0.6585 (0.23)          5           1
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

10,000 Random Key SELECTs

    ---------------------------------------------------------------------------------------------------- benchmark: 5 tests ----------------------------------------------------------------------------------------------------
    Name (time in ms)                                Min                    Max                   Mean              StdDev                 Median                 IQR            Outliers      OPS            Rounds  Iterations
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_preload_random_fetch_10000         21.8220 (1.0)          22.1212 (1.0)          21.9571 (1.0)        0.0711 (1.0)          21.9536 (1.0)        0.0961 (1.0)          15;0  45.5434 (1.0)          46           1
    test_mdbm_random_fetch_10000                 21.9901 (1.01)         22.5182 (1.02)         22.1769 (1.01)       0.0830 (1.17)         22.1779 (1.01)       0.1004 (1.04)         11;1  45.0920 (0.99)         45           1
    test_kyotocabinet_random_fetch_10000         36.8328 (1.69)         37.3409 (1.69)         37.0176 (1.69)       0.1339 (1.88)         36.9780 (1.68)       0.1321 (1.37)          7;2  27.0142 (0.59)         27           1
    test_anydbm_random_fetch_10000               94.8422 (4.35)         95.7720 (4.33)         95.3817 (4.34)       0.2782 (3.91)         95.3591 (4.34)       0.4044 (4.21)          3;0  10.4842 (0.23)         11           1
    test_sqlite3_random_fetch_10000          80,545.1789 (>1000.0)  80,840.8899 (>1000.0)  80,641.6320 (>1000.0)  126.0336 (>1000.0)  80,587.2970 (>1000.0)  182.1152 (>1000.0)       1;0   0.0124 (0.00)          5           1
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

100,000 Random Key SELECTs

    ---------------------------------------------------------------------------------------------- benchmark: 4 tests ----------------------------------------------------------------------------------------------
    Name (time in ms)                              Min                   Max                Mean             StdDev              Median                IQR            Outliers     OPS            Rounds  Iterations
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_preload_random_fetch_100000     225.6060 (1.0)        227.1869 (1.0)      226.5218 (1.0)       0.6364 (1.0)      226.5248 (1.0)       0.9841 (1.18)          2;0  4.4146 (1.0)           5           1
    test_mdbm_random_fetch_100000             226.4791 (1.00)       228.4970 (1.01)     227.0140 (1.00)      0.8486 (1.33)     226.6810 (1.00)      0.8308 (1.0)           1;0  4.4050 (1.00)          5           1
    test_kyotocabinet_random_fetch_100000     352.7610 (1.56)       356.5919 (1.57)     354.4669 (1.56)      1.3991 (2.20)     354.3491 (1.56)      1.5802 (1.90)          2;0  2.8211 (0.64)          5           1
    test_anydbm_random_fetch_100000           973.2511 (4.31)     1,007.8731 (4.44)     982.5035 (4.34)     14.3176 (22.50)    976.7361 (4.31)     10.7591 (12.95)         1;1  1.0178 (0.23)          5           1
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


Link

-   Yahoo! MDBM
-   MDBM::Concept
-   MDBM::Build
-   MDBM::Document
-   MDBM::FAQ
-   DBM
-   MDBM::Macro(const)
-   Go-mdbm
-   PHP-mdbm

------------------------------------------------------------------------

Please feel free. I hope it is helpful for you