# -*- Mode: Python -*-

import os
import unittest

from common import gio, glib


class TestFile(unittest.TestCase):
    def setUp(self):
        self._f = open("file.txt", "w+")
        self.file = gio.File("file.txt")

    def tearDown(self):
        self._f.close()
        if os.path.exists('file.txt'):
            os.unlink("file.txt")

    def testReadAsync(self):
        self._f.write("testing")
        self._f.seek(0)

        def callback(file, result):
            try:
                stream = file.read_finish(result)
                self.failUnless(isinstance(stream, gio.InputStream))
                self.assertEquals(stream.read(), "testing")
            finally:
                loop.quit()

        self.file.read_async(callback)

        loop = glib.MainLoop()
        loop.run()

    def testAppendToAsync(self):
        self._f.write("append_to ")
        self._f.close()

        def callback(file, result):
            try:
                stream = file.append_to_finish(result)
                self.failUnless(isinstance(stream, gio.OutputStream))
                w = stream.write("testing")
                cont, leng, etag = self.file.load_contents()
                self.assertEqual(cont, "append_to testing")
            finally:
                loop.quit()

        self.file.append_to_async(callback, gio.FILE_CREATE_NONE,
                                  glib.PRIORITY_HIGH)

        loop = glib.MainLoop()
        loop.run()

    def testAppendToAsyncNoargs(self):
        self._f.write("append_to ")
        self._f.close()

        def callback(file, result):
            try:
                stream = file.append_to_finish(result)
                self.failUnless(isinstance(stream, gio.OutputStream))
                w = stream.write("testing")
                cont, leng, etag = self.file.load_contents()
                self.assertEqual(cont, "append_to testing")
            finally:
                loop.quit()

        self.file.append_to_async(callback)

        loop = glib.MainLoop()
        loop.run()

    def testCreateAsync(self):
        def callback(file, result):
            try:
                stream = file.create_finish(result)
                self.failUnless(isinstance(stream, gio.OutputStream))
                w = stream.write("testing")
                cont, leng, etag = file.load_contents()
                self.assertEqual(cont, "testing")
            finally:
                if os.path.exists('temp.txt'):
                    os.unlink("temp.txt")
                loop.quit()

        gfile = gio.File("temp.txt")
        gfile.create_async(callback, gio.FILE_CREATE_NONE,
                           glib.PRIORITY_HIGH)

        loop = glib.MainLoop()
        loop.run()

    def testCreateAsyncNoargs(self):
        def callback(file, result):
            try:
                stream = file.create_finish(result)
                self.failUnless(isinstance(stream, gio.OutputStream))
                w = stream.write("testing")
                cont, leng, etag = file.load_contents()
                self.assertEqual(cont, "testing")
            finally:
                if os.path.exists('temp.txt'):
                    os.unlink("temp.txt")
                loop.quit()

        gfile = gio.File("temp.txt")
        gfile.create_async(callback)

        loop = glib.MainLoop()
        loop.run()

    def testReplaceAsync(self):
        self._f.write("testing")
        self._f.close()

        def callback(file, result):
            try:
                stream = file.replace_finish(result)
                self.failUnless(isinstance(stream, gio.OutputStream))
                stream.write("some new string")
                stream.close()
                cont, leng, etag = file.load_contents()
                self.assertEqual(cont, "some new string")
            finally:
                loop.quit()


        self.file.replace_async(callback, None, True, gio.FILE_CREATE_NONE,
                                glib.PRIORITY_HIGH)

        loop = glib.MainLoop()
        loop.run()

    def testReplaceAsyncNoargs(self):
        self._f.write("testing")
        self._f.close()

        def callback(file, result):
            try:
                stream = file.replace_finish(result)
                self.failUnless(isinstance(stream, gio.OutputStream))
                stream.write("some new string")
                stream.close()
                cont, leng, etag = file.load_contents()
                self.assertEqual(cont, "some new string")
            finally:
                loop.quit()


        self.file.replace_async(callback)

        loop = glib.MainLoop()
        loop.run()

    def testReadAsyncError(self):
        self.assertRaises(TypeError, self.file.read_async)
        self.assertRaises(TypeError, self.file.read_async, "foo", "bar")
        self.assertRaises(TypeError, self.file.read_async, "foo",
                          priority="bar")
        self.assertRaises(TypeError, self.file.read_async, "foo",
                          priority="bar")
        self.assertRaises(TypeError, self.file.read_async, "foo",
                          priority=1, cancellable="bar")
        self.assertRaises(TypeError, self.file.read_async, "foo", 1, "bar")

    def testConstructor(self):
        for gfile in [gio.File("/"),
                      gio.File("file:///"),
                      gio.File(uri="file:///"),
                      gio.File(path="/"),
                      gio.File(u"/"),
                      gio.File(path=u"/")]:
            self.failUnless(isinstance(gfile, gio.File))
            self.assertEquals(gfile.get_path(), "/")
            self.assertEquals(gfile.get_uri(), "file:///")

    def testConstructorError(self):
        self.assertRaises(TypeError, gio.File)
        self.assertRaises(TypeError, gio.File, 1)
        self.assertRaises(TypeError, gio.File, "foo", "bar")
        self.assertRaises(TypeError, gio.File, foo="bar")
        self.assertRaises(TypeError, gio.File, uri=1)
        self.assertRaises(TypeError, gio.File, path=1)

    def testLoadContents(self):
        self._f.write("testing load_contents")
        self._f.seek(0)
        c = gio.Cancellable()
        cont, leng, etag = self.file.load_contents(cancellable=c)
        self.assertEqual(cont, "testing load_contents")
        self.assertEqual(leng, 21)
        self.assertNotEqual(etag, '')

    def testLoadContentsAsync(self):
        self._f.write("testing load_contents_async")
        self._f.seek(0)

        def callback(contents, result):
            try:
                cont, leng, etag = contents.load_contents_finish(result)
                self.assertEqual(cont, "testing load_contents_async")
                self.assertEqual(leng, 27)
                self.assertNotEqual(etag, '')
            finally:
                loop.quit()

        canc = gio.Cancellable()
        self.file.load_contents_async(callback, cancellable=canc)

        loop = glib.MainLoop()
        loop.run()

    def testQueryInfoAsync(self):
        def callback(file, result):
            try:
                info = file.query_info_finish(result)
                self.failUnless(isinstance(info, gio.FileInfo))
                self.failUnless(info.get_name(), "file.txt")
            finally:
                loop.quit()

        self.file.query_info_async(callback, "standard")

        loop = glib.MainLoop()
        loop.run()

    def testMountMountable(self):
        gfile = gio.File('localtest:')
        def unmount_done(mount, result):
            try:
                retval = mount.unmount_finish(result)
                self.failUnless(retval)
            finally:
                loop.quit()

        def mount_enclosing_volume_done(gfile, result):
            try:
                try:
                    retval = gfile.mount_enclosing_volume_finish(result)
                except gio.Error, e:
                    # If we run the tests too fast
                    if e.code == gio.ERROR_ALREADY_MOUNTED:
                        print ('WARNING: testfile is already mounted, '
                        'skipping test')
                        loop.quit()
                        return
                    raise
                self.failUnless(retval)
            finally:
                try:
                    mount = gfile.find_enclosing_mount()
                except gio.Error:
                    loop.quit()
                    return
                mount.unmount(unmount_done)

        mount_operation = gio.MountOperation()
        gfile.mount_enclosing_volume(mount_operation,
                                     mount_enclosing_volume_done)

        loop = glib.MainLoop()
        loop.run()

    def testCopy(self):
        if os.path.exists('copy.txt'):
            os.unlink("copy.txt")

        source = gio.File('file.txt')
        destination = gio.File('copy.txt')
        try:
            retval = source.copy(destination)
            self.failUnless(retval)

            self.failUnless(os.path.exists('copy.txt'))
            self.assertEqual(open('file.txt').read(),
                             open('copy.txt').read())
        finally:
            os.unlink("copy.txt")

        self.called = False
        def callback(current, total):
            self.called = True
        source = gio.File('file.txt')
        destination = gio.File('copy.txt')
        try:
            retval = source.copy(destination, callback)
            self.failUnless(retval)

            self.failUnless(os.path.exists('copy.txt'))
            self.assertEqual(open('file.txt').read(),
                             open('copy.txt').read())
            self.failUnless(self.called)
        finally:
            os.unlink("copy.txt")

    # See bug 546591.
    def test_copy_progress(self):
        source = gio.File('file.txt')
        destination = gio.File('copy.txt')

        def progress(current, total):
            self.assert_(isinstance(current, long))
            self.assert_(isinstance(total, long))
            self.assert_(0 <= current <= total)

        try:
            retval = source.copy(destination,
                                 flags=gio.FILE_COPY_OVERWRITE,
                                 progress_callback=progress)
            self.failUnless(retval)

            self.failUnless(os.path.exists('copy.txt'))
            self.assertEqual(open('file.txt').read(),
                             open('copy.txt').read())
        finally:
            os.unlink("copy.txt")

    def testMove(self):
        if os.path.exists('move.txt'):
            os.unlink("move.txt")

        source = gio.File('file.txt')
        destination = gio.File('move.txt')
        retval = source.move(destination)
        self.failUnless(retval)

        self.failIf(os.path.exists('file.txt'))
        self.failUnless(os.path.exists('move.txt'))

        self.called = False
        def callback(current, total):
            self.called = True
        source = gio.File('move.txt')
        destination = gio.File('move-2.txt')
        try:
            retval = source.move(destination, callback)
            self.failUnless(retval)

            self.failIf(os.path.exists('move.txt'))
            self.failUnless(os.path.exists('move-2.txt'))
            self.failUnless(self.called)
        finally:
            os.unlink("move-2.txt")

    def testInfoList(self):
        infolist = self.file.query_settable_attributes()
        for info in infolist:
            if info.name == "time::modified":
                self.assertEqual(info.type, gio.FILE_ATTRIBUTE_TYPE_UINT64)
                self.assertEqual(info.name, "time::modified")
                self.assertEqual(info.flags,
                                 gio.FILE_ATTRIBUTE_INFO_COPY_WHEN_MOVED)

    def testQueryWritableNamespaces(self):
        infolist = self.file.query_writable_namespaces()
        for info in infolist:
            if info.name == "xattr":
                self.assertEqual(info.type, gio.FILE_ATTRIBUTE_TYPE_STRING)

    def testSetAttribute(self):
        self._f.write("testing attributes")
        self._f.seek(0)
        infolist = self.file.query_settable_attributes()

        self.assertNotEqual(len(infolist), 0)

        for info in infolist:
            if info.name == "time::modified-usec":
                ret = self.file.set_attribute("time::modified-usec",
                                              gio.FILE_ATTRIBUTE_TYPE_UINT32,
                                              10, gio.FILE_QUERY_INFO_NONE)
                self.assertEqual(ret, True)

    def testReplaceContents(self):
        self.file.replace_contents("testing replace_contents")
        cont, leng, etag = self.file.load_contents()
        self.assertEqual(cont, "testing replace_contents")

        caught = False
        try:
            self.file.replace_contents("this won't work", etag="wrong")
        except gio.Error, e:
            self.assertEqual(e.code, gio.ERROR_WRONG_ETAG)
            caught = True
        self.failUnless(caught)

        self.file.replace_contents("testing replace_contents again", etag=etag)
        cont, leng, etag = self.file.load_contents()
        self.assertEqual(cont, "testing replace_contents again")

        self.file.replace_contents("testing replace_contents yet again", etag=None)
        cont, leng, etag = self.file.load_contents()
        self.assertEqual(cont, "testing replace_contents yet again")

    def testReplaceContentsAsync(self):

        def callback(contents, result):
            try:
                newetag = contents.replace_contents_finish(result)
                cont, leng, etag = self.file.load_contents()
                self.assertEqual(cont, "testing replace_contents_async")
                self.assertEqual(leng, 30)
                self.assertEqual(etag, newetag)
                self.assertNotEqual(newetag, '')
            finally:
                loop.quit()

        canc = gio.Cancellable()
        self.file.replace_contents_async("testing replace_contents_async", callback, cancellable=canc)

        loop = glib.MainLoop()
        loop.run()

    def test_eq(self):
        self.assertEqual(gio.File('foo'),
                         gio.File('foo'))
        self.assertNotEqual(gio.File('foo'),
                            gio.File('bar'))

    def test_hash(self):
        self.assertEquals(hash(gio.File('foo')),
                          hash(gio.File('foo')))


