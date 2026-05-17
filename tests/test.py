# -*- coding: utf-8 -*-

import unittest
import os
from math import fabs
from pylibconfig import Config

class PyLibConfigTest(unittest.TestCase):
    def test_basic_operations(self):
        """Test Config read/write/add/setValue/value"""
        config = Config()

        config.addString("", "test")
        config.setValue("test", "value")
        config.addBoolean("", "test_bool")
        config.setValue("test_bool", True)
        config.addInteger("", "test_int")
        config.setValue("test_int", 9)
        config.addInteger("", "test_unsigned_int")
        config.setValue("test_unsigned_int", -9)
        config.addFloat("", "test_float")
        config.setValue("test_float", 82.1002)

        self.assertTrue(config.value("test")[0] == "value")
        self.assertTrue(config.value("test")[1] == True)
        self.assertTrue(config.value("test_bool")[0] == True)
        self.assertTrue(config.value("loose")[1] == False)
        self.assertTrue(config.value("test_int")[0] == 9)
        self.assertTrue(config.value("test_unsigned_int")[0] == -9)
        self.assertTrue(fabs(config.value("test_float")[0] - 82.1002) < 0.00001)

        config.setValue("test_bool", False)
        config.writeFile("test.conf")
        self.assertTrue(os.path.exists("./test.conf"))

        config2 = Config()
        config2.readFile("test.conf")
        self.assertTrue(config2.value("test")[0] == "value")
        self.assertTrue(config2.value("test")[1] == True)

        os.remove("./test.conf")

    def test_children(self):
        """Test children() and children_root()"""
        config = Config()
        config.addGroup("", "group1")
        config.addString("", "str1")
        config.addInteger("", "int1")

        children = config.children("")
        self.assertEqual(len(children), 3)

    def test_exists(self):
        """Test exists()"""
        config = Config()
        config.addString("", "foo")
        self.assertTrue(config.exists("foo"))
        self.assertFalse(config.exists("nonexistent"))

    def test_lookup_methods(self):
        """Test type-safe lookup methods"""
        config = Config()
        config.addInteger("", "int_key")
        config.setValue("int_key", 42)
        config.addString("", "str_key")
        config.setValue("str_key", "hello")
        config.addBoolean("", "bool_key")
        config.setValue("bool_key", True)
        config.addFloat("", "float_key")
        config.setValue("float_key", 3.14)

        self.assertEqual(config.lookupInt("int_key"), 42)
        self.assertEqual(config.lookupString("str_key"), "hello")
        self.assertEqual(config.lookupBool("bool_key"), True)
        self.assertTrue(fabs(config.lookupFloat("float_key") - 3.14) < 0.0001)

        # Default values for missing keys
        self.assertEqual(config.lookupInt("missing", 99), 99)
        self.assertEqual(config.lookupString("missing", "default"), "default")
        self.assertEqual(config.lookupBool("missing", True), True)

    def test_getRoot_and_lookup(self):
        """Test getRoot() and lookup() returning Setting objects"""
        config = Config()
        config.addString("", "test")
        config.setValue("test", "value")

        root = config.getRoot()
        self.assertTrue(root.isGroup())
        self.assertTrue(root.isAggregate())
        self.assertTrue(root.isRoot())
        self.assertFalse(root.isScalar())
        self.assertEqual(root.getLength(), 1)

        s = config.lookup("test")
        self.assertEqual(s.getName(), "test")
        from pylibconfig import TYPE_STRING
        self.assertEqual(s.getType(), TYPE_STRING)
        self.assertTrue(s.isScalar())
        if hasattr(s, 'isString'):
            self.assertTrue(s.isString())
        self.assertFalse(s.isNumber())
        self.assertEqual(s.getPath(), "test")

    def test_setting_navigation(self):
        """Test Setting parent/child navigation"""
        config = Config()
        config.addGroup("", "group")
        config.addString("group", "str")
        config.setValue("group.str", "value")

        group = config.lookup("group")
        self.assertEqual(group.getLength(), 1)
        try:
            child = group.lookup("str")
        except AttributeError:
            self.skipTest("Setting.lookup() not available (requires libconfig >= 1.5)")
            return
        self.assertEqual(child.getName(), "str")

    def test_setting_getValue(self):
        """Test Setting.getValue()"""
        config = Config()
        config.addInteger("", "i")
        config.setValue("i", 42)
        config.addString("", "s")
        config.setValue("s", "text")
        config.addBoolean("", "b")
        config.setValue("b", True)
        config.addFloat("", "f")
        config.setValue("f", 3.14)

        self.assertEqual(config.lookup("i").getValue(), 42)
        self.assertEqual(config.lookup("s").getValue(), "text")
        self.assertEqual(config.lookup("b").getValue(), True)
        self.assertTrue(fabs(config.lookup("f").getValue() - 3.14) < 0.0001)

    def test_options(self):
        """Test setOptions/getOptions/setOption/getOption (1.8+)"""
        config = Config()
        try:
            from pylibconfig import OPTION_FSYNC, OPTION_ALLOW_OVERRIDES
            config.setOption(OPTION_FSYNC, True)
            self.assertTrue(config.getOption(OPTION_FSYNC))
            config.setOptions(OPTION_FSYNC | OPTION_ALLOW_OVERRIDES)
            self.assertEqual(config.getOptions() & OPTION_FSYNC, OPTION_FSYNC)
            self.assertEqual(config.getOptions() & OPTION_ALLOW_OVERRIDES, OPTION_ALLOW_OVERRIDES)
        except ImportError:
            self.skipTest("Options not available (requires libconfig >= 1.8)")

    def test_format_and_precision(self):
        """Test setDefaultFormat/setFloatPrecision/setTabWidth (1.8+)"""
        config = Config()
        try:
            from pylibconfig import FORMAT_HEX
            config.setDefaultFormat(FORMAT_HEX)
            self.assertEqual(config.getDefaultFormat(), FORMAT_HEX)
        except (ImportError, AttributeError):
            self.skipTest("Format/Precision not available (requires libconfig >= 1.8)")

        try:
            config.setFloatPrecision(6)
            self.assertEqual(config.getFloatPrecision(), 6)
            config.setTabWidth(4)
            self.assertEqual(config.getTabWidth(), 4)
        except AttributeError:
            self.skipTest("Precision/TabWidth not available (requires libconfig >= 1.8)")

    def test_clear(self):
        """Test clear()"""
        config = Config()
        config.addString("", "test")
        config.setValue("test", "value")
        self.assertTrue(config.exists("test"))
        try:
            config.clear()
            self.assertFalse(config.exists("test"))
        except AttributeError:
            self.skipTest("clear() not available (requires libconfig >= 1.8)")

    def test_setting_source_info(self):
        """Test getSourceFile/getSourceLine"""
        config = Config()
        config.addString("", "test")
        config.setValue("test", "v")
        config.writeFile("_test_source.cfg")
        config2 = Config()
        config2.readFile("_test_source.cfg")
        s = config2.lookup("test")
        if hasattr(s, 'getSourceFile'):
            self.assertTrue(s.getSourceFile().endswith("_test_source.cfg"))
        self.assertGreater(s.getSourceLine(), 0)
        os.remove("_test_source.cfg")

    def test_add_with_empty_path(self):
        """Test add methods with empty path (root)"""
        config = Config()
        config.addString("", "s1")
        config.addInteger("", "i1")
        config.addBoolean("", "b1")
        config.addFloat("", "f1")
        config.addGroup("", "g1")
        config.addList("", "l1")
        config.addArray("", "a1")

        self.assertTrue(config.exists("s1"))
        self.assertTrue(config.exists("i1"))
        self.assertTrue(config.exists("b1"))
        self.assertTrue(config.exists("f1"))
        self.assertTrue(config.exists("g1"))
        self.assertTrue(config.exists("l1"))
        self.assertTrue(config.exists("a1"))
        from pylibconfig import TYPE_GROUP
        self.assertEqual(config.lookup("g1").getType(), TYPE_GROUP)

    def test_setting_format(self):
        """Test Setting-level setFormat/getFormat"""
        config = Config()
        config.addInteger("", "num")
        config.setValue("num", 255)
        try:
            from pylibconfig import FORMAT_HEX
            s = config.lookup("num")
            s.setFormat(FORMAT_HEX)
            self.assertEqual(s.getFormat(), FORMAT_HEX)
        except ImportError:
            self.skipTest("Format not available (requires libconfig >= 1.5)")

    def test_readString(self):
        """Test readString()"""
        config = Config()
        try:
            config.readString("test_string = \"hello\";")
        except AttributeError:
            self.skipTest("readString() not available (requires libconfig >= 1.4)")
            return
        self.assertEqual(config.lookupString("test_string"), "hello")

    def test_autoConvert(self):
        """Test getAutoConvert/setAutoConvert"""
        config = Config()
        self.assertFalse(config.getAutoConvert())
        config.setAutoConvert(True)
        self.assertTrue(config.getAutoConvert())

    def test_constants(self):
        """Test type/format/option constants are exported"""
        from pylibconfig import (TYPE_NONE, TYPE_INT, TYPE_INT64, TYPE_FLOAT,
                                  TYPE_STRING, TYPE_BOOLEAN, TYPE_GROUP,
                                  TYPE_ARRAY, TYPE_LIST)
        self.assertEqual(TYPE_NONE, 0)
        self.assertEqual(TYPE_INT, 1)
        self.assertEqual(TYPE_GROUP, 6)


if __name__ == '__main__':
    unittest.main()