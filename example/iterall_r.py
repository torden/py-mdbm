#!/usr/bin/env python
# encoding: utf-8
import mdbm
# import random

print("[*] Iterating over all records")

path = "/tmp/test1.mdbm"
flags = mdbm.MDBM_O_RDWR
mode = 0o644  # means 0644

dbm = mdbm.open(path, flags, mode)

maxloop = 10

print("[+] Uses the Global Iter")
print("|-------|-------|--------|------|")
print("|  key  |  val  | pageno | next |")
print("|-------|-------|--------|------|")

mval = dbm.first_r()

loop = 0
while mval:

    print("|%07s|%07s|%8d|%6d|" % (mval['key'], mval['val'], mval['iter']['m_pageno'], mval['iter']['m_next']))
    loop = loop + 1
    if loop > maxloop:
        break
    mval = dbm.next_r()

print("|-------|-------|--------|------|")
print("")
print("[+] Uses the Local Iter")
print("|-------|-------|--------|------|")
print("|  key  |  val  | pageno | next |")
print("|-------|-------|--------|------|")


localiter = dbm.init_iter()
mval = dbm.first_r(localiter)

loop = 0
while mval:

    print("|%07s|%07s|%8d|%6d|" % (mval['key'], mval['val'], mval['iter']['m_pageno'], mval['iter']['m_next']))
    loop = loop + 1
    if loop > maxloop:
        break

    mval = dbm.next_r(mval['iter'])

print("|-------|-------|--------|------|")
print("[*] count of records : %d" % dbm.count_records())


dbm.close()

print("done")
