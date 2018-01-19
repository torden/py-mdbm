#!/usr/bin/env python
# encoding: utf-8
from __future__ import print_function
import mdbm
import random

print("[*] Deleting records in-place")

path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)

print("|---------|---------|---------------|-----------|")
print("|   key   |   val   | iter.m_pageno | iter.next |")
print("|---------|---------|---------------|-----------|")

for i in range(0, 10):

    k = str(random.randrange(0, 65534))
    mval = dbm.fetch_r(k)

    if mval:
        print("|%9s|%9s|%15d|%11d|" % (k, mval['val'], mval['iter']['m_pageno'], mval['iter']['m_next']))

    rv = dbm.delete_r(mval['iter'])
    if not rv:
        print("failed to delete an record")
    else:
        v = dbm.fetch(k)
        if v:
            print("failed to deleted an record")

print("|---------|---------|---------------|-----------|")
print("[*] count of records : %d" % dbm.count_records())

dbm.close()
print("done")
