Py-mdbm
=======

-  Py-mdbm is a Python binds to `Yahoo! MDBM C
   API. <https://github.com/yahoo/mdbm/>`__
-  MDBM is a super-fast memory-mapped key/value store.
-  MDBM is an ndbm work-alike hashed database library based on sdbm
   which is based on Per-Aake Larson’s Dynamic Hashing algorithms.
-  MDBM is a high-performance, memory-mapped hash database similar to
   the homegrown libhash.
-  The records stored in a mdbm database may have keys and values of
   arbitrary and variable lengths.

+------------------+--------------------+--------------------+
| Build Stats      | Py-mdbm ver.       | Y! mdbm ver.       |
+==================+====================+====================+
| |Build Status|   | |GitHub version|   | |GitHub version|   |
+------------------+--------------------+--------------------+

API
---

Currently Supported APIs
~~~~~~~~~~~~~~~~~~~~~~~~

the following is list of support api on now.

+------+------+
| Grou | API  |
| p    |      |
+======+======+
| `Fil | mdbm |
| e    | \_op |
| Mana | en,  |
| geme | mdbm |
| nt < | \_cl |
| http | ose, |
| ://y | mdbm |
| ahoo | \_sy |
| .git | nc,  |
| hub. | mdbm |
| io/m | \_fs |
| dbm/ | ync, |
| api/ | mdbm |
| grou | \_cl |
| p__F | ose\ |
| ileM | _fd, |
| anag | mdbm |
| emen | \_re |
| tGro | plac |
| up.h | e\_d |
| tml> | b,   |
| `__  | mdbm |
|      | \_re |
|      | plac |
|      | e\_f |
|      | ile, |
|      | mdbm |
|      | \_du |
|      | p\_h |
|      | andl |
|      | e,   |
|      | mdbm |
|      | \_pr |
|      | e\_s |
|      | plit |
|      | ,    |
|      | mdbm |
|      | \_fc |
|      | opy  |
+------+------+
| `Con | mdbm |
| figu | \_ge |
| rati | t\_v |
| on < | ersi |
| http | on,  |
| ://y | mdbm |
| ahoo | \_ge |
| .git | t\_s |
| hub. | ize, |
| io/m | mdbm |
| dbm/ | \_ge |
| api/ | t\_p |
| grou | age\ |
| p__C | _siz |
| onfi | e,   |
| gura | mdbm |
| tion | \_ge |
| Grou | t\_l |
| p.ht | imit |
| ml>` | \_si |
| __   | ze,  |
|      | mdbm |
|      | \_ge |
|      | t\_h |
|      | ash, |
|      | mdbm |
|      | \_ge |
|      | t\_a |
|      | lign |
|      | ment |
|      | ,    |
|      | mdbm |
|      | \_se |
|      | t\_a |
|      | lign |
|      | ment |
|      | ,    |
|      | mdbm |
|      | \_se |
|      | tspi |
|      | llsi |
|      | ze,  |
|      | mdbm |
|      | \_li |
|      | mit\ |
|      | _dir |
|      | \_si |
|      | ze,  |
|      | mdbm |
|      | \_ge |
|      | t\_m |
|      | agic |
|      | \_nu |
|      | mber |
|      | ,    |
|      | *[ST |
|      | RIKE |
|      | OUT: |
|      | mdbm |
|      | \_li |
|      | mit\ |
|      | _siz |
|      | e\_v |
|      | 3,   |
|      | mdbm |
|      | \_se |
|      | t\_w |
|      | indo |
|      | w\_s |
|      | ize] |
|      | *    |
+------+------+
| `Rec | mdbm |
| ord  | \_fe |
| Acce | tch, |
| ss < | mdbm |
| http | \_de |
| ://y | lete |
| ahoo | ,    |
| .git | mdbm |
| hub. | \_st |
| io/m | ore, |
| dbm/ | mdbm |
| api/ | \_fe |
| grou | tch\ |
| p__R | _r,  |
| ecor | mdbm |
| dAcc | \_fe |
| essG | tch\ |
| roup | _dup |
| .htm | \_r, |
| l>`_ | mdbm |
| _    | \_de |
|      | lete |
|      | \_r, |
|      | mdbm |
|      | \_st |
|      | ore\ |
|      | _r,  |
|      | mdbm |
|      | \_fe |
|      | tch\ |
|      | _inf |
|      | o    |
+------+------+
| `Rec | mdbm |
| ord  | \_fi |
| Iter | rst, |
| atio | mdbm |
| n <h | \_ne |
| ttp: | xt,  |
| //ya | mdbm |
| hoo. | \_fi |
| gith | rstk |
| ub.i | ey,  |
| o/md | mdbm |
| bm/a | \_ne |
| pi/g | xtke |
| roup | y,   |
| __Re | mdbm |
| cord | \_fi |
| Iter | rst\ |
| atio | _r,  |
| nGro | mdbm |
| up.h | \_ne |
| tml> | xt\_ |
| `__  | r,   |
|      | mdbm |
|      | \_fi |
|      | rstk |
|      | ey\_ |
|      | r,   |
|      | mdbm |
|      | \_ne |
|      | xtke |
|      | y\_r |
|      | ,    |
|      | *[ST |
|      | RIKE |
|      | OUT: |
|      | mdbm |
|      | \_it |
|      | erat |
|      | e]*  |
+------+------+
| `Loc | mdbm |
| king | \_is |
|  <ht | lock |
| tp:/ | ed,  |
| /yah | mdbm |
| oo.g | \_is |
| ithu | owne |
| b.io | d,   |
| /mdb | mdbm |
| m/ap | \_lo |
| i/gr | ck,  |
| oup_ | mdbm |
| _Loc | \_un |
| king | lock |
| Grou | ,    |
| p.ht | mdbm |
| ml>` | \_lo |
| __   | ck\_ |
|      | rese |
|      | t,   |
|      | mdbm |
|      | \_de |
|      | lete |
|      | \_lo |
|      | ckfi |
|      | les, |
|      | mdbm |
|      | \_ge |
|      | t\_l |
|      | ockm |
|      | ode, |
|      | mdbm |
|      | \_tr |
|      | yloc |
|      | k,   |
|      | mdbm |
|      | \_pl |
|      | ock, |
|      | mdbm |
|      | \_pu |
|      | nloc |
|      | k,   |
|      | mdbm |
|      | \_tr |
|      | yplo |
|      | ck,  |
|      | mdbm |
|      | \_lo |
|      | ck\_ |
|      | shar |
|      | ed,  |
|      | mdbm |
|      | \_tr |
|      | yloc |
|      | k\_s |
|      | hare |
|      | d,   |
|      | mdbm |
|      | \_lo |
|      | ck\_ |
|      | smar |
|      | t,   |
|      | mdbm |
|      | \_tr |
|      | yloc |
|      | k\_s |
|      | mart |
|      | ,    |
|      | mdbm |
|      | \_un |
|      | lock |
|      | \_sm |
|      | art  |
+------+------+
| `Dat | mdbm |
| a    | \_co |
| Mana | mpre |
| geme | ss\_ |
| nt < | tree |
| http | ,    |
| ://y | mdbm |
| ahoo | \_tr |
| .git | unca |
| hub. | te,  |
| io/m | mdbm |
| dbm/ | \_pu |
| api/ | rge, |
| grou | mdbm |
| p__D | \_cl |
| ataM | ean, |
| anag | *[ST |
| emen | RIKE |
| tGro | OUT: |
| up.h | mdbm |
| tml> | \_pr |
| `__  | une, |
|      | mdbm |
|      | \_se |
|      | t\_c |
|      | lean |
|      | func |
|      | ]*   |
+------+------+
| `Dat | mdbm |
| a    | \_ch |
| Inte | eck, |
| grit | mdbm |
| y <h | \_ch |
| ttp: | k\_a |
| //ya | ll\_ |
| hoo. | page |
| gith | ,    |
| ub.i | mdbm |
| o/md | \_ch |
| bm/a | k\_p |
| pi/g | age, |
| roup | mdbm |
| __Da | \_pr |
| taIn | otec |
| tegr | t    |
| ityG |      |
| roup |      |
| .htm |      |
| l>`_ |      |
| _    |      |
+------+------+
| `Dat | mdbm |
| a    | \_du |
| Disp | mp\_ |
| lay  | all\ |
| <htt | _pag |
| p:// | e,   |
| yaho | mdbm |
| o.gi | \_du |
| thub | mp\_ |
| .io/ | page |
| mdbm |      |
| /api |      |
| /gro |      |
| up__ |      |
| Data |      |
| Disp |      |
| layG |      |
| roup |      |
| .htm |      |
| l>`_ |      |
| _    |      |
+------+------+
| `Sta | mdbm |
| tist | \_co |
| ics  | unt\ |
| <htt | _rec |
| p:// | ords |
| yaho | ,    |
| o.gi | mdbm |
| thub | \_co |
| .io/ | unt\ |
| mdbm | _pag |
| /api | es,  |
| /gro | mdbm |
| up__ | \_ge |
| Stat | t\_s |
| isti | tats |
| csGr | ,    |
| oup. | mdbm |
| html | \_ge |
| >`__ | t\_d |
|      | b\_i |
|      | nfo, |
|      | mdbm |
|      | \_ge |
|      | t\_s |
|      | tat\ |
|      | _cou |
|      | nter |
|      | ,    |
|      | mdbm |
|      | \_ge |
|      | t\_s |
|      | tat\ |
|      | _tim |
|      | e,   |
|      | mdbm |
|      | \_re |
|      | set\ |
|      | _sta |
|      | t\_o |
|      | pera |
|      | tion |
|      | s,   |
|      | mdbm |
|      | \_en |
|      | able |
|      | \_st |
|      | at\_ |
|      | oper |
|      | atio |
|      | ns,  |
|      | mdbm |
|      | \_se |
|      | t\_s |
|      | tat\ |
|      | _tim |
|      | e\_f |
|      | unc, |
|      | *[ST |
|      | RIKE |
|      | OUT: |
|      | mdbm |
|      | \_ge |
|      | t\_s |
|      | tat\ |
|      | _nam |
|      | e,   |
|      | mdbm |
|      | \_se |
|      | t\_s |
|      | tats |
|      | \_fu |
|      | nc,  |
|      | mdbm |
|      | \_ch |
|      | unk\ |
|      | _ite |
|      | rate |
|      | ,    |
|      | mdbm |
|      | \_ge |
|      | t\_d |
|      | b\_s |
|      | tats |
|      | ,    |
|      | mdbm |
|      | \_ge |
|      | t\_w |
|      | indo |
|      | w\_s |
|      | tats |
|      | ]*   |
+------+------+
| `Cac | mdbm |
| he   | \_se |
| and  | t\_c |
| Back | ache |
| ing  | mode |
| Stor | ,    |
| e <h | mdbm |
| ttp: | \_ge |
| //ya | t\_c |
| hoo. | ache |
| gith | mode |
| ub.i | ,    |
| o/md | mdbm |
| bm/a | \_ge |
| pi/g | t\_c |
| roup | ache |
| __Ca | mode |
| cheA | \_na |
| ndBa | me,  |
| ckin | *[ST |
| gSto | RIKE |
| reGr | OUT: |
| oup. | mdbm |
| html | \_se |
| >`__ | t\_b |
|      | acki |
|      | ngst |
|      | ore] |
|      | *    |
+------+------+
| `Imp | *[ST |
| ort  | RIKE |
| and  | OUT: |
| Expo | mdbm |
| rt < | \_cd |
| http | bdum |
| ://y | p\_t |
| ahoo | o\_f |
| .git | ile, |
| hub. | mdbm |
| io/m | \_cd |
| dbm/ | bdum |
| api/ | p\_t |
| grou | rail |
| p__I | er\_ |
| mpor | and\ |
| tExp | _clo |
| ortG | se,  |
| roup | mdbm |
| .htm | \_cd |
| l>`_ | bdum |
| _    | p\_a |
|      | dd\_ |
|      | reco |
|      | rd,  |
|      | mdbm |
|      | \_db |
|      | dump |
|      | \_to |
|      | \_fi |
|      | le,  |
|      | mdbm |
|      | \_db |
|      | dump |
|      | \_tr |
|      | aile |
|      | r\_a |
|      | nd\_ |
|      | clos |
|      | e,   |
|      | mdbm |
|      | \_db |
|      | dump |
|      | \_ad |
|      | d\_r |
|      | ecor |
|      | d,   |
|      | mdbm |
|      | \_db |
|      | dump |
|      | \_ex |
|      | port |
|      | \_he |
|      | ader |
|      | ,    |
|      | mdbm |
|      | \_db |
|      | dump |
|      | \_im |
|      | port |
|      | \_he |
|      | ader |
|      | ,    |
|      | mdbm |
|      | \_db |
|      | dump |
|      | \_im |
|      | port |
|      | ,    |
|      | mdbm |
|      | \_cd |
|      | bdum |
|      | p\_i |
|      | mpor |
|      | t]*  |
+------+------+
| `Mis | mdbm |
| cell | \_pr |
| aneo | eloa |
| us < | d,   |
| http | mdbm |
| ://y | \_ge |
| ahoo | t\_e |
| .git | rrno |
| hub. | ,    |
| io/m | mdbm |
| dbm/ | \_ge |
| api/ | t\_p |
| grou | age, |
| p__M | mdbm |
| isce | \_lo |
| llan | ck\_ |
| eous | page |
| Grou | s,   |
| p.ht | mdbm |
| ml>` | \_un |
| __   | lock |
|      | \_pa |
|      | ges, |
|      | mdbm |
|      | \_ge |
|      | t\_h |
|      | ash\ |
|      | _val |
|      | ue   |
+------+------+

