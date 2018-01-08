import pytest
import mdbm
import anydbm
import random

def mdbm_large_store(limit):
    path = "/tmp/test_py_benchmark1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    flags = flags | mdbm.MDBM_O_CREAT
    flags = flags | mdbm.MDBM_LARGE_OBJECTS
    flags = flags | mdbm.MDBM_ANY_LOCKS
    flags = flags | mdbm.MDBM_O_TRUNC
    mode = 0o644 #means 0644
    dbm = mdbm.open(path, flags, mode, 0, 0)
    dbm.log_minlevel(mdbm.MDBM_LOG_EMERGENCY) #required !! it while running on pytest-performance

    for i in range(0, limit):
        k = str(i)
        v = str(random.randrange(0, 65535))
       
        rv = dbm.store(k, v, mdbm.MDBM_REPLACE)
        if rv == False:
            return False
      
    dbm.close()
    return True

def anydbm_large_store(limit):
    path = "/tmp/test_py_benchmark1.dbm"

    db = anydbm.open(path, 'n')
    for i in range(0, limit):
        k = str(i)
        v = str(random.randrange(0, 65535))
        try:
            db[k] = v
        except:
            return False

    db.close()
   
    return True

def anydbm_cache_large_store(limit):
    path = "cache"

    db = anydbm.open(path, 'n')
    for i in range(0, limit):
        k = str(i)
        v = str(random.randrange(0, 65535))
        try:
            db[k] = v
        except:
            return False

    db.close()
   
    return True

@pytest.mark.loop_10000
def test_mdbm_store_10000(benchmark):
    result = benchmark(mdbm_large_store, 10000)
    assert result == True

@pytest.mark.loop_10000
def test_anydbm_store_10000(benchmark):
    result = benchmark(anydbm_large_store, 10000)
    assert result == True

@pytest.mark.loop_10000
def test_anydbm_cache_store_10000(benchmark):
    result = benchmark(anydbm_cache_large_store, 10000)
    assert result == True


@pytest.mark.loop_100000
def test_mdbm_store_100000(benchmark):
    result = benchmark(mdbm_large_store, 100000)
    assert result == True

@pytest.mark.loop_100000
def test_anydbm_store_100000(benchmark):
    result = benchmark(anydbm_large_store, 100000)
    assert result == True

@pytest.mark.loop_100000
def test_anydbm_cache_store_100000(benchmark):
    result = benchmark(anydbm_large_store, 100000)
    assert result == True
