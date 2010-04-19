# -*- coding: utf-8 -*-

import unittest
import os
from pylibconfig import Config

class PyLibConfigTest ( unittest.TestCase ):
    def test_greeting ( self ):
        config = Config ()
        
        config.addString ( "", "test" )
        config.setValue ( "test", "value" )
        
        self.assert_ ( config.value ( "test" )[0] == "value" )
        self.assert_ ( config.value ( "test" )[1] == True )
        self.assert_ ( config.value ( "loose" )[1] == False )
        
        config.writeFile ( "test.conf" )
        
        self.assert_ ( os.path.exists ( "./test.conf" ) == True )
        
        config.readFile ( "test.conf" )

        self.assert_ ( config.value ( "test" )[0] == "value" )
        self.assert_ ( config.value ( "test" )[1] == True )
        
        os.remove ( "./test.conf" )