Deprecated APIs
~~~~~~~~~~~~~~~

+------+------+------+
| *API | *STA | *COM |
| *    | TUS* | MENT |
|      |      | *    |
+======+======+======+
| mdbm | DEPR | mdbm |
| \_sa | ECAT | \_sa |
| ve   | ED   | ve   |
|      |      | is   |
|      |      | only |
|      |      | supp |
|      |      | orte |
|      |      | d    |
|      |      | for  |
|      |      | V2   |
|      |      | MDBM |
|      |      | s.   |
+------+------+------+
| mdbm | DEPR | mdbm |
| \_re | ECAT | \_re |
| stor | ED   | stor |
| e    |      | e    |
|      |      | is   |
|      |      | only |
|      |      | supp |
|      |      | orte |
|      |      | d    |
|      |      | for  |
|      |      | V2   |
|      |      | MDBM |
|      |      | s.   |
+------+------+------+
| mdbm | DEPR | Lega |
| \_se | ECAT | cy   |
| thas | ED   | vers |
| h    |      | ion  |
|      |      | of   |
|      |      | mdbm |
|      |      | \_se |
|      |      | t\_h |
|      |      | ash( |
|      |      | )    |
|      |      | This |
|      |      | func |
|      |      | tion |
|      |      | has  |
|      |      | inco |
|      |      | nsis |
|      |      | tent |
|      |      | nami |
|      |      | ng,  |
|      |      | an   |
|      |      | erro |
|      |      | r    |
|      |      | retu |
|      |      | rn   |
|      |      | valu |
|      |      | e.   |
|      |      | It   |
|      |      | will |
|      |      | be   |
|      |      | remo |
|      |      | ved  |
|      |      | in a |
|      |      | futu |
|      |      | re   |
|      |      | vers |
|      |      | ion. |
+------+------+------+