class TestGFileEnumerator(unittest.TestCase):
    def setUp(self):
        self.file = gio.File(".")

    def testEnumerateChildren(self):
        enumerator = self.file.enumerate_children(
            "standard::*", gio.FILE_QUERY_INFO_NOFOLLOW_SYMLINKS)
        for file_info in enumerator:
            if file_info.get_name() == 'test_gio.py':
                break
        else:
            raise AssertionError

    def testEnumerateChildrenAsync(self):
        def callback(gfile, result):
            try:
                for file_info in gfile.enumerate_children_finish(result):
                    if file_info.get_name() == 'test_gio.py':
                        break
                else:
                    raise AssertionError
            finally:
                loop.quit()

        self.file.enumerate_children_async(
            "standard::*", callback)
        loop = glib.MainLoop()
        loop.run()

    def testNextFilesAsync(self):
        def callback(enumerator, result):
            try:
                for file_info in enumerator.next_files_finish(result):
                    if file_info.get_name() == 'test_gio.py':
                        break
                else:
                    raise AssertionError
            finally:
                loop.quit()

        enumerator = self.file.enumerate_children(
            "standard::*", gio.FILE_QUERY_INFO_NOFOLLOW_SYMLINKS)
        enumerator.next_files_async(1000, callback)
        loop = glib.MainLoop()
        loop.run()


