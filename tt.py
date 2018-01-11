import mdbm 

rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_NONE)
print(rv)
rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_LFU)
print(rv)
rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_LRU)
print(rv)
rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_GDSF)
print(rv)