Only a V2 implementation
~~~~~~~~~~~~~~~~~~~~~~~~

+------+------+------+
| *API | *STA | *COM |
| *    | TUS* | MENT |
|      |      | *    |
+======+======+======+
| mdbm | V3   | Ther |
| \_st | not  | e    |
| at\_ | supp | is   |
| all\ | orte | only |
| _pag | d    | a V2 |
| e    |      | impl |
|      |      | emen |
|      |      | tati |
|      |      | on.  |
|      |      | V3   |
|      |      | not  |
|      |      | curr |
|      |      | entl |
|      |      | y    |
|      |      | supp |
|      |      | orte |
|      |      | d.   |
+------+------+------+
| mdbm | V3   | Ther |
| \_st | not  | e    |
| at\_ | supp | is   |
| head | orte | only |
| er   | d    | a V2 |
|      |      | impl |
|      |      | emen |
|      |      | tati |
|      |      | on.  |
|      |      | V3   |
|      |      | not  |
|      |      | curr |
|      |      | entl |
|      |      | y    |
|      |      | supp |
|      |      | orte |
|      |      | d.   |
+------+------+------+

Has not been implemented
~~~~~~~~~~~~~~~~~~~~~~~~

+-------------------+-------------------+---------------------------------+
| *API*             | *STATUS*          | *COMMENT*                       |
+===================+===================+=================================+
| dbm\_chk\_error   | Not Implemented   | This has not been implemented   |
+-------------------+-------------------+---------------------------------+

