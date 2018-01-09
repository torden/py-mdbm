#!/usr/bin/python
# encoding: utf-8
import mdbm
import random


path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
flags = flags | mdbm.MDBM_O_CREAT
flags = flags | mdbm.MDBM_LARGE_OBJECTS
flags = flags | mdbm.MDBM_ANY_LOCKS
flags = flags | mdbm.MDBM_O_TRUNC
mode = 0o644  # means 0644
dbm = mdbm.open(path, flags, mode, 0, 0)

for i in range(0, 65535):
    k = str(i)
    v = str(random.randrange(0, 65535))

    rv = dbm.store(k, v, mdbm.MDBM_INSERT)
    if not rv:
        print("failed to data store to ", path)
        break

print("count of records : ", dbm.get_count_records())

dbm.close()

print("done")
