#!/usr/bin/env python
# encoding: utf-8
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
