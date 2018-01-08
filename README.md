# Py-mdbm

*Unfortunately, Not ready for use.*

- MDBM is a super-fast memory-mapped key/value store.
- MDBM is an ndbm work-alike hashed database library based on sdbm which is based on Per-Aake Larson’s Dynamic Hashing algorithms.
- MDBM is a high-performance, memory-mapped hash database similar to the homegrown libhash.
- The records stored in a mdbm database may have keys and values of arbitrary and variable lengths.

|Build Stats|PHP-mdbm ver.|Y! mdbm ver.|
|:-:|:-:|:-:|
|[![Build Status](https://travis-ci.org/torden/py-mdbm.svg?branch=master)](https://travis-ci.org/torden/py-mdbm)|[![GitHub version](https://badge.fury.io/gh/torden%2Fpy-mdbm.svg)](https://badge.fury.io/gh/torden%2Fpy-mdbm)|[![GitHub version](https://badge.fury.io/gh/yahoo%2Fmdbm.svg)](https://badge.fury.io/gh/yahoo%2Fmdbm)|

## API

### Currently Supported APIs

the following is list of support api on now.

|Group|API|
|---|---|
|[File Management](http://yahoo.github.io/mdbm/api/group__FileManagementGroup.html)|mdbm_open, mdbm_close, mdbm_sync, mdbm_fsync, mdbm_close_fd, *~~mdbm_replace_db, mdbm_replace_file, mdbm_dup_handle,  mdbm_pre_split, mdbm_fcopy~~*|
|[Configuration](http://yahoo.github.io/mdbm/api/group__ConfigurationGroup.html)|mdbm_get_version, mdbm_get_size, mdbm_get_page_size, mdbm_get_limit_size, mdbm_get_hash, *~~mdbm_get_magic_number, mdbm_setspillsize, mdbm_get_alignment, mdbm_set_alignment, mdbm_limit_size_v3, mdbm_limit_dir_size, mdbm_set_window_size~~*|
|[Record Access](http://yahoo.github.io/mdbm/api/group__RecordAccessGroup.html)|mdbm_fetch, mdbm_delete, mdbm_store, *~~mdbm_fetch_r~~, ~~mdbm_fetch_buf~~, ~~mdbm_fetch_dup_r~~, ~~mdbm_fetch_str~~, ~~mdbm_fetch_info~~, ~~mdbm_delete_r~~, ~~mdbm_delete_str~~, ~~mdbm_store_r~~, ~~mdbm_store_str~~*|
|[Record Iteration](http://yahoo.github.io/mdbm/api/group__RecordIterationGroup.html)|mdbm_first, mdbm_next, mdbm_firstkey, mdbm_nextkey, *~~mdbm_first_r~~, ~~mdbm_next_r~~, ~~mdbm_firstkey_r~~, ~~mdbm_nextkey_r~~, ~~mdbm_iterate~~*|
|[Locking](http://yahoo.github.io/mdbm/api/group__LockingGroup.html)|mdbm_islocked, mdbm_isowned, mdbm_lock, mdbm_unlock, mdbm_lock_reset, mdbm_delete_lockfiles,  mdbm_get_lockmode, mdbm_trylock, mdbm_plock, mdbm_punlock, mdbm_tryplock, mdbm_lock_shared, mdbm_trylock_shared, mdbm_lock_smart, mdbm_trylock_smart, mdbm_unlock_smart|
|[Data Management](http://yahoo.github.io/mdbm/api/group__DataManagementGroup.html)|mdbm_compress_tree, mdbm_truncate, mdbm_purge,  *~~mdbm_prune, mdbm_set_cleanfunc, mdbm_clean~~*|
|[Data Integrity](http://yahoo.github.io/mdbm/api/group__DataIntegrityGroup.html)|mdbm_check, mdbm_chk_all_page, mdbm_chk_page, mdbm_protect, *~~mdbm_chk_error~~*|
|[Data Display](http://yahoo.github.io/mdbm/api/group__DataDisplayGroup.html)|*~~mdbm_dump_all_page~~, ~~mdbm_dump_page~~*|
|[Statistics](http://yahoo.github.io/mdbm/api/group__StatisticsGroup.html)|mdbm_count_records, mdbm_count_pages, *~~mdbm_get_stat_counter, mdbm_get_stat_time, mdbm_reset_stat_operations, mdbm_enable_stat_operations, mdbm_set_stat_time_func, mdbm_get_stat_name, mdbm_set_stats_func, mdbm_get_stats, mdbm_get_db_info, mdbm_chunk_iterate, mdbm_get_db_stats, mdbm_get_window_stats~~*|
|[Cache and Backing Store](http://yahoo.github.io/mdbm/api/group__CacheAndBackingStoreGroup.html)|*~~mdbm_set_cachemode, mdbm_get_cachemode, mdbm_get_cachemode_name, mdbm_set_backingstore~~*|
|[Import and Export](http://yahoo.github.io/mdbm/api/group__ImportExportGroup.html)|*~~mdbm_cdbdump_to_file, mdbm_cdbdump_trailer_and_close, mdbm_cdbdump_add_record, mdbm_dbdump_to_file, mdbm_dbdump_trailer_and_close, mdbm_dbdump_add_record, mdbm_dbdump_export_header, mdbm_dbdump_import_header, mdbm_dbdump_import, mdbm_cdbdump_import~~*|
|[Miscellaneous](http://yahoo.github.io/mdbm/api/group__MiscellaneousGroup.html)|mdbm_preload, mdbm_get_errno,  mdbm_get_page, mdbm_lock_pages, mdbm_unlock_pages, *~~mdbm_get_hash_value~~*|

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

### py-mdbm

#### Download

```shell
git clone https://github.com/torden/py-mdbm
```

#### Build and Test

```shell
cd py-mdbm
pip install -r requirements.txt
CMD_PYTHON=/app/python/bin/python make
```

## Example


## Benchmark

The following is result of Py-mdbm vs AnyDBM benchmarks for simple data storing and random fetching in them.

### Spec

#### Host

|Type|Spec|
|---|---|
|CPU|I7|
|RAM|32G DDR4|
|HDD|Nvme|

#### VM

|Type|Spec|
|---|---|
|Machine|VM(VirtualBox)|
|OS|Ubuntu 17.10 (Artful Aardvark)|
|CPU|2 vCore|
|RAM|8G|
|AnyDBM|Berkeley DB (Hash, version 9, native byte-order)|
|Mdbm|893f7a8 on 26 Jul, MDBM V3|

### Command

```
make benchmark
```

### Output

```
platform linux2 -- Python 2.7.14, pytest-3.1.3, py-1.4.34, pluggy-0.4.0
benchmark: 3.1.1 (defaults: timer=time.time disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)

------------------------------------------------------------------------------------ benchmark: 2 tests -----------------------------------------------------------------------------------
Name (time in ms)               Min                Max               Mean            StdDev             Median               IQR            Outliers      OPS            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_mdbm_store_10000       15.6021 (1.0)      20.7560 (1.0)      16.6336 (1.0)      0.8858 (1.0)      16.3540 (1.0)      0.7806 (1.0)          13;4  60.1192 (1.0)          60           1
test_anydbm_store_10000     47.2810 (3.03)     59.7410 (2.88)     49.5850 (2.98)     2.8609 (3.23)     48.9625 (2.99)     1.1770 (1.51)          2;2  20.1674 (0.34)         20           1
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------- benchmark: 2 tests --------------------------------------------------------------------------------------
Name (time in ms)                 Min                 Max                Mean             StdDev              Median                IQR            Outliers     OPS            Rounds  Iterations
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
test_mdbm_store_100000       162.7200 (1.0)      186.0912 (1.0)      171.2356 (1.0)       8.1884 (1.0)      168.6914 (1.0)       7.4978 (1.0)           2;1  5.8399 (1.0)           6           1
test_anydbm_store_100000     777.6020 (4.78)     875.8821 (4.71)     825.2219 (4.82)     38.2175 (4.67)     828.3558 (4.91)     57.7620 (7.70)          2;0  1.2118 (0.21)          5           1
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