Support two compatibility version
---------------------------------

Python
~~~~~~

+------------------------+-------------+----------+
| *Version*              | *Support*   | *Test*   |
+========================+=============+==========+
| Python 2.6.x ~ 2.7.x   | yes         | always   |
+------------------------+-------------+----------+
| Python 3.0.x ~ 3.x.x   | yes         | always   |
+------------------------+-------------+----------+
| PyPy                   | yes         | always   |
+------------------------+-------------+----------+
| PyPy3                  | yes         | always   |
+------------------------+-------------+----------+

MDBM
~~~~

+----------------------------+-------------+----------+-------------+
| *branch or release ver.*   | *Support*   | *Test*   | *Comment*   |
+============================+=============+==========+=============+
| master                     | yes         | always   |             |
+----------------------------+-------------+----------+-------------+
| 4.x                        | yes         | always   |             |
+----------------------------+-------------+----------+-------------+

Install
-------

MDBM
~~~~

-  Ubuntu : See the `pre-build
   packages <https://github.com/torden/go-mdbm/tree/master/pkg>`__
-  RHEL (CentOS) : See the `pre-build
   packages <https://github.com/torden/go-mdbm/tree/master/pkg>`__

py-mdbm (use pip)
~~~~~~~~~~~~~~~~~

.. code:: shell

    pip install py-mdbm

py-mdbm (use source)
~~~~~~~~~~~~~~~~~~~~

Download
^^^^^^^^

.. code:: shell

    git clone https://github.com/torden/py-mdbm

Build and Test
^^^^^^^^^^^^^^

.. code:: shell

    cd py-mdbm
    CMD_PYTHON=`which python` make

Check
~~~~~

.. code:: shell

    $ python
    >>> import mdbm
    >>> help(mdbm)

Benchmark
~~~~~~~~~

.. code:: shell

    cd py-mdbm
    `which pip` install -r requirements.txt
    CMD_PYTHON=`which python` CMD_PYTEST=`which pytest` make benchmark

Example
-------

See the `Source
Code <https://github.com/torden/py-mdbm/tree/master/example>`__ for more
details

The following is Sample codes for a first look at py-mdbm

Creating and populating a database
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Python 2 or higher
^^^^^^^^^^^^^^^^^^

