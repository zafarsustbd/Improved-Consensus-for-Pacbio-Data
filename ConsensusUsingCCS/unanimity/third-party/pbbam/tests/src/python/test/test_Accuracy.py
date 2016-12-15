# Copyright (c) 2014-2015, Pacific Biosciences of California, Inc.
#
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted (subject to the limitations in the
# disclaimer below) provided that the following conditions are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#  * Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#
#  * Neither the name of Pacific Biosciences nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
# GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
# BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
# USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
# Author: Derek Barnett

import PacBioBam
import config 
import unittest

class AccuracyTest(unittest.TestCase):
    
    # ------------ SETUP --------------
    
    def runTest(self):
        self.test_clamp()
        
    # ------------ TESTS --------------
        
    def test_clamp(self):
        a_zero     = PacBioBam.Accuracy(0.0)
        a_neg      = PacBioBam.Accuracy(-0.5)
        a_min      = PacBioBam.Accuracy(0.0)
        a_normal   = PacBioBam.Accuracy(0.9)
        a_max      = PacBioBam.Accuracy(1.0)
        a_tooLarge = PacBioBam.Accuracy(1.1)
        
        self.assertAlmostEqual(float(0.0), float(a_zero))
        self.assertAlmostEqual(float(0.0), float(a_neg))
        self.assertAlmostEqual(float(0.0), float(a_min))
        self.assertAlmostEqual(float(0.9), float(a_normal))
        self.assertAlmostEqual(float(1.0), float(a_max))
        self.assertAlmostEqual(float(1.0), float(a_tooLarge))
        