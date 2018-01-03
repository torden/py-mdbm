import platform
import mdbm

print("This is Python " + platform.python_version())

dbm = mdbm.open("/tmp/test_py.mdbm", mdbm.MDBM_O_RDWR|mdbm.MDBM_O_CREAT|mdbm.MDBM_LARGE_OBJECTS|mdbm.MDBM_O_TRUNC|mdbm.MDBM_ANY_LOCKS, 0666, 0,0)

#print(dir(dbm))
#help(mdbm)