.. code:: python

    import mdbm
    import random

    print("[*] Creating and populating a database")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    flags = flags | mdbm.MDBM_O_CREAT
    flags = flags | mdbm.MDBM_LARGE_OBJECTS
    flags = flags | mdbm.MDBM_ANY_LOCKS
    flags = flags | mdbm.MDBM_O_TRUNC
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)
    for i in range(0, 65535):
        k = str(i)
        v = str(random.randrange(0, 65535))

        rv = dbm.store(k, v, mdbm.MDBM_INSERT)
        if not rv:
            print("[-] failed to data store to ", path)
            break

    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")

Python 3 or higher
^^^^^^^^^^^^^^^^^^

.. code:: python

    import mdbm
    import random

    print("[*] Creating and populating a database")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    flags = flags | mdbm.MDBM_O_CREAT
    flags = flags | mdbm.MDBM_LARGE_OBJECTS
    flags = flags | mdbm.MDBM_ANY_LOCKS
    flags = flags | mdbm.MDBM_O_TRUNC
    mode = 0o644  # means 0644

    with mdbm.open(path, flags, mode) as dbm:
        for i in range(0, 65535):
            k = str(i)
            v = str(random.randrange(0, 65535))

            rv = dbm.store(k, v, mdbm.MDBM_INSERT)
            if not rv:
                print("[-] failed to data store to ", path)
                break

        print("[*] count of records : %d" % dbm.count_records())

    print("done")

Fetching records in-place
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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

    dbm.close()

    print("done")

Fetching and updating records in-place
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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

Deleting records in-place
~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    import mdbm
    import random

    print("[*] Deleting records in-place")

    path = "/tmp/test1.mdbm"
    flags = mdbm.MDBM_O_RDWR
    mode = 0o644  # means 0644

    dbm = mdbm.open(path, flags, mode)

    for i in range(0, 10):

        k = str(random.randrange(0, 65534))

        rv = dbm.delete(k)
        if not rv:
            print("[-] failed to delete an record, key=%s" % k)

        v = dbm.fetch(k)
        if v:
            print("[-] failed to delete an record, key=%s, val=%s" % (k,v))
            break

    print("[*] count of records : %d" % dbm.count_records())

    dbm.close()

    print("done")

Iterating over all records
~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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

Iterating over all keys
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

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

Iteration over all value by key
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    import mdbm
    import random

    print("[*] Creating and populating a database")

    path = "/tmp/test_py_dup.mdbm"
    flags = mdbm.MDBM_O_RDWR
    flags = flags | mdbm.MDBM_O_CREAT
    flags = flags | mdbm.MDBM_LARGE_OBJECTS
    flags = flags | mdbm.MDBM_ANY_LOCKS
    flags = flags | mdbm.MDBM_O_TRUNC
    mode = 0o644  # means 0644

    with mdbm.open(path, flags, mode) as dbm:

        for k in range(0, 100):
            key = str(k)

            for i in range(1, 12):
                val = str(123 * i)

                rv = dbm.store(key, val, mdbm.MDBM_INSERT_DUP)
                if not rv:
                    print("[-] failed to data store to ", path)
                    break

    print("[*] Loop through DB, looking at records with the same key.")
    with mdbm.open(path, mdbm.MDBM_O_RDONLY, mode) as dbm:

        print("[*] count of records : %d" % dbm.count_records())
        print("|-------|-------|")
        print("|  key  |  val  |")
        print("|-------|-------|")

        k = str(random.randrange(0, 99))

        empty_iter = dbm.init_iter()
        info = dbm.fetch_dup_r(k, empty_iter)
        while info:

            print("|%07s|%07s|" % (k, info['val']))
            info = dbm.fetch_dup_r(k, info['iter'])

    print("|-------|-------|")

    print("done")

Benchmark
---------

The following is results of Py-mdbm vs AnyDBM vs SQLite3 vs Kyotocabinet
benchmarks for simple data storing and random fetching in them.

-  See the `Source
   Code <https://github.com/torden/py-mdbm/blob/master/test_benchmark.py>`__
   for more details
-  See the
   `Glossary <https://pytest-benchmark.readthedocs.io/en/latest/glossary.html>`__
   for read result

Spec
~~~~

Host
^^^^

+--------+----------------+
| Type   | Spec           |
+========+================+
| CPU    | Inte i-7       |
+--------+----------------+
| RAM    | DDR4 32G       |
+--------+----------------+
| HDD    | Nvme M.2 SSD   |
+--------+----------------+

VM
^^

