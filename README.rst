Py-mdbm
=======

-  Py-mdbm is a Python binds to `Yahoo! MDBM C
   API. <https://github.com/yahoo/mdbm/>`__
-  MDBM is a super-fast memory-mapped key/value store.
-  MDBM is an ndbm work-alike hashed database library based on sdbm
   which is based on Per-Aake Larson’s Dynamic Hashing algorithms.
-  MDBM is a high-performance, memory-mapped hash database similar to
   the homegrown libhash.
-  The records stored in a mdbm database may have keys and values of
   arbitrary and variable lengths.

+----------------+------------------+------------------+
| Build Stats    | Py-mdbm ver.     | Y! mdbm ver.     |
+================+==================+==================+
| |Build Status| | |GitHub version| | |GitHub version| |
+----------------+------------------+------------------+

API
---

Currently Supported APIs
~~~~~~~~~~~~~~~~~~~~~~~~

the following is list of support api on now.

+-----------------------------------+-----------------------------------+
| Group                             | API                               |
+===================================+===================================+
| `File                             | mdbm_open, mdbm_close, mdbm_sync, |
| Management <http://yahoo.github.i | mdbm_fsync, mdbm_close_fd,        |
| o/mdbm/api/group__FileManagementG | mdbm_replace_db,                  |
| roup.html>`__                     | mdbm_replace_file,                |
|                                   | mdbm_dup_handle, mdbm_pre_split,  |
|                                   | mdbm_fcopy                        |
+-----------------------------------+-----------------------------------+
| `Configuration <http://yahoo.gith | mdbm_get_version, mdbm_get_size,  |
| ub.io/mdbm/api/group__Configurati | mdbm_get_page_size,               |
| onGroup.html>`__                  | mdbm_get_limit_size,              |
|                                   | mdbm_get_hash,                    |
|                                   | mdbm_get_alignment,               |
|                                   | mdbm_set_alignment,               |
|                                   | mdbm_setspillsize,                |
|                                   | mdbm_limit_dir_size,              |
|                                   | mdbm_get_magic_number,            |
|                                   | *[STRIKEOUT:mdbm_limit_size_v3,   |
|                                   | mdbm_set_window_size]*            |
+-----------------------------------+-----------------------------------+
| `Record                           | mdbm_fetch, mdbm_delete,          |
| Access <http://yahoo.github.io/md | mdbm_store, mdbm_fetch_r,         |
| bm/api/group__RecordAccessGroup.h | mdbm_fetch_dup_r, mdbm_delete_r,  |
| tml>`__                           | mdbm_store_r, mdbm_fetch_info     |
+-----------------------------------+-----------------------------------+
| `Record                           | mdbm_first, mdbm_next,            |
| Iteration <http://yahoo.github.io | mdbm_firstkey, mdbm_nextkey,      |
| /mdbm/api/group__RecordIterationG | mdbm_first_r, mdbm_next_r,        |
| roup.html>`__                     | mdbm_firstkey_r, mdbm_nextkey_r,  |
|                                   | *[STRIKEOUT:mdbm_iterate]*        |
+-----------------------------------+-----------------------------------+
| `Locking <http://yahoo.github.io/ | mdbm_islocked, mdbm_isowned,      |
| mdbm/api/group__LockingGroup.html | mdbm_lock, mdbm_unlock,           |
| >`__                              | mdbm_lock_reset,                  |
|                                   | mdbm_delete_lockfiles,            |
|                                   | mdbm_get_lockmode, mdbm_trylock,  |
|                                   | mdbm_plock, mdbm_punlock,         |
|                                   | mdbm_tryplock, mdbm_lock_shared,  |
|                                   | mdbm_trylock_shared,              |
|                                   | mdbm_lock_smart,                  |
|                                   | mdbm_trylock_smart,               |
|                                   | mdbm_unlock_smart                 |
+-----------------------------------+-----------------------------------+
| `Data                             | mdbm_compress_tree,               |
| Management <http://yahoo.github.i | mdbm_truncate, mdbm_purge,        |
| o/mdbm/api/group__DataManagementG | mdbm_clean,                       |
| roup.html>`__                     | *[STRIKEOUT:mdbm_prune,           |
|                                   | mdbm_set_cleanfunc]*              |
+-----------------------------------+-----------------------------------+
| `Data                             | mdbm_check, mdbm_chk_all_page,    |
| Integrity <http://yahoo.github.io | mdbm_chk_page, mdbm_protect       |
| /mdbm/api/group__DataIntegrityGro |                                   |
| up.html>`__                       |                                   |
+-----------------------------------+-----------------------------------+
| `Data                             | mdbm_dump_all_page,               |
| Display <http://yahoo.github.io/m | mdbm_dump_page                    |
| dbm/api/group__DataDisplayGroup.h |                                   |
| tml>`__                           |                                   |
+-----------------------------------+-----------------------------------+
| `Statistics <http://yahoo.github. | mdbm_count_records,               |
| io/mdbm/api/group__StatisticsGrou | mdbm_count_pages, mdbm_get_stats, |
| p.html>`__                        | mdbm_get_db_info,                 |
|                                   | mdbm_get_stat_counter,            |
|                                   | mdbm_get_stat_time,               |
|                                   | mdbm_reset_stat_operations,       |
|                                   | mdbm_enable_stat_operations,      |
|                                   | mdbm_set_stat_time_func,          |
|                                   | mdbm_get_db_stats,                |
|                                   | mdbm_get_window_stats,            |
|                                   | *[STRIKEOUT:mdbm_get_stat_name,   |
|                                   | mdbm_set_stats_func,              |
|                                   | mdbm_chunk_iterate]*              |
+-----------------------------------+-----------------------------------+
| `Cache and Backing                | mdbm_set_cachemode,               |
| Store <http://yahoo.github.io/mdb | mdbm_get_cachemode,               |
| m/api/group__CacheAndBackingStore | mdbm_get_cachemode_name,          |
| Group.html>`__                    | *[STRIKEOUT:mdbm_set_backingstore |
|                                   | ]*                                |
+-----------------------------------+-----------------------------------+
| `Import and                       | *[STRIKEOUT:mdbm_cdbdump_to_file, |
| Export <http://yahoo.github.io/md | mdbm_cdbdump_trailer_and_close,   |
| bm/api/group__ImportExportGroup.h | mdbm_cdbdump_add_record,          |
| tml>`__                           | mdbm_dbdump_to_file,              |
|                                   | mdbm_dbdump_trailer_and_close,    |
|                                   | mdbm_dbdump_add_record,           |
|                                   | mdbm_dbdump_export_header,        |
|                                   | mdbm_dbdump_import_header,        |
|                                   | mdbm_dbdump_import,               |
|                                   | mdbm_cdbdump_import]*             |
+-----------------------------------+-----------------------------------+
| `Miscellaneous <http://yahoo.gith | mdbm_preload, mdbm_get_errno,     |
| ub.io/mdbm/api/group__Miscellaneo | mdbm_get_page, mdbm_lock_pages,   |
| usGroup.html>`__                  | mdbm_unlock_pages,                |
|                                   | mdbm_get_hash_value               |
+-----------------------------------+-----------------------------------+

