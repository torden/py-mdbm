#!/usr/bin/env python
# encoding: utf-8
import mdbm
import random
from time import sleep

print("[*] Random Fetching records in-place")

path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)
dbm.preload()

print("|-------|-------|")
print("|  key  |  val  |")
print("|-------|-------|")

keylist = []
for i in range(0, 10):

    k = str(random.randrange(0, 65534))
    orgval = dbm.fetch(k)
    if not orgval:
        print("[-] failed to fetch value of %s in mdbm" % k)
        break

    print("|%07s|%07s|" % (k, orgval))

print("|-------|-------|")
print("[=] count of records : %d" % dbm.count_records())


print("[*] Random Fetching record's information in-place")
print("|-------|-------|----------------|-------------|-------------------------|------------------------|-------|")
print("|  key  |  val  | iter.m_page_no | iter.m_next | info.cache_num_accesses | info.cache_access_time | flags |")
print("|-------|-------|----------------|-------------|-------------------------|------------------------|-------|")


for i in range(0,10):

    k = str(random.randrange(0, 65534))
    mval = dbm.fetch_info(k)
    print("|%07s|%07s|%16d|%13d|%25d|%24d|%07d|" % \
        (k, mval['val'], mval['iter']['m_pageno'], mval['iter']['m_next'], \
        mval['info']['cache_num_accesses'], mval['info']['cache_access_time'], mval['info']['flags']))


print("|-------|-------|----------------|-------------|-------------------------|------------------------|-------|")


dbm.close()

print("done")