class TestInputStream(unittest.TestCase):
    def setUp(self):
        self._f = open("inputstream.txt", "w+")
        self._f.write("testing")
        self._f.seek(0)
        self.stream = gio.unix.InputStream(self._f.fileno(), False)

    def tearDown(self):
        self._f.close()
        os.unlink("inputstream.txt")

    def testRead(self):
        self.assertEquals(self.stream.read(), "testing")

        self.stream = gio.MemoryInputStream()
        self.assertEquals(self.stream.read(), '')

        self.stream = gio.MemoryInputStream()
        some_data = open("test_gio.py", "rb").read()
        self.stream.add_data(some_data)
        self.assertEquals(self.stream.read(), some_data)

        stream = gio.MemoryInputStream()
        stream.add_data(some_data)
        self.assertEquals(self._read_in_loop(stream,
                                             lambda: stream.read(50),
                                             50),
                          some_data)

    def test_read_part(self):
        self.assertEquals(self._read_in_loop(self.stream,
                                             lambda: self.stream.read_part()),
                          'testing')

        stream = gio.MemoryInputStream()
        some_data = open('test_gio.py', 'rb').read()
        stream.add_data(some_data)
        self.assertEquals(self._read_in_loop(stream,
                                             lambda: stream.read_part(50),
                                             50),
                          some_data)

    def _read_in_loop(self, stream, reader, size_limit=0):
        read_data = ''
        while True:
            read_part = reader()
            if read_part:
                read_data += read_part
                if size_limit > 0:
                    self.assert_(len(read_part) <= size_limit,
                                 '%d <= %d' % (len(read_part), size_limit))
            else:
                return read_data

    def testReadAsync(self):
        def callback(stream, result):
            self.assertEquals(result.get_op_res_gssize(), 7)
            try:
                data = stream.read_finish(result)
                self.assertEquals(data, "testing")
                stream.close()
            finally:
                loop.quit()

        self.stream.read_async(7, callback)

        loop = glib.MainLoop()
        loop.run()

    def testReadAsyncError(self):
        self.count = 0
        def callback(stream, result):
            try:
                self.count += 1
                if self.count == 1:
                    return
                self.assertRaises(gio.Error, stream.read_finish, result)
            finally:
                loop.quit()

        self.stream.read_async(10240, callback)
        self.stream.read_async(10240, callback)

        loop = glib.MainLoop()
        loop.run()

        self.assertEquals(self.count, 2)

        self.assertRaises(TypeError, self.stream.read_async)
        self.assertRaises(TypeError, self.stream.read_async, "foo")
        self.assertRaises(TypeError, self.stream.read_async, 1024, "bar")
        self.assertRaises(TypeError, self.stream.read_async, 1024,
                          priority="bar")
        self.assertRaises(TypeError, self.stream.read_async, 1024,
                          priority="bar")
        self.assertRaises(TypeError, self.stream.read_async, 1024,
                          priority=1, cancellable="bar")
        self.assertRaises(TypeError, self.stream.read_async, 1024, 1, "bar")


    # FIXME: this makes 'make check' freeze
    def _testCloseAsync(self):
        def callback(stream, result):
            try:
                self.failUnless(stream.close_finish(result))
            finally:
                loop.quit()

        self.stream.close_async(callback)

        loop = glib.MainLoop()
        loop.run()


