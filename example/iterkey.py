#!/usr/bin/env python
# encoding: utf-8
import mdbm
import random

print("[*] Iterating over all records")

path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)

print("|-------|")
print("|  key  |")
print("|-------|")

k = dbm.firstkey()

print("|%07s|" % k)

while k:

    print("|%07s|" % k)

    k = dbm.nextkey()

print("|-------|")
print("[*] count of records : %d" % dbm.count_records())

dbm.close()

print("done")
