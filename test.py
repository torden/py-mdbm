import random
import unittest
import mdbm


class TestMDBMMethods(unittest.TestCase):

    def setUp(self):
        path = "/tmp/test_py.mdbm"
        flags = mdbm.MDBM_O_RDWR
        flags = flags | mdbm.MDBM_O_CREAT
        flags = flags | mdbm.MDBM_LARGE_OBJECTS
        flags = flags | mdbm.MDBM_ANY_LOCKS

        self.dbm = mdbm.open(path, flags, 0666, 0, 0)

    def tearDown(self):
        self.dbm.sync()
        self.dbm.close()

    def test_01_open(self):
        pass

    def test_99_purge(self):
        self.dbm.purge()

    def test_99_truncate(self):
        self.dbm.truncate()

    def test_02_store(self):

        k = str(1)
        v = str(random.randrange(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

    def test_03_sync(self):
        rv = self.dbm.sync()
        self.assertTrue(rv, "failed to sync")

    def test_03_fsync(self):
        rv = self.dbm.fsync()
        self.assertTrue(rv, "failed to fsync")

    def test_04_compress_tree(self):
        self.dbm.compress_tree()

    def test_05_preload(self):
        rv = self.dbm.preload()
        self.assertTrue(rv)

    def test_06_fetch(self):
        rv = self.dbm.fetch(str(1))
        self.assertTrue(rv, "rv=%s" % rv)

    def test_07_get_hash(self):
        rv = self.dbm.get_hash()
        self.assertEquals(rv, 5)

    def test_08_first(self):
        kv = self.dbm.first()
        self.assertTrue(kv)

    def test_09_next(self):
        kv = self.dbm.next()
        self.assertTrue(kv)

    def test_10_firstkey(self):
        key = self.dbm.firstkey()
        self.assertTrue(key)

    def test_11_nextkey(self):
        key = self.dbm.nextkey()
        self.assertTrue(key)

    def test_80_lock_unlock(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_81_lock_store_unlock(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.fetch(str(1))
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_81_lock_fetch_unlock(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        k = str(random.randrange(0, 65535))
        v = str(random.randrange(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_99_lock_delete_unlock(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.delete(str(1))
        self.assertTrue(rv, "failred to delete")

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_90_get_alignment(self):
        rv = self.dbm.get_alignment()
        self.assertNotEqual(rv, "rv=%s" % rv)

    def test_90_get_limit_size(self):
        rv = self.dbm.get_limit_size()
        self.assertTrue((rv >= 0))

    def test_90_get_version(self):
        rv = self.dbm.get_version()
        self.assertEquals(rv, 3)

    def test_90_get_size(self):
        rv = self.dbm.get_size()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_90_get_page_size(self):
        rv = self.dbm.get_page_size()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_999_misc(self):
        pass

if __name__ == '__main__':
    unittest.main()