Deprecated APIs
~~~~~~~~~~~~~~~

+-----------------------+-----------------------+-----------------------+
| *API*                 | *STATUS*              | *COMMENT*             |
+=======================+=======================+=======================+
| mdbm_save             | DEPRECATED            | mdbm_save is only     |
|                       |                       | supported for V2      |
|                       |                       | MDBMs.                |
+-----------------------+-----------------------+-----------------------+
| mdbm_restore          | DEPRECATED            | mdbm_restore is only  |
|                       |                       | supported for V2      |
|                       |                       | MDBMs.                |
+-----------------------+-----------------------+-----------------------+
| mdbm_sethash          | DEPRECATED            | Legacy version of     |
|                       |                       | mdbm_set_hash() This  |
|                       |                       | function has          |
|                       |                       | inconsistent naming,  |
|                       |                       | an error return       |
|                       |                       | value. It will be     |
|                       |                       | removed in a future   |
|                       |                       | version.              |
+-----------------------+-----------------------+-----------------------+

Only a V2 implementation
~~~~~~~~~~~~~~~~~~~~~~~~

+-----------------------+-----------------------+-----------------------+
| *API*                 | *STATUS*              | *COMMENT*             |
+=======================+=======================+=======================+
| mdbm_stat_all_page    | V3 not supported      | There is only a V2    |
|                       |                       | implementation. V3    |
|                       |                       | not currently         |
|                       |                       | supported.            |
+-----------------------+-----------------------+-----------------------+
| mdbm_stat_header      | V3 not supported      | There is only a V2    |
|                       |                       | implementation. V3    |
|                       |                       | not currently         |
|                       |                       | supported.            |
+-----------------------+-----------------------+-----------------------+

