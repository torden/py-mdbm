import random
import unittest
import mdbm
import pprint
# import os
pp = pprint.PrettyPrinter(indent=4)


def initDefaultData(dbm):
    # warming up
    for i in range(0, 100):
        k = str(i)
        v = str(random.randint(0, 65535))
        dbm.store(k, v, mdbm.MDBM_REPLACE)

    for i in range(0, 100):
        v = str(random.randint(0, 65535))
        dbm.fetch(k)

    v = str(random.randint(0, 2))
    dbm.delete(k)


def printNotice():
    print('\n\x1b[6;33;44m[*] this following message is not error\x1b[0m\n')


class TestMDBMMethods(unittest.TestCase):

    def setUp(self):
        self.path = "/tmp/test_py.mdbm"
        self.path_pure = "/tmp/test_py_pure.mdbm"
        self.path_split = "/tmp/test_py_split.mdbm"
        self.path_prot = "/tmp/test_py_protect.mdbm"
        flags = mdbm.MDBM_O_RDWR
        flags = flags | mdbm.MDBM_O_CREAT
        flags = flags | mdbm.MDBM_LARGE_OBJECTS
        flags = flags | mdbm.MDBM_ANY_LOCKS
        flags = flags | mdbm.MDBM_O_TRUNC
        self.flags = flags
        self.mode = 0o644  # means 0644
        self.dbm = mdbm.open(self.path, flags, self.mode, 0, 0)
        self.dbm.enable_stat_operations(mdbm.MDBM_STATS_BASIC | mdbm.MDBM_STATS_TIMED)
        self.dbm.set_stat_time_func(mdbm.MDBM_CLOCK_TSC)
        initDefaultData(self.dbm)

    def tearDown(self):
        self.dbm.sync()
        self.dbm.close()

    def test_01_open(self):
        flags = mdbm.MDBM_O_RDONLY
        dbm = mdbm.open(self.path, flags, self.mode, 0, 0)
        self.assertTrue(dbm)

