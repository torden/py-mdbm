import platform
import random
import pprint
import mdbm

pp = pprint.PrettyPrinter(indent=4)
print("This is Python " + platform.python_version())

pp.pprint(dir(mdbm))

dbm = mdbm.open("/tmp/test_py.mdbm", mdbm.MDBM_O_RDWR|mdbm.MDBM_O_CREAT|mdbm.MDBM_LARGE_OBJECTS|mdbm.MDBM_O_TRUNC|mdbm.MDBM_ANY_LOCKS, 0666, 0,0)

pp.pprint("* MDBM *")
pp.pprint(dir(mdbm))
pp.pprint("* DBM *")
pp.pprint(dir(dbm))

dbm.purge()
dbm.truncate()

for i in range(0,100):
    k = str(i)
    v = str(random.randint(1, 65535))
    rv = dbm.store(k,v)
    #pp.print(rv)


v = str(random.randint(1, 100))
print("key=%s, val=%s" % (v,dbm.fetch(v)));


dbm.compress_tree()

dbm.sync()
dbm.fsync()
dbm.close()

print("done")

#help(mdbm)