Has not been implemented
~~~~~~~~~~~~~~~~~~~~~~~~

+---------------+-----------------+-------------------------------+
| *API*         | *STATUS*        | *COMMENT*                     |
+===============+=================+===============================+
| dbm_chk_error | Not Implemented | This has not been implemented |
+---------------+-----------------+-------------------------------+

Support two compatibility version
---------------------------------

Python
~~~~~~

+----------------------+-----------+--------+
| *Version*            | *Support* | *Test* |
+======================+===========+========+
| Python 2.6.x ~ 2.7.x | yes       | always |
+----------------------+-----------+--------+
| Python 3.0.x ~ 3.x.x | yes       | always |
+----------------------+-----------+--------+
| PyPy                 | yes       | always |
+----------------------+-----------+--------+
| PyPy3                | yes       | always |
+----------------------+-----------+--------+

MDBM
~~~~

+--------------------------+-----------+--------+-----------+
| *branch or release ver.* | *Support* | *Test* | *Comment* |
+==========================+===========+========+===========+
| master                   | yes       | always |           |
+--------------------------+-----------+--------+-----------+
| 4.x                      | yes       | always |           |
+--------------------------+-----------+--------+-----------+

Install
-------

.. _mdbm-1:

MDBM
~~~~

-  Ubuntu : See the `pre-build
   packages <https://github.com/torden/go-mdbm/tree/master/pkg>`__
-  RHEL (CentOS) : See the `pre-build
   packages <https://github.com/torden/go-mdbm/tree/master/pkg>`__

py-mdbm (use pip)
~~~~~~~~~~~~~~~~~

.. code:: shell

    pip install py-mdbm

py-mdbm (use source)
~~~~~~~~~~~~~~~~~~~~

Download
^^^^^^^^

.. code:: shell

    git clone https://github.com/torden/py-mdbm

Build and Test
^^^^^^^^^^^^^^

.. code:: shell

    cd py-mdbm
    CMD_PYTHON=`which python` make

Check
~~~~~

.. code:: shell

    $ python
    >>> import mdbm
    >>> help(mdbm)

Benchmark
~~~~~~~~~

.. code:: shell

    cd py-mdbm
    `which pip` install -r requirements.txt
    CMD_PYTHON=`which python` CMD_PYTEST=`which pytest` make benchmark

Example
-------

See the `Source
Code <https://github.com/torden/py-mdbm/tree/master/example>`__ for more
details

The following is Sample codes for a first look at py-mdbm

Creating and populating a database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Python 2 or higher
^^^^^^^^^^^^^^^^^^

.. code:: python

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

Python 3 or higher
^^^^^^^^^^^^^^^^^^

.. code:: python

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

    with mdbm.open(path, flags, mode) as dbm:
        for i in range(0, 65535):
            k = str(i)
            v = str(random.randrange(0, 65535))

            rv = dbm.store(k, v, mdbm.MDBM_INSERT)
            if not rv:
                print("[-] failed to data store to ", path)
                break

        print("[*] count of records : %d" % dbm.count_records())

    print("done")

Fetching records in-place
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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