class TestDataInputStream(unittest.TestCase):
    def setUp(self):
        self.base_stream = gio.MemoryInputStream()
        self.data_stream = gio.DataInputStream(self.base_stream)

    def test_read_line(self):
        # Currently fails because GIO itself is buggy.  See bug 547481.
        return
        self.base_stream.add_data('foo\nbar\n\nbaz')
        self.assertEquals('foo\n', self.data_stream.read_line())
        self.assertEquals('bar\n', self.data_stream.read_line())
        self.assertEquals('\n', self.data_stream.read_line())
        self.assertEquals('baz', self.data_stream.read_line())

    def test_read_until(self):
        self.base_stream.add_data('sentence.end of line\nthe rest')
        self.assertEquals('sentence', self.data_stream.read_until('.!?'))
        self.assertEquals('end of line', self.data_stream.read_until('\n\r'))
        self.assertEquals('the rest', self.data_stream.read_until('#$%^&'))


class TestMemoryInputStream(unittest.TestCase):
    def setUp(self):
        self.stream = gio.MemoryInputStream()

    def test_add_data(self):
        self.stream.add_data('foobar')
        self.assertEquals('foobar', self.stream.read())

        self.stream.add_data('ham ')
        self.stream.add_data(None)
        self.stream.add_data('spam')
        self.assertEquals('ham spam', self.stream.read())