#    def test_01_dup_handle(self):
#        dbm_dup = self.dbm.dup_handle()
#        self.assertTrue(dbm_dup)
#        dbm_dup.close()

    def test_99_purge(self):
        self.dbm.purge()

    def test_99_truncate(self):
        self.dbm.truncate()

    def test_02_store_fetch(self):

        k = str(1)
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        k = str(2)
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        k = str(3)
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.fetch(str(1))
        self.assertTrue(rv, "rv=%s" % rv)

    def test_02_store_r_fetch_r(self):
        k = "02"
        v = str(random.randint(0, 65535))
        rv = self.dbm.store_r(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.fetch_r(k)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_02_fetch_dup_r(self):

        k = "02"
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_INSERT_DUP)
        self.assertTrue(rv, "rv=%s" % rv)

        k = "02"
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_INSERT_DUP)
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.fetch_dup_r(k)
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.fetch_dup_r(k)
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.fetch_dup_r(k)
        self.assertFalse(rv, "rv=%s" % rv)

    def test_02_fetch_info(self):

        k = str(1)
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        k = str(2)
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        k = str(3)
        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.fetch(str(1))
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.fetch(str(2))
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.fetch(str(1))
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.fetch_info(str(1))
        self.assertTrue(rv, "rv=%s" % rv)

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

    def test_07_get_hash(self):
        rv = self.dbm.get_hash()
        self.assertEqual(rv, 5)

    def test_08_first(self):
        kv = self.dbm.first()
        self.assertTrue(kv)

    def test_08_next(self):
        kv = self.dbm.next()
        self.assertTrue(kv)

    def test_09_firstkey(self):
        key = self.dbm.firstkey()
        self.assertTrue(key)

    def test_09_nextkey(self):
        key = self.dbm.nextkey()
        self.assertTrue(key)

    def test_10_first_r(self):
        key = self.dbm.first_r()
        self.assertTrue(key)

    def test_10_next_r(self):
        key = self.dbm.next_r()
        self.assertTrue(key)

    def test_11_firstkey_r(self):
        key = self.dbm.firstkey_r()
        self.assertTrue(key)

    def test_11_nextkey_r(self):
        key = self.dbm.nextkey_r()
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
        rv = self.dbm.check(mdbm.MDBM_CHECK_HEADER, False)
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.check(mdbm.MDBM_CHECK_CHUNKS, False)
        self.assertTrue(rv, "rv=%s" % rv)
        rv = self.dbm.check(mdbm.MDBM_CHECK_DIRECTORY, False)
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.check(mdbm.MDBM_CHECK_ALL, False)
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

        k = str(random.randint(0, 65535))
        v = str(random.randint(0, 65535))
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

    def test_81_lock_pages_unlock_pages(self):
        rv = self.dbm.lock_pages()
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        rv = self.dbm.unlock_pages()
        self.assertTrue((rv >= 0))

    def test_81_plock_punlock(self):
        k = str(random.randint(0, 65535))

        rv = self.dbm.plock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.punlock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_81_tryplock_punlock(self):
        k = str(random.randint(0, 65535))

        rv = self.dbm.tryplock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.punlock(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_81_lock_smart_unlock_smart(self):
        k = str(random.randint(0, 65535))

        rv = self.dbm.lock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.unlock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_81_trylock_smart_unlock_smart(self):
        k = str(random.randint(0, 65535))

        rv = self.dbm.trylock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

        rv = self.dbm.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randint(0, 65535))
        rv = self.dbm.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm.unlock_smart(k, mdbm.MDBM_O_RDWR)
        self.assertTrue((rv >= 0))

    def test_81_protect(self):

        flags = mdbm.MDBM_O_RDWR | mdbm.MDBM_PROTECT
        self.dbm_prot = mdbm.open(self.path, flags, self.mode, 0, 0)

        k = str(random.randint(0, 65535))

        rv = self.dbm_prot.protect(mdbm.MDBM_PROT_READ)
        self.assertTrue((rv >= 0))

        rv = self.dbm_prot.get_lockmode()
        self.assertTrue((rv >= 0))

        v = str(random.randint(0, 65535))
        rv = self.dbm_prot.store(k, v, mdbm.MDBM_REPLACE)
        self.assertTrue(rv, "failred to store, \
        rv=%d, key=%s, val=%s" % (rv, k, v))

        rv = self.dbm_prot.protect(mdbm.MDBM_PROT_WRITE)
        self.assertTrue((rv >= 0))

        rv = self.dbm_prot.fetch(str(1))
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm_prot.protect(mdbm.MDBM_PROT_NONE)
        self.assertTrue((rv >= 0))

        self.dbm_prot.close()

    def test_82_lock_delete_unlock(self):
        rv = self.dbm.lock()
        self.assertTrue(rv, "rv=%s" % rv)

        rv = self.dbm.delete(str(1))
        self.assertTrue(rv, "failred to delete")

        rv = self.dbm.unlock()
        self.assertTrue(rv, "rv=%s" % rv)

    def test_82_delete_r(self):
        mrv = self.dbm.fetch_r(str(1))
        self.assertTrue(mrv)

        rv = self.dbm.delete_r(mrv['iter'])
        self.assertTrue(rv)

        mrv = self.dbm.fetch_r(str(1))
        self.assertTrue(rv, "failed to deleted use iter")

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