Iteration over all value by key
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    import mdbm
    import random

    print("[*] Creating and populating a database")

    path = "/tmp/test_py_dup.mdbm"
    flags = mdbm.MDBM_O_RDWR
    flags = flags | mdbm.MDBM_O_CREAT
    flags = flags | mdbm.MDBM_LARGE_OBJECTS
    flags = flags | mdbm.MDBM_ANY_LOCKS
    flags = flags | mdbm.MDBM_O_TRUNC
    mode = 0o644  # means 0644

    with mdbm.open(path, flags, mode) as dbm:

        for k in range(0, 100):
            key = str(k)

            for i in range(1, 12):
                val = str(123 * i)

                rv = dbm.store(key, val, mdbm.MDBM_INSERT_DUP)
                if not rv:
                    print("[-] failed to data store to ", path)
                    break

    print("[*] Loop through DB, looking at records with the same key.")
    with mdbm.open(path, mdbm.MDBM_O_RDONLY, mode) as dbm:

        print("[*] count of records : %d" % dbm.count_records())
        print("|-------|-------|")
        print("|  key  |  val  |")
        print("|-------|-------|")

        k = str(random.randrange(0, 99))

        empty_iter = dbm.init_iter()
        info = dbm.fetch_dup_r(k, empty_iter)
        while info:

            print("|%07s|%07s|" % (k, info['val']))
            info = dbm.fetch_dup_r(k, info['iter'])

    print("|-------|-------|")

    print("done")

.. _benchmark-1:

Benchmark
---------

The following is results of Py-mdbm vs AnyDBM vs SQLite3 vs Kyotocabinet
benchmarks for simple data storing and random fetching in them.

-  See the `Source
   Code <https://github.com/torden/py-mdbm/blob/master/test_benchmark.py>`__
   for more details
-  See the
   `Glossary <https://pytest-benchmark.readthedocs.io/en/latest/glossary.html>`__
   for read result

Spec
~~~~

Host
^^^^

+------+--------------+
| Type | Spec         |
+======+==============+
| CPU  | Inte i-7     |
+------+--------------+
| RAM  | DDR4 32G     |
+------+--------------+
| HDD  | Nvme M.2 SSD |
+------+--------------+

VM
^^

+--------------+---------------------------------------------------------+
| Type         | Spec                                                    |
+==============+=========================================================+
| Machine      | VM(VirtualBox)                                          |
+--------------+---------------------------------------------------------+
| OS           | CentOS 7 64bit                                          |
+--------------+---------------------------------------------------------+
| CPU          | 2 vCore                                                 |
+--------------+---------------------------------------------------------+
| RAM          | 8G                                                      |
+--------------+---------------------------------------------------------+
| AnyDBM       | Berkeley DB (Hash, version 9, native byte-order) format |
+--------------+---------------------------------------------------------+
| Mdbm         | 893f7a8 on 26 Jul, MDBM V3 format                       |
+--------------+---------------------------------------------------------+
| SQLite       | V3                                                      |
+--------------+---------------------------------------------------------+
| Kyotocabinet | 1.2.76, kch                                             |
+--------------+---------------------------------------------------------+

Command
~~~~~~~

::

    CMD_PYTHON=`which python` CMD_PYTEST=`which pytest` make benchmark

File Size
~~~~~~~~~

+------------------+------------------+------------------------------------+------+
| Count of Records | Type             | File Name                          | Size |
+==================+==================+====================================+======+
| 10,000           | SQLite3          | test_py_benchmark_10000.db         | 300K |
+------------------+------------------+------------------------------------+------+
|                  | AnyDBM           | test_py_benchmark_10000.dbm        | 348K |
+------------------+------------------+------------------------------------+------+
|                  | Kyotocabinet KCH | test_py_benchmark_10000.kch        | 6.3M |
+------------------+------------------+------------------------------------+------+
|                  | MDBM             | test_py_benchmark_10000.mdbm       | 260K |
+------------------+------------------+------------------------------------+------+
|                  | MDBM(TSC)        | test_py_benchmark_tsc_10000.mdbm   | 260K |
+------------------+------------------+------------------------------------+------+
| 100,000          | SQLite3          | test_py_benchmark_100000.db        | 3.3M |
+------------------+------------------+------------------------------------+------+
|                  | AnyDBM           | test_py_benchmark_100000.dbm       | 2.5M |
+------------------+------------------+------------------------------------+------+
|                  | Kyotocabinet KCH | test_py_benchmark_100000.kch       | 9.1M |
+------------------+------------------+------------------------------------+------+
|                  | MDBM             | test_py_benchmark_100000.mdbm      | 4.0M |
+------------------+------------------+------------------------------------+------+
|                  | MDBM(TSC)        | test_py_benchmark_tsc_100000.mdbm  | 4.0M |
+------------------+------------------+------------------------------------+------+
| 1,000,000        | SQLite3          | test_py_benchmark_1000000.db       | 35M  |
+------------------+------------------+------------------------------------+------+
|                  | AnyDBM           | test_py_benchmark_1000000.dbm      | 39M  |
+------------------+------------------+------------------------------------+------+
|                  | Kyotocabinet KCH | test_py_benchmark_1000000.kch      | 37M  |
+------------------+------------------+------------------------------------+------+
|                  | MDBM             | test_py_benchmark_1000000.mdbm     | 32M  |
+------------------+------------------+------------------------------------+------+
|                  | MDBM(TSC)        | test_py_benchmark_tsc_1000000.mdbm | 32M  |
+------------------+------------------+------------------------------------+------+