class TestOutputStream(unittest.TestCase):
    def setUp(self):
        self._f = open("outputstream.txt", "w")
        self.stream = gio.unix.OutputStream(self._f.fileno(), False)

    def tearDown(self):
        self._f.close()
        os.unlink("outputstream.txt")

    def testWrite(self):
        self.stream.write("testing")
        self.stream.close()
        self.failUnless(os.path.exists("outputstream.txt"))
        self.assertEquals(open("outputstream.txt").read(), "testing")

    def test_write_part(self):
        stream = gio.MemoryOutputStream()
        some_data = open('test_gio.py', 'rb').read()
        buffer = some_data

        # In fact this makes only one looping (memory stream is fast,
        # write_part behaves just like write), but let's still be
        # complete.
        while buffer:
            written = stream.write_part(buffer)
            if written == len(buffer):
                break
            else:
                buffer = buffer[written:]

        self.assertEquals(stream.get_contents(), some_data)

    def testWriteAsync(self):
        def callback(stream, result):
            self.assertEquals(result.get_op_res_gssize(), 7)
            try:
                self.assertEquals(stream.write_finish(result), 7)
                self.failUnless(os.path.exists("outputstream.txt"))
                self.assertEquals(open("outputstream.txt").read(), "testing")
            finally:
                loop.quit()

        self.stream.write_async("testing", callback)

        loop = glib.MainLoop()
        loop.run()

    def testWriteAsyncError(self):
        def callback(stream, result):
            self.assertEquals(result.get_op_res_gssize(), 0)
            try:
                self.assertRaises(gio.Error, stream.write_finish, result)
            finally:
                loop.quit()

        self.stream.close()
        self.stream.write_async("testing", callback)

        loop = glib.MainLoop()
        loop.run()

        self.assertRaises(TypeError, self.stream.write_async)
        self.assertRaises(TypeError, self.stream.write_async, 1138)
        self.assertRaises(TypeError, self.stream.write_async, "foo", "bar")
        self.assertRaises(TypeError, self.stream.write_async, "foo",
                          priority="bar")
        self.assertRaises(TypeError, self.stream.write_async, "foo",
                          priority="bar")
        self.assertRaises(TypeError, self.stream.write_async, "foo",
                          priority=1, cancellable="bar")
        self.assertRaises(TypeError, self.stream.write_async, "foo", 1, "bar")

    # FIXME: this makes 'make check' freeze
    def _testCloseAsync(self):
        def callback(stream, result):
            try:
                self.failUnless(stream.close_finish(result))
            finally:
                loop.quit()

        self.stream.close_async(callback)

        loop = glib.MainLoop()
        loop.run()


