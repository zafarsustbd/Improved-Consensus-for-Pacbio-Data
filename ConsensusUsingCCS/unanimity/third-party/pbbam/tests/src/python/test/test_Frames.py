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

class FramesTest(unittest.TestCase):
    
    # ------------ SETUP --------------
    
    def setUp(self):
        self.testframes = [
            0,  8,  140, 0,   0,  7,   4,  0,  85, 2,
            1,  3,  2,   10,  1,  20,  47, 10, 9,  60,
            20, 3,  12,  5,   13, 165, 6,  14, 22, 12,
            2,  4,  9,   218, 27, 3,   15, 2,  17, 2,
            45, 24, 89,  10,  7,  1,   11, 15, 0,  7,
            0,  28, 17,  12,  6,  10,  37, 0,  12, 52,
            0,  7,  1,   14,  3,  26,  12, 0,  20, 17,
            2,  13, 2,   9,   13, 7,   15, 29, 3,   6,
            2,  1,  28,  10,  3,  14,  7,  1,  22, 1,
            6,  6,  0,   19,  31, 6,   2,  14, 0,  0,
            1000, 947, 948
        ]

        self.encoded  = [
            0,     8,  102,   0,   0,   7,   4,   0,  75,   2,   1,   3,   2,
            10,    1,   20,  47,  10,   9,  60,  20,   3,  12,   5,  13, 115,
            6,    14,   22,  12,   2,   4,   9, 135,  27,   3,  15,   2,  17,
            2,    45,   24,  77,  10,   7,   1,  11,  15,   0,   7,   0,  28,
            17,   12,    6,  10,  37,   0,  12,  52,   0,   7,   1,  14,   3,
            26,   12,    0,  20,  17,   2,  13,   2,   9,  13,   7,  15,  29,
            3,     6,    2,   1,  28,  10,   3,  14,   7,   1,  22,   1,   6,
            6,     0,   19,  31,   6,   2,  14,   0,   0,
            255, 254,  255
        ]
        
    def runTest(self):
        self.test_ctors()
        self.test_encode()
        
    # ------------ TESTS --------------
    
    def test_ctors(self):
        f = PacBioBam.Frames()
        self.assertEqual(0, len(f.Data()))

        f2 = PacBioBam.Frames(self.testframes)
        d  = f2.Data()
        self.assertEqual(len(self.testframes), len(d))
        for i, v in enumerate(d):
            self.assertEqual(int(self.testframes[i]), int(v))
    
    def test_encode(self):
        f = PacBioBam.Frames(self.testframes)
        e = f.Encode()
        self.assertEqual(len(self.encoded), len(e))
        for i, v in enumerate(e):
            self.assertEqual(int(self.encoded[i]), int(v))
        