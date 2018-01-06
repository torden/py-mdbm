import random
import unittest
import mdbm


class TestMDBMMethods(unittest.TestCase):

    def setUp(self):
        self.path = "/tmp/test_py.mdbm"
        flags = mdbm.MDBM_O_RDWR
        flags = flags | mdbm.MDBM_O_CREAT
        flags = flags | mdbm.MDBM_LARGE_OBJECTS
        flags = flags | mdbm.MDBM_ANY_LOCKS
        mode = 0o644
        self.dbm = mdbm.open(self.path, flags, mode, 0, 0)
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

        k = str(2)
        v = str(random.randrange(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        k = str(3)
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
        self.assertEqual(rv, 5)

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

    def test_12_count_records(self):
        cnt = self.dbm.count_records()
        self.assertTrue((cnt >= 1))

    def test_13_count_pages(self):
        cnt = self.dbm.count_records()
        self.assertTrue((cnt >= 1))

    def test_14_get_page(self):
        rv = self.dbm.get_page(str(1))
        self.assertTrue((rv >= 0))

    def test_15_get_errno(self):
        rv = self.dbm.get_errno()
        self.assertTrue((rv == 0))

    def test_70_check(self):
        rv = self.dbm.check(10, False)
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.check(10, True)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_71_check_page(self):
        rv = self.dbm.chk_page(0)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_72_check_all_page(self):
        rv = self.dbm.chk_all_page()
        self.assertTrue(rv, "rv=%s" % rv)

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

    def test_81_get_lockmode(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_81_trylock(self):
        rv = self.dbm.trylock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_81_lock_shared(self):
        rv = self.dbm.lock_shared()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_81_trylock_shared(self):
        rv = self.dbm.trylock_shared()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_81_trylock_shared(self):
        rv = self.dbm.trylock_shared()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_81_lock_pages_unlock_pages(self):
        rv = self.dbm.lock_pages()
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        rv = self.dbm.unlock_pages()
        self.assertTrue((rv >= 0))

    def test_81_plock_punlock(self):
        k = str(random.randrange(0, 65535))

        rv = self.dbm.plock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randrange(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.punlock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_81_tryplock_punlock(self):
        k = str(random.randrange(0, 65535))

        rv = self.dbm.tryplock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randrange(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.punlock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_81_lock_smart_unlock_smart(self):
        k = str(random.randrange(0, 65535))

        rv = self.dbm.lock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randrange(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.unlock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_81_trylock_smart_unlock_smart(self):
        k = str(random.randrange(0, 65535))

        rv = self.dbm.trylock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randrange(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.unlock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_82_lock_delete_unlock(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.delete(str(1))
        self.assertTrue(rv, "failred to delete")

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_83_islocked(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.islocked()
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_84_islocked(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.isowned()
        self.assertTrue(rv, "rv=%s" % rv)
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
        self.assertEqual(rv, 3)

    def test_90_get_size(self):
        rv = self.dbm.get_size()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_90_get_page_size(self):
        rv = self.dbm.get_page_size()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_98_lock_reset(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.lock_reset(self.path)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_99_delete_lockfiles(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.delete_lockfiles(self.path)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_999_misc(self):
        pass

if __name__ == '__main__':
    unittest.main()