10,000 INSERTs
~~~~~~~~~~~~~~

::

    platform linux2 -- Python 2.7.14, pytest-3.3.2, py-1.5.2, pluggy-0.6.0
    benchmark: 3.1.1 (defaults: timer=time.time disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)
    rootdir: /root/PERSONAL/py-mdbm, inifile:
    plugins: benchmark-3.1.1
    collected 31 items

    ------------------------------------------------------------------------------------------- benchmark: 5 tests ------------------------------------------------------------------------------------------
    Name (time in ms)                          Min                 Max                Mean            StdDev              Median               IQR            Outliers      OPS            Rounds  Iterations
    ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_tsc_10000              42.7790 (1.0)       46.4041 (1.0)       44.4735 (1.0)      0.8599 (1.0)       44.7228 (1.01)     1.0532 (1.0)           5;0  22.4853 (1.0)          23           1
    test_mdbm_store_10000                  43.0260 (1.01)      55.0859 (1.19)      45.1026 (1.01)     2.8206 (3.28)      44.1189 (1.0)      1.9995 (1.90)          3;2  22.1716 (0.99)         23           1
    test_kyotocabinet_kch_store_10000      64.2769 (1.50)      72.2461 (1.56)      66.6182 (1.50)     2.1470 (2.50)      66.5540 (1.51)     2.4997 (2.37)          6;1  15.0109 (0.67)         16           1
    test_sqlite3_store_10000               71.1770 (1.66)      89.0980 (1.92)      74.6003 (1.68)     4.5800 (5.33)      73.3149 (1.66)     2.8142 (2.67)          1;1  13.4048 (0.60)         13           1
    test_anydbm_store_10000               129.4661 (3.03)     132.9770 (2.87)     131.7690 (2.96)     1.3268 (1.54)     132.4065 (3.00)     2.1240 (2.02)          1;0   7.5890 (0.34)          8           1
    ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

.. _inserts-1:

100,000 INSERTs
~~~~~~~~~~~~~~~

::

    ------------------------------------------------------------------------------------------------ benchmark: 5 tests -----------------------------------------------------------------------------------------------
    Name (time in ms)                             Min                   Max                  Mean             StdDev                Median                IQR            Outliers     OPS            Rounds  Iterations
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_100000                   432.5280 (1.0)        444.3109 (1.0)        440.1428 (1.0)       5.1283 (1.0)        443.0151 (1.0)       7.8554 (1.46)          1;0  2.2720 (1.0)           5           1
    test_mdbm_store_tsc_100000               443.6021 (1.03)       457.2010 (1.03)       450.7210 (1.02)      6.5694 (1.28)       453.4068 (1.02)     12.3150 (2.28)          2;0  2.2187 (0.98)          5           1
    test_kyotocabinet_kch_store_100000       553.1771 (1.28)       572.2950 (1.29)       559.4640 (1.27)      7.3967 (1.44)       557.5171 (1.26)      5.3908 (1.0)           1;1  1.7874 (0.79)          5           1
    test_sqlite3_store_100000                668.3731 (1.55)       690.7680 (1.55)       676.8432 (1.54)     10.4372 (2.04)       670.3589 (1.51)     17.5762 (3.26)          1;0  1.4774 (0.65)          5           1
    test_anydbm_store_100000               1,746.3379 (4.04)     1,778.0671 (4.00)     1,759.8858 (4.00)     12.6857 (2.47)     1,761.1270 (3.98)     19.0974 (3.54)          2;0  0.5682 (0.25)          5           1
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

