import pytest
import mdbm
import anydbm
import sqlite3
from tinydb import TinyDB, Query
from kyotocabinet import *
import random, os, sys

"""
attrs==17.4.0
cx-Oracle==5.2
docutils==0.14
funcsigs==1.0.2
httpstat==1.1.1
intelhex==2.0
Kyoto-Cabinet==1.1
pathlib==1.0.1
pluggy==0.6.0
py==1.5.2
py-cpuinfo==3.3.0
pytest==3.3.2
pytest-benchmark==3.1.1
PyYAML==3.11
redis==2.10.5
six==1.11.0
statistics==1.0.3.5
tinydb==3.7.0
virtualenv==13.1.2
"""

def mdbm_store(limit):
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
       
        rv = dbm.store(k, v, mdbm.MDBM_INSERT)
        if rv == False:
            return False
      
    dbm.close()
    return True

def mdbm_fetch(limit):
    path = "/tmp/test_py_benchmark1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644 #means 0644
    dbm = mdbm.open(path, flags, mode, 0, 0)
    dbm.log_minlevel(mdbm.MDBM_LOG_EMERGENCY) #required !! it while running on pytest-performance

    for i in range(0, limit):
        k = str(random.randrange(0, limit-1))
        v = dbm.fetch(k)
        if len(v) < 1:
            return False
      
    dbm.close()
    return True

def mdbm_fetch_after_preload(limit):
    path = "/tmp/test_py_benchmark1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644 #means 0644
    dbm = mdbm.open(path, flags, mode, 0, 0)
    dbm.log_minlevel(mdbm.MDBM_LOG_EMERGENCY) #required !! it while running on pytest-performance

    dbm.preload()

    for i in range(0, limit):
        k = str(random.randrange(0, limit-1))
        v = dbm.fetch(k)
        if len(v) < 1:
            return False
      
    dbm.close()
    return True



def anydbm_store(limit):
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

def anydbm_fetch(limit):
    path = "/tmp/test_py_benchmark1.dbm"

    db = anydbm.open(path, 'r')
    for i in range(0, limit):
        k = str(random.randrange(0, limit-1))
        try:
            v = db[k]
            if len(v) < 1:
                print(k,v)
                False
        except:
            return False

    db.close()
   
    return True


def kyotocabinet_store(limit):
    path = "/tmp/test_py_benchmark1.kch"

    db = DB()
    flags = DB.OWRITER
    flags = flags | DB.OCREATE
    flags = flags | DB.OTRUNCATE
    flags = flags | DB.OTRYLOCK
    if not db.open(path, flags):
        return False

    for i in range(0, limit):
        k = str(i)
        v = str(random.randrange(0, 65535))
 
        rv = db.set(k,v)
        if rv == False:
            return False
      
    db.close()
    return True

def kyotocabinet_fetch(limit):
    path = "/tmp/test_py_benchmark1.kch"

    db = DB()
    flags = DB.OWRITER
    if not db.open(path, flags):
        return False

    for i in range(0, limit):
        k = str(random.randrange(0, limit-1))
 
        v = db.get(k)
        if len(v) < 1:
            return False
      
    db.close()
    return True


def sqlite3_store(limit):
    path = "/tmp/test_py_benchmark1.db"

    try:
        os.remove(path)
    except OSError:
        pass

    try:
        conn = sqlite3.connect(path)
        c = conn.cursor()
        c.execute('''CREATE TABLE benchmark (key text, val text)''')

        data = []
        for i in range(0, limit):
            k = str(i)
            v = str(random.randrange(0, 65535))

            # Very slowly..
            #c.executeii("INSERT INTO benchmark VALUES (?,?)", (k,v,))
            data.append((k,v))

        c.executemany("INSERT INTO benchmark VALUES (?,?)", data)
        conn.commit()
        conn.close()
    except:
        print "Unexpected error:", sys.exc_info()
        return False
    return True

def sqlite3_fetch(limit):
    path = "/tmp/test_py_benchmark1.db"

    try:
        conn = sqlite3.connect(path)
        c = conn.cursor()

        for i in range(0, limit):
            k = str(random.randrange(0, limit-1))

            c.execute('SELECT val FROM benchmark WHERE key = ?', (k,))
            if len(c.fetchone()[0]) < 1:
                return False
        conn.close()
    except:
        print "Unexpected error:", sys.exc_info()
        return False
    return True



def tinydb_store(limit):
    path = "/tmp/test_py_benchmark1.json"

    try:
        os.remove(path)
    except OSError:
        pass

    try:
        db = TinyDB(path)

        for i in range(0, limit):
            k = str(i)
            v = str(random.randrange(0, 65535))
            db.insert({"key":k, "val":v})
    except:
        print "Unexpected error:", sys.exc_info()
        return False
    return True

def tinydb_fetch(limit):
    path = "/tmp/test_py_benchmark1.json"

    try:
        db = TinyDB(path)
        perf = Query()

        for i in range(0, limit):
            k = str(random.randrange(0, limit-1))

            v = db.search(perf.key == k)
            if len(v) < 1:
                return False
    except:
        print "Unexpected error:", sys.exc_info()
        return False
    return True