+----------------+-----------------------------------------------------------+
| Type           | Spec                                                      |
+================+===========================================================+
| Machine        | VM(VirtualBox)                                            |
+----------------+-----------------------------------------------------------+
| OS             | CentOS 7 64bit                                            |
+----------------+-----------------------------------------------------------+
| CPU            | 2 vCore                                                   |
+----------------+-----------------------------------------------------------+
| RAM            | 8G                                                        |
+----------------+-----------------------------------------------------------+
| AnyDBM         | Berkeley DB (Hash, version 9, native byte-order) format   |
+----------------+-----------------------------------------------------------+
| Mdbm           | 893f7a8 on 26 Jul, MDBM V3 format                         |
+----------------+-----------------------------------------------------------+
| SQLite         | V3                                                        |
+----------------+-----------------------------------------------------------+
| Kyotocabinet   | 1.2.76, kch                                               |
+----------------+-----------------------------------------------------------+

Command
~~~~~~~

::

    CMD_PYTHON=`which python` CMD_PYTEST=`which pytest` make benchmark

File Size
~~~~~~~~~

+--------------------+--------------------+------------------------------------------+--------+
| Count of Records   | Type               | File Name                                | Size   |
+====================+====================+==========================================+========+
| 10,000             | SQLite3            | test\_py\_benchmark\_10000.db            | 300K   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | AnyDBM             | test\_py\_benchmark\_10000.dbm           | 348K   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | Kyotocabinet KCH   | test\_py\_benchmark\_10000.kch           | 6.3M   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | MDBM               | test\_py\_benchmark\_10000.mdbm          | 260K   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | MDBM(TSC)          | test\_py\_benchmark\_tsc\_10000.mdbm     | 260K   |
+--------------------+--------------------+------------------------------------------+--------+
| 100,000            | SQLite3            | test\_py\_benchmark\_100000.db           | 3.3M   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | AnyDBM             | test\_py\_benchmark\_100000.dbm          | 2.5M   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | Kyotocabinet KCH   | test\_py\_benchmark\_100000.kch          | 9.1M   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | MDBM               | test\_py\_benchmark\_100000.mdbm         | 4.0M   |
+--------------------+--------------------+------------------------------------------+--------+
|                    | MDBM(TSC)          | test\_py\_benchmark\_tsc\_100000.mdbm    | 4.0M   |
+--------------------+--------------------+------------------------------------------+--------+
| 1,000,000          | SQLite3            | test\_py\_benchmark\_1000000.db          | 35M    |
+--------------------+--------------------+------------------------------------------+--------+
|                    | AnyDBM             | test\_py\_benchmark\_1000000.dbm         | 39M    |
+--------------------+--------------------+------------------------------------------+--------+
|                    | Kyotocabinet KCH   | test\_py\_benchmark\_1000000.kch         | 37M    |
+--------------------+--------------------+------------------------------------------+--------+
|                    | MDBM               | test\_py\_benchmark\_1000000.mdbm        | 32M    |
+--------------------+--------------------+------------------------------------------+--------+
|                    | MDBM(TSC)          | test\_py\_benchmark\_tsc\_1000000.mdbm   | 32M    |
+--------------------+--------------------+------------------------------------------+--------+

10,000 INSERTs
~~~~~~~~~~~~~~

::

    platform linux2 -- Python 2.7.14, pytest-3.3.2, py-1.5.2, pluggy-0.6.0
    benchmark: 3.1.1 (defaults: timer=time.time disable_gc=False min_rounds=5 min_time=0.000005 max_time=1.0 calibration_precision=10 warmup=False warmup_iterations=100000)
    rootdir: /root/PERSONAL/py-mdbm, inifile:
    plugins: benchmark-3.1.1
    collected 31 items

    ------------------------------------------------------------------------------------------- benchmark: 5 tests ------------------------------------------------------------------------------------------
    Name (time in ms)                          Min                 Max                Mean            StdDev              Median               IQR            Outliers      OPS            Rounds  Iterations
    ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_tsc_10000              42.7790 (1.0)       46.4041 (1.0)       44.4735 (1.0)      0.8599 (1.0)       44.7228 (1.01)     1.0532 (1.0)           5;0  22.4853 (1.0)          23           1
    test_mdbm_store_10000                  43.0260 (1.01)      55.0859 (1.19)      45.1026 (1.01)     2.8206 (3.28)      44.1189 (1.0)      1.9995 (1.90)          3;2  22.1716 (0.99)         23           1
    test_kyotocabinet_kch_store_10000      64.2769 (1.50)      72.2461 (1.56)      66.6182 (1.50)     2.1470 (2.50)      66.5540 (1.51)     2.4997 (2.37)          6;1  15.0109 (0.67)         16           1
    test_sqlite3_store_10000               71.1770 (1.66)      89.0980 (1.92)      74.6003 (1.68)     4.5800 (5.33)      73.3149 (1.66)     2.8142 (2.67)          1;1  13.4048 (0.60)         13           1
    test_anydbm_store_10000               129.4661 (3.03)     132.9770 (2.87)     131.7690 (2.96)     1.3268 (1.54)     132.4065 (3.00)     2.1240 (2.02)          1;0   7.5890 (0.34)          8           1
    ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