class TestMemoryOutputStream(unittest.TestCase):
    def setUp(self):
        self.stream = gio.MemoryOutputStream()

    def test_get_contents(self):
        self.stream.write('foobar')
        self.assertEquals('foobar', self.stream.get_contents())

        self.stream.write('baz')
        self.assertEquals('foobarbaz', self.stream.get_contents())


class TestVolumeMonitor(unittest.TestCase):
    def setUp(self):
        self.monitor = gio.volume_monitor_get()

    def testGetConnectedDrives(self):
        drives = self.monitor.get_connected_drives()
        self.failUnless(isinstance(drives, list))

    def testGetVolumes(self):
        volumes = self.monitor.get_volumes()
        self.failUnless(isinstance(volumes, list))

    def testGetMounts(self):
        mounts = self.monitor.get_mounts()
        self.failUnless(isinstance(mounts, list))
        if not mounts:
            return

        self.failUnless(isinstance(mounts[0], gio.Mount))
        # Bug 538601
        icon = mounts[0].get_icon()
        if not icon:
            return
        self.failUnless(isinstance(icon, gio.Icon))


class TestContentTypeGuess(unittest.TestCase):
    def testFromName(self):
        mime_type = gio.content_type_guess('diagram.svg')
        self.assertEquals('image/svg+xml', mime_type)

    def testFromContents(self):
        mime_type = gio.content_type_guess(data='<html></html>')
        self.assertEquals('text/html', mime_type)

    def testFromContentsUncertain(self):
        mime_type, result_uncertain = gio.content_type_guess(
            data='<html></html>', want_uncertain=True)
        self.assertEquals('text/html', mime_type)
        self.assertEquals(bool, type(result_uncertain))


class TestFileInfo(unittest.TestCase):
    def setUp(self):
        self.fileinfo = gio.File("test_gio.py").query_info("*")

    def testListAttributes(self):
        attributes = self.fileinfo.list_attributes("standard")
        self.failUnless(attributes)
        self.failUnless('standard::name' in attributes)

    def testModificationTime(self):
        mtime = self.fileinfo.get_modification_time()
        self.assertEqual(type(mtime), float)


class TestAppInfo(unittest.TestCase):
    def setUp(self):
        self.appinfo = gio.AppInfo("does-not-exist")

    def testSimple(self):
        self.assertEquals(self.appinfo.get_description(),
                          "Custom definition for does-not-exist")

    def test_eq(self):
        info1 = gio.app_info_get_all()[0]
        info2 = info1.dup()
        self.assert_(info1 is not info2)
        self.assertEquals(info1, info2)

        self.assertNotEqual(gio.app_info_get_all()[0], gio.app_info_get_all()[1])

class TestVfs(unittest.TestCase):
    def setUp(self):
        self.vfs = gio.vfs_get_default()

    def testGetSupportedURISchemes(self):
        result = self.vfs.get_supported_uri_schemes()
        self.failUnless(type(result), [])