.. _inserts-2:

1,000,000 INSERTs
~~~~~~~~~~~~~~~~~

::

    ----------------------------------------------------------------------------------------- benchmark: 5 tests -----------------------------------------------------------------------------------------
    Name (time in s)                            Min                Max               Mean            StdDev             Median               IQR            Outliers     OPS            Rounds  Iterations
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_1000000                  4.4507 (1.0)       4.5549 (1.0)       4.5087 (1.0)      0.0386 (1.41)      4.5170 (1.00)     0.0471 (1.0)           2;0  0.2218 (1.0)           5           1
    test_mdbm_store_tsc_1000000              4.4964 (1.01)      4.5557 (1.00)      4.5252 (1.00)     0.0275 (1.0)       4.5133 (1.0)      0.0494 (1.05)          3;0  0.2210 (1.00)          5           1
    test_kyotocabinet_kch_store_1000000      5.5518 (1.25)      7.3104 (1.60)      5.9554 (1.32)     0.7585 (27.62)     5.6386 (1.25)     0.4548 (9.65)          1;1  0.1679 (0.76)          5           1
    test_sqlite3_store_1000000               6.9506 (1.56)      7.1580 (1.57)      7.0168 (1.56)     0.0811 (2.95)      6.9938 (1.55)     0.0623 (1.32)          1;1  0.1425 (0.64)          5           1
    test_anydbm_store_1000000               18.8494 (4.24)     19.3685 (4.25)     19.1384 (4.24)     0.1884 (6.86)     19.1481 (4.24)     0.1982 (4.21)          2;0  0.0523 (0.24)          5           1
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

10,000 Random Key SELECTs
~~~~~~~~~~~~~~~~~~~~~~~~~

::

    ----------------------------------------------------------------------------------------------- benchmark: 6 tests -----------------------------------------------------------------------------------------------
    Name (time in ms)                                 Min                 Max                Mean             StdDev              Median                IQR            Outliers      OPS            Rounds  Iterations
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_random_fetch_10000                  33.6039 (1.0)       37.1680 (1.02)      35.4372 (1.0)       0.8726 (1.63)      35.3181 (1.0)       1.0861 (1.73)          9;0  28.2189 (1.0)          29           1
    test_mdbm_preload_random_fetch_tsc_10000      34.1651 (1.02)      36.5930 (1.0)       35.5276 (1.00)      0.5728 (1.07)      35.6691 (1.01)      0.6691 (1.06)          8;0  28.1471 (1.00)         29           1
    test_mdbm_preload_random_fetch_10000          34.8370 (1.04)      37.1509 (1.02)      35.6486 (1.01)      0.5368 (1.0)       35.6290 (1.01)      0.6291 (1.0)           8;1  28.0516 (0.99)         27           1
    test_kyotocabinet_random_fetch_10000          50.1349 (1.49)     315.4690 (8.62)      66.3761 (1.87)     60.3302 (112.39)    52.3400 (1.48)      1.3785 (2.19)          1;1  15.0657 (0.53)         19           1
    test_anydbm_random_fetch_10000                98.3920 (2.93)     127.4319 (3.48)     103.2393 (2.91)      8.6436 (16.10)    101.2516 (2.87)      3.1178 (4.96)          1;1   9.6862 (0.34)         10           1
    test_sqlite3_random_fetch_10000              179.9428 (5.35)     264.3309 (7.22)     198.3913 (5.60)     32.8237 (61.15)    183.5115 (5.20)     14.0412 (22.32)         1;1   5.0405 (0.18)          6           1
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

.. _random-key-selects-1:

