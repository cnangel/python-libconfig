# -*- coding: utf-8 -*-

import unittest
import os
from math import fabs
from pylibconfig import Config

class PyLibConfigTest ( unittest.TestCase ):
    def test_greeting ( self ):
        config = Config ()
        
        config.addString ( "", "test" )
        config.setValue ( "test", "value" )
        config.addBoolean ( "", "test_bool")
        config.setValue ( "test_bool", True )
        config.addInteger ( "", "test_int")
        config.setValue ( "test_int", 9 )
        config.addInteger ( "", "test_unsigned_int")
        config.setValue ( "test_unsigned_int", -9 )
        config.addFloat ( "", "test_float")
        config.setValue ( "test_float", 82.1002 )
        
        self.assert_ ( config.value ( "test" )[0] == "value" )
        self.assert_ ( config.value ( "test" )[1] == True )
        self.assert_ ( config.value ( "test_bool" )[0] == True )
        self.assert_ ( config.value ( "loose" )[1] == False )
        self.assert_ ( config.value ( "test_int" )[0] == 9 )
        self.assert_ ( config.value ( "test_unsigned_int" )[0] == -9 )
        self.assert_ ( fabs( config.value ( "test_float" )[0] - 82.1002) < 0.00001 )

        config.setValue( "test_bool", False )
        
        config.writeFile ( "test.conf" )
        
        self.assert_ ( os.path.exists ( "./test.conf" ) == True )
        
        config.readFile ( "test.conf" )

        self.assert_ ( config.value ( "test" )[0] == "value" )
        self.assert_ ( config.value ( "test" )[1] == True )
        
        os.remove ( "./test.conf" )
