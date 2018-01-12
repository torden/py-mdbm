#!/usr/bin/env python
# encoding: utf-8
import mdbm
import random

print("[*] Fetching records in-place")

path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)
dbm.preload()

print("|-------|-------|")
print("|  key  |  val  |")
print("|-------|-------|")

for i in range(0, 10):

    k = str(random.randrange(0, 65534))
    orgval = dbm.fetch(k)
    if not orgval:
        print("[-] failed to fetch value of %s in mdbm" % k)
        break

    print("|%07s|%07s|" % (k, orgval))

print("|-------|-------|")
print("[*] count of records : %d" % dbm.count_records())

for i in range(0, 10):

    k = str(random.randrange(0, 65534))
    mval = dbm.fetch_info(k)
    print(mval)


dbm.close()

print("done")