#    # segfault
#    def test_90_get_cachemode_name(self):
#        rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_NONE)
#        self.assertEqual(rv, "none")
#        rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_LFU)
#        self.assertEqual(rv, "LFU")
#        rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_LRU)
#        self.assertEqual(rv, "LRU")
#        rv = mdbm.get_cachemode_name(mdbm.MDBM_CACHEMODE_GDSF)
#        self.assertEqual(rv, "GDSF")

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

    def test_get_hash_value(self):
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_CRC32)
        self.assertEqual(rv, 3278106577)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_EJB)
        self.assertEqual(rv, 115485)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_PHONG)
        self.assertEqual(rv, 2730050143)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_OZ)
        self.assertEqual(rv, 1071549504)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_TOREK)
        self.assertEqual(rv, 1618211232)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_FNV)
        self.assertEqual(rv, 3381919419)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_STL)
        self.assertEqual(rv, 266133472)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_MD5)
        self.assertEqual(rv, 1243268166)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_SHA_1)
        self.assertEqual(rv, 4033019161)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_JENKINS)
        self.assertEqual(rv, 2081026389)
        rv = mdbm.get_hash_value("I hope this will be helpful for you", mdbm.MDBM_HASH_HSIEH)
        self.assertEqual(rv, 4291882138)

    def test_99_alignment(self):
        rv = self.dbm.get_alignment()
        self.assertEqual(rv, mdbm.MDBM_ALIGN_8_BITS)

    def test_99_setsplillsize(self):
        rv = self.dbm.setspillsize(1024)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_99_limit_dir_size(self):
        rv = self.dbm.limit_dir_size(1024)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_99_get_magic_number(self):
        rv = self.dbm.get_magic_number()
        # print(mdbm._MDBM_MAGIC) # v2
        # print(mdbm._MDBM_MAGIC_NEW) # v2 with large objects
        # print(mdbm._MDBM_MAGIC_NEW2) # v3
        self.assertEqual(rv, mdbm.MDBM_MAGIC)

    def test_99_pre_split(self):
        dbm2 = mdbm.open(self.path_split, self.flags, self.mode)
        rv = dbm2.pre_split(512)
        self.assertTrue(rv, "rv=%s" % rv)

    def test_99_set_cachemode_get_cachemode(self):

        dbm_pure = mdbm.open(self.path_pure, self.flags, self.mode, 0, 0)
        rv = dbm_pure.set_cachemode(mdbm.MDBM_CACHEMODE_NONE)
        self.assertTrue(rv, "rv=%s" % rv)
        rv = dbm_pure.get_cachemode()
        self.assertEqual(rv, mdbm.MDBM_CACHEMODE_NONE)

        rv = dbm_pure.set_cachemode(mdbm.MDBM_CACHEMODE_LFU)
        self.assertTrue(rv, "rv=%s" % rv)
        rv = dbm_pure.get_cachemode()
        self.assertEqual(rv, mdbm.MDBM_CACHEMODE_LFU)

        rv = dbm_pure.set_cachemode(mdbm.MDBM_CACHEMODE_LRU)
        self.assertTrue(rv, "rv=%s" % rv)
        rv = dbm_pure.get_cachemode()
        self.assertEqual(rv, mdbm.MDBM_CACHEMODE_LRU)

        rv = dbm_pure.set_cachemode(mdbm.MDBM_CACHEMODE_GDSF)
        self.assertTrue(rv, "rv=%s" % rv)
        rv = dbm_pure.get_cachemode()
        self.assertEqual(rv, mdbm.MDBM_CACHEMODE_GDSF)

    def test_999_dump_all_page(self):
        printNotice()
        self.dbm.dump_all_page()

    def test_999_dump_page(self):
        printNotice()
        self.dbm.dump_page(0)

    def test_999_fcopy(self):
        rv = self.dbm.fcopy("/tmp/test_fcopied.mdbm")
        self.assertTrue(rv)

    def test_999_get_stat_counter(self):
        rv = self.dbm.get_stat_counter(mdbm.MDBM_STAT_TYPE_FETCH)
        self.assertTrue(rv)
        rv = self.dbm.get_stat_counter(mdbm.MDBM_STAT_TYPE_STORE)
        self.assertTrue(rv)
        rv = self.dbm.get_stat_counter(mdbm.MDBM_STAT_TYPE_DELETE)
        self.assertTrue(rv)

    def test_999_get_stat_time(self):
        rv = self.dbm.get_stat_time(mdbm.MDBM_STAT_TYPE_FETCH)
        self.assertTrue(rv)
        rv = self.dbm.get_stat_time(mdbm.MDBM_STAT_TYPE_STORE)
        self.assertTrue(rv)
        rv = self.dbm.get_stat_time(mdbm.MDBM_STAT_TYPE_DELETE)
        self.assertTrue(rv)

    def test_999_get_stats(self):
        rv = self.dbm.get_stats()
        self.assertTrue(rv)

    def test_999_get_db_stats(self):
        rv = self.dbm.get_db_stats(mdbm.MDBM_ITERATE_NOLOCK)
        self.assertTrue(rv)
        printNotice()
        pp.pprint(rv)

    def test_999_get_window_stats(self):
        rv = self.dbm.get_window_stats()
        self.assertTrue(rv)
        printNotice()
        pp.pprint(rv)

    def test_999_select_log(self):
        rv = self.dbm.select_log_plugin(mdbm.MDBM_LOG_TO_STDERR)
        self.assertTrue(rv)

        rv = self.dbm.select_log_plugin(mdbm.MDBM_LOG_TO_SYSLOG)
        self.assertTrue(rv)

        rv = self.dbm.select_log_plugin(mdbm.MDBM_LOG_TO_FILE)
        self.assertTrue(rv)

        rv = self.dbm.set_log_filename("/tmp/test_py.log")
        self.assertTrue(rv)
 
    def test_999_zzz_last_reset_stat_operations(self):
        self.dbm.reset_stat_operations()  # none

    def test_999_misc(self):
        pass

if __name__ == '__main__':
    unittest.main()