100,000 INSERTs
~~~~~~~~~~~~~~~

::

    ------------------------------------------------------------------------------------------------ benchmark: 5 tests -----------------------------------------------------------------------------------------------
    Name (time in ms)                             Min                   Max                  Mean             StdDev                Median                IQR            Outliers     OPS            Rounds  Iterations
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_100000                   432.5280 (1.0)        444.3109 (1.0)        440.1428 (1.0)       5.1283 (1.0)        443.0151 (1.0)       7.8554 (1.46)          1;0  2.2720 (1.0)           5           1
    test_mdbm_store_tsc_100000               443.6021 (1.03)       457.2010 (1.03)       450.7210 (1.02)      6.5694 (1.28)       453.4068 (1.02)     12.3150 (2.28)          2;0  2.2187 (0.98)          5           1
    test_kyotocabinet_kch_store_100000       553.1771 (1.28)       572.2950 (1.29)       559.4640 (1.27)      7.3967 (1.44)       557.5171 (1.26)      5.3908 (1.0)           1;1  1.7874 (0.79)          5           1
    test_sqlite3_store_100000                668.3731 (1.55)       690.7680 (1.55)       676.8432 (1.54)     10.4372 (2.04)       670.3589 (1.51)     17.5762 (3.26)          1;0  1.4774 (0.65)          5           1
    test_anydbm_store_100000               1,746.3379 (4.04)     1,778.0671 (4.00)     1,759.8858 (4.00)     12.6857 (2.47)     1,761.1270 (3.98)     19.0974 (3.54)          2;0  0.5682 (0.25)          5           1
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

1,000,000 INSERTs
~~~~~~~~~~~~~~~~~

::

    ----------------------------------------------------------------------------------------- benchmark: 5 tests -----------------------------------------------------------------------------------------
    Name (time in s)                            Min                Max               Mean            StdDev             Median               IQR            Outliers     OPS            Rounds  Iterations
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_store_1000000                  4.4507 (1.0)       4.5549 (1.0)       4.5087 (1.0)      0.0386 (1.41)      4.5170 (1.00)     0.0471 (1.0)           2;0  0.2218 (1.0)           5           1
    test_mdbm_store_tsc_1000000              4.4964 (1.01)      4.5557 (1.00)      4.5252 (1.00)     0.0275 (1.0)       4.5133 (1.0)      0.0494 (1.05)          3;0  0.2210 (1.00)          5           1
    test_kyotocabinet_kch_store_1000000      5.5518 (1.25)      7.3104 (1.60)      5.9554 (1.32)     0.7585 (27.62)     5.6386 (1.25)     0.4548 (9.65)          1;1  0.1679 (0.76)          5           1
    test_sqlite3_store_1000000               6.9506 (1.56)      7.1580 (1.57)      7.0168 (1.56)     0.0811 (2.95)      6.9938 (1.55)     0.0623 (1.32)          1;1  0.1425 (0.64)          5           1
    test_anydbm_store_1000000               18.8494 (4.24)     19.3685 (4.25)     19.1384 (4.24)     0.1884 (6.86)     19.1481 (4.24)     0.1982 (4.21)          2;0  0.0523 (0.24)          5           1
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

10,000 Random Key SELECTs
~~~~~~~~~~~~~~~~~~~~~~~~~

::

    ----------------------------------------------------------------------------------------------- benchmark: 6 tests -----------------------------------------------------------------------------------------------
    Name (time in ms)                                 Min                 Max                Mean             StdDev              Median                IQR            Outliers      OPS            Rounds  Iterations
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_random_fetch_10000                  33.6039 (1.0)       37.1680 (1.02)      35.4372 (1.0)       0.8726 (1.63)      35.3181 (1.0)       1.0861 (1.73)          9;0  28.2189 (1.0)          29           1
    test_mdbm_preload_random_fetch_tsc_10000      34.1651 (1.02)      36.5930 (1.0)       35.5276 (1.00)      0.5728 (1.07)      35.6691 (1.01)      0.6691 (1.06)          8;0  28.1471 (1.00)         29           1
    test_mdbm_preload_random_fetch_10000          34.8370 (1.04)      37.1509 (1.02)      35.6486 (1.01)      0.5368 (1.0)       35.6290 (1.01)      0.6291 (1.0)           8;1  28.0516 (0.99)         27           1
    test_kyotocabinet_random_fetch_10000          50.1349 (1.49)     315.4690 (8.62)      66.3761 (1.87)     60.3302 (112.39)    52.3400 (1.48)      1.3785 (2.19)          1;1  15.0657 (0.53)         19           1
    test_anydbm_random_fetch_10000                98.3920 (2.93)     127.4319 (3.48)     103.2393 (2.91)      8.6436 (16.10)    101.2516 (2.87)      3.1178 (4.96)          1;1   9.6862 (0.34)         10           1
    test_sqlite3_random_fetch_10000              179.9428 (5.35)     264.3309 (7.22)     198.3913 (5.60)     32.8237 (61.15)    183.5115 (5.20)     14.0412 (22.32)         1;1   5.0405 (0.18)          6           1
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

