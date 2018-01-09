#!/usr/bin/python
# encoding: utf-8
import mdbm
import random

print("[*] Iterating over all records")

path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)

print("|-------|-------|")
print("|  key  |  val  |")
print("|-------|-------|")

kv = dbm.first()

print("|%07s|%07s|" % kv)

while kv:

    print("|%07s|%07s|" % kv)

    kv = dbm.next()

print("|-------|-------|")
print("[*] count of records : %d" % dbm.count_records())

dbm.close()

print("done")
