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

import os
import unittest

class EndToEndTest(unittest.TestCase):
        
    def originalNames(self):
        # loop over original file, store names, write to generated file
        try:
            file = PacBioBam.BamFile(self.ex2BamFn)
            writer = PacBioBam.BamWriter(self.generatedBamFn, file.Header())
            
            dataset = PacBioBam.DataSet(self.ex2BamFn)
            entireFile = PacBioBam.EntireFileQuery(dataset)
         
            names_in = []
            for record in PacBioBam.Iterate(entireFile):
                names_in.append(record.FullName())
                writer.Write(record)
            return names_in
            
        except RuntimeError:
            self.assertTrue(False) # should not throw
        
    def generatedNames(self):
        try:   
            # open dataset on generated BAM file, read in names
            dataset = PacBioBam.DataSet(self.generatedBamFn)
            entireFile = PacBioBam.EntireFileQuery(dataset)
            names_out = []
            for record in PacBioBam.Iterate(entireFile):
                names_out.append(record.FullName())
            return names_out
                
        except RuntimeError:
            self.assertTrue(False) # should not throw
        
    def runTest(self):
        
        self.testData = config.TestData()
        self.ex2BamFn = self.testData.directory + "/aligned.bam"
        self.generatedBamFn = self.testData.directory + "/generated.bam"
        
        # compare input records to generated copy's records
        names_in = self.originalNames()
        names_out = self.generatedNames()
        self.assertEqual(names_in, names_out)
        
        # clean up
        os.remove(self.generatedBamFn)
