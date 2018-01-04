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
|[File Management](http://yahoo.github.io/mdbm/api/group__FileManagementGroup.html)||
|[Configuration](http://yahoo.github.io/mdbm/api/group__ConfigurationGroup.html)||
|[Record Access](http://yahoo.github.io/mdbm/api/group__RecordAccessGroup.html)||
|[Record Iteration](http://yahoo.github.io/mdbm/api/group__RecordIterationGroup.html)||
|[Locking](http://yahoo.github.io/mdbm/api/group__LockingGroup.html)||
|[Data Management](http://yahoo.github.io/mdbm/api/group__DataManagementGroup.html)||
|[Data Integrity](http://yahoo.github.io/mdbm/api/group__DataIntegrityGroup.html)||
|[Data Display](http://yahoo.github.io/mdbm/api/group__DataDisplayGroup.html)||
|[Statistics](http://yahoo.github.io/mdbm/api/group__StatisticsGroup.html)||
|[Cache and Backing Store](http://yahoo.github.io/mdbm/api/group__CacheAndBackingStoreGroup.html)||
|[Import and Export](http://yahoo.github.io/mdbm/api/group__ImportExportGroup.html)||
|[Miscellaneous](http://yahoo.github.io/mdbm/api/group__MiscellaneousGroup.html)||

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
|2.6.x ~ 2.7.x|yes|always|
|3.0.x ~ 3.x.x|as soon|N/A|

### MDBM

|*branch or release ver.*|*Support*|*Test*|*Comment*|
|---|---|---|---|
|master|yes|always|
|4.x|yes|always|


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
