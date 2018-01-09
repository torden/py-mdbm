#!/usr/bin/python
# encoding: utf-8
import mdbm
import random

print("[*] Fetching and updating records in-place")

path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)
for i in range(0, 65535):
    k = str(i)
    v = str(random.randrange(0, 65535))

    orgval = dbm.fetch(k)
    if not orgval:
        print("[-] failed to fetch value of %s in mdbm" % k)
        break

    print("[=] key(%s) : replace val(%s) to '%s' : " % (k, orgval, v)),

    rv = dbm.store(k, v, mdbm.MDBM_REPLACE)
    if not rv:
        print("FAIL")
        break
    print("DONE")

print("[*] count of records : %d" % dbm.count_records())

dbm.close()

print("done")