# 10000 -----------------------------------------------
@pytest.mark.store_loop_10000
def test_mdbm_store_10000(benchmark):
    result = benchmark(mdbm_store, 10000)
    assert result == True

@pytest.mark.store_loop_10000
def test_anydbm_store_10000(benchmark):
    result = benchmark(anydbm_store, 10000)
    assert result == True

@pytest.mark.store_loop_10000
def test_kyotocabinet_kch_store_10000(benchmark):
    result = benchmark(kyotocabinet_store, 10000)
    assert result == True

""" FAIL
@pytest.mark.store_loop_10000
def test_tinydb_store_10000(benchmark):
    result = benchmark(tinydb_store, 10000)
    assert result == True
"""

@pytest.mark.store_loop_10000
def test_sqlite3_store_10000(benchmark):
    result = benchmark(sqlite3_store, 10000)
    assert result == True

# 100000 -----------------------------------------------
@pytest.mark.store_loop_100000
def test_mdbm_store_100000(benchmark):
    result = benchmark(mdbm_store, 100000)
    assert result == True

@pytest.mark.store_loop_100000
def test_anydbm_store_100000(benchmark):
    result = benchmark(anydbm_store, 100000)
    assert result == True

@pytest.mark.store_loop_100000
def test_kyotocabinet_kch_store_100000(benchmark):
    result = benchmark(kyotocabinet_store, 100000)
    assert result == True

""" FAIL
@pytest.mark.store_loop_100000
def test_tinydb_store_100000(benchmark):
    result = benchmark(tinydb_store, 100000)
    assert result == True
"""

@pytest.mark.store_loop_100000
def test_sqlite3_store_100000(benchmark):
    result = benchmark(sqlite3_store, 100000)
    assert result == True


# 10000 -----------------------------------------------
@pytest.mark.random_fetch_loop_10000
def test_mdbm_random_fetch_10000(benchmark):
    result = benchmark(mdbm_fetch, 10000)
    assert result == True

@pytest.mark.random_fetch_loop_10000
def test_mdbm_preload_random_fetch_10000(benchmark):
    result = benchmark(mdbm_fetch_after_preload, 10000)
    assert result == True

@pytest.mark.random_fetch_loop_10000
def test_anydbm_random_fetch_10000(benchmark):
    result = benchmark(anydbm_fetch, 10000)
    assert result == True

@pytest.mark.random_fetch_loop_10000
def test_kyotocabinet_random_fetch_10000(benchmark):
    result = benchmark(kyotocabinet_fetch, 10000)
    assert result == True

""" FAIL
@pytest.mark.random_fetch_loop_10000
def test_tinydb_random_fetch_10000(benchmark):
    result = benchmark(tinydb_fetch, 10000)
    assert result == True
"""

@pytest.mark.random_fetch_loop_10000
def test_sqlite3_random_fetch_10000(benchmark):
    result = benchmark(sqlite3_fetch, 10000)
    assert result == True

# 100000 -----------------------------------------------
@pytest.mark.random_fetch_loop_100000
def test_mdbm_random_fetch_100000(benchmark):
    result = benchmark(mdbm_fetch, 100000)
    assert result == True

@pytest.mark.random_fetch_loop_100000
def test_mdbm_preload_random_fetch_100000(benchmark):
    result = benchmark(mdbm_fetch_after_preload, 100000)
    assert result == True

@pytest.mark.random_fetch_loop_100000
def test_anydbm_random_fetch_100000(benchmark):
    result = benchmark(anydbm_fetch, 100000)
    assert result == True

@pytest.mark.random_fetch_loop_100000
def test_kyotocabinet_random_fetch_100000(benchmark):
    result = benchmark(kyotocabinet_fetch, 100000)
    assert result == True

""" FAIL
@pytest.mark.random_fetch_loop_100000
def test_tinydb_random_fetch_100000(benchmark):
    result = benchmark(tinydb_fetch, 100000)
    assert result == True
""" 

""" FAIL
read(8, "\0\0\0\2\0\0\7H\0\0\0\0\0\0\0\0", 16) = 16
fstat(8, {st_mode=S_IFREG|0644, st_size=1908736, ...}) = 0
access("/tmp/test_py_benchmark1.db-wal", F_OK) = -1 ENOENT (No such file or directory)
fstat(8, {st_mode=S_IFREG|0644, st_size=1908736, ...}) = 0
fcntl(8, F_SETLK, {type=F_UNLCK, whence=SEEK_SET, start=0, len=0}) = 0
fcntl(8, F_SETLK, {type=F_RDLCK, whence=SEEK_SET, start=1073741824, len=1}) = 0
fcntl(8, F_SETLK, {type=F_RDLCK, whence=SEEK_SET, start=1073741826, len=510}) = 0
fcntl(8, F_SETLK, {type=F_UNLCK, whence=SEEK_SET, start=1073741824, len=1}) = 0
access("/tmp/test_py_benchmark1.db-journal", F_OK) = -1 ENOENT (No such file or directory)
fstat(8, {st_mode=S_IFREG|0644, st_size=1908736, ...}) = 0
lseek(8, 24, SEEK_SET)                  = 24

@pytest.mark.random_fetch_loop_100000
def test_sqlite3_random_fetch_100000(benchmark):
    result = benchmark(sqlite3_fetch, 100000)
    assert result == True
"""