100,000 Random Key SELECTs
~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    -------------------------------------------------------------------------------------------------- benchmark: 5 tests --------------------------------------------------------------------------------------------------
    Name (time in ms)                                    Min                   Max                  Mean            StdDev                Median               IQR            Outliers     OPS            Rounds  Iterations
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_preload_random_fetch_tsc_100000       351.3479 (1.0)        362.6180 (1.02)       358.0612 (1.01)     4.9707 (3.89)       360.7321 (1.02)     8.1980 (3.88)          1;0  2.7928 (0.99)          5           1
    test_mdbm_preload_random_fetch_100000           352.9408 (1.00)       360.9550 (1.01)       356.8196 (1.01)     3.2021 (2.51)       357.4481 (1.01)     5.0185 (2.38)          2;0  2.8025 (0.99)          5           1
    test_mdbm_random_fetch_100000                   353.4501 (1.01)       356.4832 (1.0)        354.6917 (1.0)      1.2767 (1.0)        354.3482 (1.0)      2.1121 (1.0)           1;0  2.8193 (1.0)           5           1
    test_kyotocabinet_random_fetch_100000           513.2129 (1.46)       516.0379 (1.45)       514.8367 (1.45)     1.3007 (1.02)       515.3730 (1.45)     2.3472 (1.11)          1;0  1.9424 (0.69)          5           1
    test_anydbm_random_fetch_100000               1,196.3558 (3.41)     1,217.2129 (3.41)     1,207.2943 (3.40)     7.5601 (5.92)     1,206.6510 (3.41)     8.3598 (3.96)          2;0  0.8283 (0.29)          5           1
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

.. _random-key-selects-2:

1,000,000 Random Key SELECTs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    --------------------------------------------------------------------------------------------- benchmark: 5 tests --------------------------------------------------------------------------------------------
    Name (time in s)                                   Min                Max               Mean            StdDev             Median               IQR            Outliers     OPS            Rounds  Iterations
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_preload_random_fetch_tsc_1000000      3.6708 (1.0)       3.7161 (1.0)       3.7020 (1.0)      0.0194 (1.29)      3.7138 (1.00)     0.0255 (1.71)          1;0  0.2701 (1.0)           5           1
    test_mdbm_preload_random_fetch_1000000          3.6781 (1.00)      3.7315 (1.00)      3.7045 (1.00)     0.0212 (1.41)      3.7021 (1.0)      0.0336 (2.25)          2;0  0.2699 (1.00)          5           1
    test_mdbm_random_fetch_1000000                  3.6957 (1.01)      3.7336 (1.00)      3.7079 (1.00)     0.0150 (1.0)       3.7054 (1.00)     0.0149 (1.0)           1;0  0.2697 (1.00)          5           1
    test_kyotocabinet_random_fetch_1000000          5.2549 (1.43)      5.2865 (1.42)      5.2677 (1.42)     0.0151 (1.01)      5.2599 (1.42)     0.0273 (1.82)          1;0  0.1898 (0.70)          5           1
    test_anydbm_random_fetch_1000000               12.3323 (3.36)     12.4784 (3.36)     12.4044 (3.35)     0.0586 (3.90)     12.3911 (3.35)     0.0927 (6.20)          2;0  0.0806 (0.30)          5           1
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Link
----

-  `Yahoo! MDBM <https://github.com/yahoo/mdbm>`__
-  `MDBM::Concept <http://yahoo.github.io/mdbm/guide/concepts.html>`__
-  `MDBM::Build <https://github.com/yahoo/mdbm/blob/master/README.build>`__
-  `MDBM::Document <http://yahoo.github.io/mdbm/>`__
-  `MDBM::FAQ <http://yahoo.github.io/mdbm/guide/faq.html>`__
-  `DBM <https://en.wikipedia.org/wiki/Dbm>`__
-  `MDBM::Macro(const) <http://yahoo.github.io/mdbm/api/mdbm_8h.html>`__
-  `Go-mdbm <https://github.com/torden/go-mdbm>`__
-  `PHP-mdbm <https://github.com/torden/php-mdbm>`__

--------------

Please feel free. I hope it is helpful for you

.. |Build Status| image:: https://travis-ci.org/torden/py-mdbm.svg?branch=master
   :target: https://travis-ci.org/torden/py-mdbm
.. |GitHub version| image:: https://badge.fury.io/gh/torden%2Fpy-mdbm.svg
   :target: https://badge.fury.io/gh/torden%2Fpy-mdbm
.. |GitHub version| image:: https://badge.fury.io/gh/yahoo%2Fmdbm.svg
   :target: https://badge.fury.io/gh/yahoo%2Fmdbm
