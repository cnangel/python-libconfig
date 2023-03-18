# -*- coding: utf-8 -*-
""" unittest """

import unittest
import os
from math import fabs
from pylibconfig import Config


class PyLibConfigTest(unittest.TestCase):
    """unittest libconfig for python"""

    def test_greeting(self):
        """test_greeting"""
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
        config.setValue("test_float", 2.1002)

        self.assertTrue(config.value("test")[0] == "value")
        self.assertTrue(config.value("test")[1])
        self.assertTrue(config.value("test_bool")[0])
        self.assertEqual(config.value("loose")[1], False)
        self.assertTrue(config.value("test_int")[0] == 9)
        self.assertTrue(config.value("test_unsigned_int")[0] == -9)
        self.assertTrue(fabs(config.value("test_float")[0] - 2.1002) < 0.00001)

        config.setValue("test_bool", False)

        config.writeFile("test.conf")

        self.assertEqual(os.path.exists("./test.conf"), True)

        config.readFile("test.conf")

        self.assertTrue(config.value("test")[0] == "value")
        self.assertTrue(config.value("test")[1])

        os.remove("./test.conf")
