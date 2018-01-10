#!/usr/bin/env python3
# encoding: utf-8
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