100,000 Random Key SELECTs
~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    -------------------------------------------------------------------------------------------------- benchmark: 5 tests --------------------------------------------------------------------------------------------------
    Name (time in ms)                                    Min                   Max                  Mean            StdDev                Median               IQR            Outliers     OPS            Rounds  Iterations
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_preload_random_fetch_tsc_100000       351.3479 (1.0)        362.6180 (1.02)       358.0612 (1.01)     4.9707 (3.89)       360.7321 (1.02)     8.1980 (3.88)          1;0  2.7928 (0.99)          5           1
    test_mdbm_preload_random_fetch_100000           352.9408 (1.00)       360.9550 (1.01)       356.8196 (1.01)     3.2021 (2.51)       357.4481 (1.01)     5.0185 (2.38)          2;0  2.8025 (0.99)          5           1
    test_mdbm_random_fetch_100000                   353.4501 (1.01)       356.4832 (1.0)        354.6917 (1.0)      1.2767 (1.0)        354.3482 (1.0)      2.1121 (1.0)           1;0  2.8193 (1.0)           5           1
    test_kyotocabinet_random_fetch_100000           513.2129 (1.46)       516.0379 (1.45)       514.8367 (1.45)     1.3007 (1.02)       515.3730 (1.45)     2.3472 (1.11)          1;0  1.9424 (0.69)          5           1
    test_anydbm_random_fetch_100000               1,196.3558 (3.41)     1,217.2129 (3.41)     1,207.2943 (3.40)     7.5601 (5.92)     1,206.6510 (3.41)     8.3598 (3.96)          2;0  0.8283 (0.29)          5           1
    ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

1,000,000 Random Key SELECTs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    --------------------------------------------------------------------------------------------- benchmark: 5 tests --------------------------------------------------------------------------------------------
    Name (time in s)                                   Min                Max               Mean            StdDev             Median               IQR            Outliers     OPS            Rounds  Iterations
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    test_mdbm_preload_random_fetch_tsc_1000000      3.6708 (1.0)       3.7161 (1.0)       3.7020 (1.0)      0.0194 (1.29)      3.7138 (1.00)     0.0255 (1.71)          1;0  0.2701 (1.0)           5           1
    test_mdbm_preload_random_fetch_1000000          3.6781 (1.00)      3.7315 (1.00)      3.7045 (1.00)     0.0212 (1.41)      3.7021 (1.0)      0.0336 (2.25)          2;0  0.2699 (1.00)          5           1
    test_mdbm_random_fetch_1000000                  3.6957 (1.01)      3.7336 (1.00)      3.7079 (1.00)     0.0150 (1.0)       3.7054 (1.00)     0.0149 (1.0)           1;0  0.2697 (1.00)          5           1
    test_kyotocabinet_random_fetch_1000000          5.2549 (1.43)      5.2865 (1.42)      5.2677 (1.42)     0.0151 (1.01)      5.2599 (1.42)     0.0273 (1.82)          1;0  0.1898 (0.70)          5           1
    test_anydbm_random_fetch_1000000               12.3323 (3.36)     12.4784 (3.36)     12.4044 (3.35)     0.0586 (3.90)     12.3911 (3.35)     0.0927 (6.20)          2;0  0.0806 (0.30)          5           1
    -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Link
----

-  `Yahoo! MDBM <https://github.com/yahoo/mdbm>`__
-  `MDBM::Concept <http://yahoo.github.io/mdbm/guide/concepts.html>`__
-  `MDBM::Build <https://github.com/yahoo/mdbm/blob/master/README.build>`__
-  `MDBM::Document <http://yahoo.github.io/mdbm/>`__
-  `MDBM::FAQ <http://yahoo.github.io/mdbm/guide/faq.html>`__
-  `DBM <https://en.wikipedia.org/wiki/Dbm>`__
-  `MDBM::Macro(const) <http://yahoo.github.io/mdbm/api/mdbm_8h.html>`__
-  `Go-mdbm <https://github.com/torden/go-mdbm>`__
-  `PHP-mdbm <https://github.com/torden/php-mdbm>`__

--------------

Please feel free. I hope it is helpful for you

.. |Build Status| image:: https://travis-ci.org/torden/py-mdbm.svg?branch=master
   :target: https://travis-ci.org/torden/py-mdbm
.. |GitHub version| image:: https://badge.fury.io/gh/torden%2Fpy-mdbm.svg
   :target: https://badge.fury.io/gh/torden%2Fpy-mdbm
.. |GitHub version| image:: https://badge.fury.io/gh/yahoo%2Fmdbm.svg
   :target: https://badge.fury.io/gh/yahoo%2Fmdbm
