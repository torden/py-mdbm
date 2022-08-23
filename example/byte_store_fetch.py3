#!/usr/bin/env python3
# encoding: utf-8
import mdbm
import random

print("[*] Creating and populating a database")

path = "/tmp/test1-byte.mdbm"
flags = mdbm.MDBM_O_RDWR
flags = flags | mdbm.MDBM_O_CREAT
flags = flags | mdbm.MDBM_LARGE_OBJECTS
flags = flags | mdbm.MDBM_ANY_LOCKS
flags = flags | mdbm.MDBM_O_TRUNC
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)

print("|--------|-------|")
print("|  key   |  val  |")
print("|--------|-------|")
for i in range(0, 10):
    k = bytes(str(i), 'utf-8')
    v = bytes(str(random.randrange(0, 65535)), 'utf-8')

    print("|%08s|%08s|" % (k, v))


    rv = dbm.store(k, v, mdbm.MDBM_INSERT|mdbm.MDBM_CACHE_MODIFY)
    if not rv:
        print("[-] failed to data store to ", path)
        break

for i in range(10, 20):
    k = str(i)
    v = str(random.randrange(0, 65535))

    print("|%08s|%08s|" % (k, v))


    rv = dbm.store(k, v, mdbm.MDBM_INSERT|mdbm.MDBM_CACHE_MODIFY)
    if not rv:
        print("[-] failed to data store to ", path)
        break


print("|--------|--------|")
print("[*] count of records : %d" % dbm.count_records())
print("\n")

dbm.close()


print("[*] Fetching records in-place")

path = "/tmp/test1-byte.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

with mdbm.open(path, flags, mode) as dbm:
    dbm.preload()

    print("|--------|--------------------|")
    print("|  key   |          val       |")
    print("|--------|--------------------|")

    for i in range(0, 20):

        k = bytes(str(i), 'utf-8')
        orgval = dbm.fetch(k)
        if not orgval:
            print("[-] failed to fetch value of %s in mdbm" % k)
            break

        print("|%08s|%20s|" % (k, orgval))

    print("|--------|--------------------|")
    print("[*] count of records : %d" % dbm.count_records())

print("done")
