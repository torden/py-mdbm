# Py-mdbm

- Py-mdbm is a Python binds to [Yahoo! MDBM C API.](https://github.com/yahoo/mdbm/)
- MDBM is a super-fast memory-mapped key/value store.
- MDBM is an ndbm work-alike hashed database library based on sdbm which is based on Per-Aake Larson’s Dynamic Hashing algorithms.
- MDBM is a high-performance, memory-mapped hash database similar to the homegrown libhash.
- The records stored in a mdbm database may have keys and values of arbitrary and variable lengths.

|Build Stats|Py-mdbm ver.|Y! mdbm ver.|
|:-:|:-:|:-:|
|[![Build Status](https://travis-ci.org/torden/py-mdbm.svg?branch=master)](https://travis-ci.org/torden/py-mdbm)|[![GitHub version](https://badge.fury.io/gh/torden%2Fpy-mdbm.svg)](https://badge.fury.io/gh/torden%2Fpy-mdbm)|[![GitHub version](https://badge.fury.io/gh/yahoo%2Fmdbm.svg)](https://badge.fury.io/gh/yahoo%2Fmdbm)|

## API

### Currently Supported APIs

the following is list of support api on now.

|Group|API|
|---|---|
|[File Management](http://yahoo.github.io/mdbm/api/group__FileManagementGroup.html)|mdbm_open, mdbm_close, mdbm_sync, mdbm_fsync, mdbm_close_fd, mdbm_replace_db, mdbm_replace_file, mdbm_dup_handle, mdbm_pre_split, mdbm_fcopy|
|[Configuration](http://yahoo.github.io/mdbm/api/group__ConfigurationGroup.html)|mdbm_get_version, mdbm_get_size, mdbm_get_page_size, mdbm_get_limit_size, mdbm_get_hash, mdbm_get_alignment, mdbm_set_alignment, mdbm_setspillsize, mdbm_limit_dir_size,  mdbm_get_magic_number, *~~mdbm_limit_size_v3, mdbm_set_window_size~~*|
|[Record Access](http://yahoo.github.io/mdbm/api/group__RecordAccessGroup.html)|mdbm_fetch, mdbm_delete, mdbm_store, mdbm_fetch_r, mdbm_fetch_dup_r, mdbm_delete_r, mdbm_store_r, mdbm_fetch_info|
|[Record Iteration](http://yahoo.github.io/mdbm/api/group__RecordIterationGroup.html)|mdbm_first, mdbm_next, mdbm_firstkey, mdbm_nextkey, mdbm_first_r, mdbm_next_r, mdbm_firstkey_r, mdbm_nextkey_r, *~~mdbm_iterate~~*|
|[Locking](http://yahoo.github.io/mdbm/api/group__LockingGroup.html)|mdbm_islocked, mdbm_isowned, mdbm_lock, mdbm_unlock, mdbm_lock_reset, mdbm_delete_lockfiles,  mdbm_get_lockmode, mdbm_trylock, mdbm_plock, mdbm_punlock, mdbm_tryplock, mdbm_lock_shared, mdbm_trylock_shared, mdbm_lock_smart, mdbm_trylock_smart, mdbm_unlock_smart|
|[Data Management](http://yahoo.github.io/mdbm/api/group__DataManagementGroup.html)|mdbm_compress_tree, mdbm_truncate, mdbm_purge, mdbm_clean, *~~mdbm_prune, mdbm_set_cleanfunc~~*|
|[Data Integrity](http://yahoo.github.io/mdbm/api/group__DataIntegrityGroup.html)|mdbm_check, mdbm_chk_all_page, mdbm_chk_page, mdbm_protect|
|[Data Display](http://yahoo.github.io/mdbm/api/group__DataDisplayGroup.html)|mdbm_dump_all_page, mdbm_dump_page|
|[Statistics](http://yahoo.github.io/mdbm/api/group__StatisticsGroup.html)|mdbm_count_records, mdbm_count_pages, mdbm_get_stats, mdbm_get_db_info, mdbm_get_stat_counter, mdbm_get_stat_time, mdbm_reset_stat_operations, mdbm_enable_stat_operations, mdbm_set_stat_time_func, *~~mdbm_get_stat_name, mdbm_set_stats_func, mdbm_chunk_iterate, mdbm_get_db_stats, mdbm_get_window_stats~~*|
|[Cache and Backing Store](http://yahoo.github.io/mdbm/api/group__CacheAndBackingStoreGroup.html)|mdbm_set_cachemode, mdbm_get_cachemode, mdbm_get_cachemode_name, *~~mdbm_set_backingstore~~*|
|[Import and Export](http://yahoo.github.io/mdbm/api/group__ImportExportGroup.html)|*~~mdbm_cdbdump_to_file, mdbm_cdbdump_trailer_and_close, mdbm_cdbdump_add_record, mdbm_dbdump_to_file, mdbm_dbdump_trailer_and_close, mdbm_dbdump_add_record, mdbm_dbdump_export_header, mdbm_dbdump_import_header, mdbm_dbdump_import, mdbm_cdbdump_import~~*|
|[Miscellaneous](http://yahoo.github.io/mdbm/api/group__MiscellaneousGroup.html)|mdbm_preload, mdbm_get_errno,  mdbm_get_page, mdbm_lock_pages, mdbm_unlock_pages, mdbm_get_hash_value|

### Deprecated APIs

|*API*|*STATUS*|*COMMENT*|
|---|---|---|
|mdbm_save|DEPRECATED|mdbm_save is only supported for V2 MDBMs.|
|mdbm_restore|DEPRECATED|mdbm_restore is only supported for V2 MDBMs.|
|mdbm_sethash|DEPRECATED|Legacy version of mdbm_set_hash() This function has inconsistent naming, an error return value. It will be removed in a future version.|

### Only a V2 implementation

|*API*|*STATUS*|*COMMENT*|
|---|---|---|
|mdbm_stat_all_page|V3 not supported|There is only a V2 implementation. V3 not currently supported.|
|mdbm_stat_header|V3 not supported|There is only a V2 implementation. V3 not currently supported.|

### Has not been implemented

|*API*|*STATUS*|*COMMENT*|
|---|---|---|
|dbm_chk_error|Not Implemented|This has not been implemented|


## Support two compatibility version

### Python

|*Version*|*Support*|*Test*|
|---|---|---|
|Python 2.6.x ~ 2.7.x|yes|always|
|Python 3.0.x ~ 3.x.x|yes|always|
|PyPy|yes|always|
|PyPy3|yes|always|

### MDBM

|*branch or release ver.*|*Support*|*Test*|*Comment*|
|---|---|---|---|
|master|yes|always|
|4.x|yes|always|

## Install

### MDBM

- Ubuntu : See the [pre-build packages](https://github.com/torden/go-mdbm/tree/master/pkg)
- RHEL (CentOS) : See the [pre-build packages](https://github.com/torden/go-mdbm/tree/master/pkg)

### py-mdbm (use pip)

```shell
pip install py-mdbm
```

### py-mdbm (use source)

#### Download

```shell
git clone https://github.com/torden/py-mdbm
```

#### Build and Test

```shell
cd py-mdbm
CMD_PYTHON=`which python` make
```

### Check

```shell
$ python
>>> import mdbm
>>> help(mdbm)
```

### Benchmark

```shell
cd py-mdbm
`which pip` install -r requirements.txt
CMD_PYTHON=`which python` CMD_PYTEST=`which pytest` make benchmark
```

## Example

See the [Source Code](https://github.com/torden/py-mdbm/tree/master/example) for more details

The following is Sample codes for a first look at py-mdbm

### Creating and populating a database

#### Python 2 or higher

```python
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
```

#### Python 3 or higher

```python
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
```


### Fetching records in-place

```python
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
```

### Fetching and updating records in-place

```python
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
```

### Deleting records in-place

```python
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
```

### Iterating over all records

```python
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
```

### Iterating over all keys

```python
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
```

### Iteration over all value by key

```python
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
```

## Benchmark

The following is results of Py-mdbm vs AnyDBM vs SQLite3 vs Kyotocabinet benchmarks for simple data storing and random fetching in them.

See the [Source Code](https://github.com/torden/py-mdbm/blob/master/test_benchmark.py) for more details

### Spec

#### Host

|Type|Spec|
|---|---|
|CPU|Inte i-7|
|RAM|DDR4 32G|
|HDD|Nvme M.2 SSD|

#### VM

|Type|Spec|
|---|---|
|Machine|VM(VirtualBox)|
|OS|CentOS 7 64bit|
|CPU|2 vCore|
|RAM|8G|
|AnyDBM|Berkeley DB (Hash, version 9, native byte-order) format|
|Mdbm|893f7a8 on 26 Jul, MDBM V3 format|
|SQLite|V3|
|Kyotocabinet|1.2.76, kch|

### Command

```
CMD_PYTHON=`which python` CMD_PYTEST=`which pytest` make benchmark
```

### 10,000 INSERTs

```
platform linux2 -- Python 2.7.14, pytest-3.3.2, py-1.5.2, pluggy-0.6.0
benchmark: 3.1.1 (defaults: timer=time.time disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)
rootdir: /root/PERSONAL/py-mdbm, inifile:
plugins: benchmark-3.1.1
collected 21 items

------------------------------------------------------------------------------------------- benchmark: 5 tests ------------------------------------------------------------------------------------------
Name (time in ms)                          Min                 Max                Mean            StdDev              Median               IQR            Outliers      OPS            Rounds  Iterations
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_mdbm_store_tsc_10000              38.3699 (1.0)       42.6970 (1.0)       40.1762 (1.0)      0.8295 (1.0)       40.1430 (1.0)      0.9634 (1.64)          4;1  24.8904 (1.0)          24           1
test_mdbm_store_10000                  39.0320 (1.02)      42.9850 (1.01)      40.2414 (1.00)     0.8308 (1.00)      40.1490 (1.00)     0.5884 (1.0)           5;3  24.8500 (1.00)         25           1
test_sqlite3_store_10000               50.0350 (1.30)      64.6431 (1.51)      52.4234 (1.30)     3.1417 (3.79)      51.6200 (1.29)     1.4859 (2.53)          1;1  19.0754 (0.77)         19           1
test_kyotocabinet_kch_store_10000      61.0192 (1.59)      65.7201 (1.54)      62.8013 (1.56)     1.3162 (1.59)      62.4061 (1.55)     1.4217 (2.42)          6;1  15.9232 (0.64)         17           1
test_anydbm_store_10000               121.6981 (3.17)     124.4709 (2.92)     122.5444 (3.05)     0.8690 (1.05)     122.5522 (3.05)     0.9117 (1.55)          1;1   8.1603 (0.33)          9           1
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```

### 100,000 INSERTs

```
------------------------------------------------------------------------------------------------ benchmark: 5 tests -----------------------------------------------------------------------------------------------
Name (time in ms)                             Min                   Max                  Mean             StdDev                Median                IQR            Outliers     OPS            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_mdbm_store_tsc_100000               422.9810 (1.0)        448.0500 (1.0)        436.4583 (1.0)      11.6168 (2.58)       438.9038 (1.0)      21.9153 (3.99)          1;0  2.2912 (1.0)           5           1
test_mdbm_store_100000                   453.1040 (1.07)       538.0430 (1.20)       486.5464 (1.11)     31.8692 (7.08)       475.0891 (1.08)     33.7510 (6.15)          2;0  2.0553 (0.90)          5           1
test_sqlite3_store_100000                478.4930 (1.13)       490.1061 (1.09)       482.7990 (1.11)      4.4992 (1.0)        482.0240 (1.10)      5.4923 (1.0)           1;0  2.0713 (0.90)          5           1
test_kyotocabinet_kch_store_100000       534.4710 (1.26)       566.1159 (1.26)       550.9701 (1.26)     13.6158 (3.03)       552.2079 (1.26)     24.2882 (4.42)          2;0  1.8150 (0.79)          5           1
test_anydbm_store_100000               1,648.0649 (3.90)     1,761.5669 (3.93)     1,691.5490 (3.88)     44.5776 (9.91)     1,674.3729 (3.81)     58.6830 (10.68)         1;0  0.5912 (0.26)          5           1
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```


### 10,000 Random Key SELECTs

```
---------------------------------------------------------------------------------------------------------- benchmark: 6 tests ----------------------------------------------------------------------------------------------------------
Name (time in ms)                                     Min                     Max                    Mean                StdDev                  Median                   IQR            Outliers      OPS            Rounds  Iterations
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_mdbm_preload_random_fetch_tsc_10000          32.3410 (1.0)           39.9480 (1.16)          34.4072 (1.03)         1.7823 (4.12)          33.9930 (1.02)         1.4995 (2.63)          6;3  29.0637 (0.97)         28           1
test_mdbm_random_fetch_10000                      32.6140 (1.01)          35.2380 (1.02)          33.5960 (1.00)         0.5980 (1.38)          33.4665 (1.00)         0.8099 (1.42)          7;1  29.7655 (1.00)         30           1
test_mdbm_preload_random_fetch_10000              32.7070 (1.01)          34.4172 (1.0)           33.4585 (1.0)          0.4321 (1.0)           33.3650 (1.0)          0.5710 (1.0)           7;0  29.8878 (1.0)          30           1
test_kyotocabinet_random_fetch_10000              49.7329 (1.54)          52.3691 (1.52)          50.7634 (1.52)         0.7441 (1.72)          50.7050 (1.52)         1.2895 (2.26)          8;0  19.6992 (0.66)         20           1
test_anydbm_random_fetch_10000                   112.9920 (3.49)         118.2418 (3.44)         114.6708 (3.43)         1.4869 (3.44)         114.6572 (3.44)         1.1405 (2.00)          2;1   8.7206 (0.29)          9           1
test_sqlite3_random_fetch_10000              109,115.6809 (>1000.0)  115,857.6381 (>1000.0)  114,274.0032 (>1000.0)  2,895.5717 (>1000.0)  115,345.3960 (>1000.0)  2,082.3945 (>1000.0)       1;1   0.0088 (0.00)          5           1
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```


### 100,000 Random Key SELECTs

```
--------------------------------------------------------------------------------------------------- benchmark: 5 tests ---------------------------------------------------------------------------------------------------
Name (time in ms)                                    Min                   Max                  Mean             StdDev                Median                IQR            Outliers     OPS            Rounds  Iterations
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_mdbm_preload_random_fetch_tsc_100000       375.0720 (1.0)        381.3601 (1.0)        378.6532 (1.0)       2.4191 (1.0)        379.1029 (1.0)       3.3982 (1.10)          2;0  2.6409 (1.0)           5           1
test_mdbm_preload_random_fetch_100000           379.4649 (1.01)       389.4479 (1.02)       385.2309 (1.02)      4.4767 (1.85)       387.2790 (1.02)      7.7794 (2.51)          1;0  2.5958 (0.98)          5           1
test_mdbm_random_fetch_100000                   380.7530 (1.02)       387.1801 (1.02)       382.7838 (1.01)      2.6155 (1.08)       382.2269 (1.01)      3.0940 (1.0)           1;0  2.6124 (0.99)          5           1
test_kyotocabinet_random_fetch_100000           533.9031 (1.42)       563.3259 (1.48)       544.8388 (1.44)     11.4614 (4.74)       542.0711 (1.43)     14.4702 (4.68)          1;0  1.8354 (0.69)          5           1
test_anydbm_random_fetch_100000               1,300.4990 (3.47)     1,359.2131 (3.56)     1,335.3496 (3.53)     23.5258 (9.72)     1,345.5169 (3.55)     33.8326 (10.94)         2;0  0.7489 (0.28)          5           1
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
```

## Link

- [Yahoo! MDBM](https://github.com/yahoo/mdbm)
- [MDBM::Concept](http://yahoo.github.io/mdbm/guide/concepts.html)
- [MDBM::Build](https://github.com/yahoo/mdbm/blob/master/README.build)
- [MDBM::Document](http://yahoo.github.io/mdbm/)
- [MDBM::FAQ](http://yahoo.github.io/mdbm/guide/faq.html)
- [DBM](https://en.wikipedia.org/wiki/Dbm)
- [MDBM::Macro(const)](http://yahoo.github.io/mdbm/api/mdbm_8h.html)
- [Go-mdbm](https://github.com/torden/go-mdbm)
- [PHP-mdbm](https://github.com/torden/php-mdbm)

---
Please feel free. I hope it is helpful for you
