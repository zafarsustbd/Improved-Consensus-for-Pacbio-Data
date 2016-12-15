// Copyright (c) 2014-2015, Pacific Biosciences of California, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted (subject to the limitations in the
// disclaimer below) provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//  * Neither the name of Pacific Biosciences nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
// GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY PACIFIC
// BIOSCIENCES AND ITS CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL PACIFIC BIOSCIENCES OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// Author: Derek Barnett

#ifdef PBBAM_TESTING
#define private public
#endif

#include "TestData.h"
#include <gtest/gtest.h>
#include <pbbam/BamFile.h>
#include <pbbam/EntireFileQuery.h>
#include <pbbam/../../src/FileUtils.h>
#include <stdexcept>
#include <cstdlib>
#include <unistd.h>
using namespace PacBio;
using namespace PacBio::BAM;
using namespace std;

namespace PacBio {
namespace BAM {
namespace tests {

template<typename T>
void CheckFile(const T& input, const size_t expectedCount)
{
    size_t observedCount = 0;
    EntireFileQuery entireFile(input);
    for (const BamRecord& r : entireFile) {
        (void)r;
        ++observedCount;
    }
    EXPECT_EQ(expectedCount, observedCount);
}

} // namespace tests
} // namespace BAM
} // namespace PacBio

TEST(BamFileTest, NonExistentFileThrows)
{
    EXPECT_THROW(BamFile{ "does_not_exist.bam" }, std::runtime_error);
}

TEST(BamFileTest, NonBamFileThrows)
{
    EXPECT_THROW(BamFile { tests::Data_Dir + "/lambdaNEB.fa.fai" }, std::runtime_error);
}

TEST(BamFileTest, RelativePathBamOk)
{
    // cache current working directory, then drill down so we can point to
    // BAMs using relative path
    const string cwd = internal::FileUtils::CurrentWorkingDirectory();
    ASSERT_EQ(0, chdir(tests::Data_Dir.c_str()));
    ASSERT_EQ(0, chdir("relative/a"));

    // BamFile from relative BAM fn
    tests::CheckFile(BamFile{ "../b/test1.bam" }, 3);

    // dataset from relative BAM fn
    tests::CheckFile(DataSet{ "../b/test1.bam" }, 3);

    // dataset from BamFile object (itself from relative BAM fn)
    {
        auto file = BamFile{"../b/test1.bam"};
        tests::CheckFile(DataSet{ file }, 3);
    }

    // restore working directory
    ASSERT_EQ(0, chdir(cwd.c_str()));
}

TEST(BamFileTest, RelativePathXmlOk)
{
    // cache current working directory, then drill down so we can point to
    // BAMs using relative path
    const string cwd = internal::FileUtils::CurrentWorkingDirectory();
    ASSERT_EQ(0, chdir(tests::Data_Dir.c_str()));

    // dataset from XML containing relative paths
    tests::CheckFile(DataSet{ "relative/relative.xml" }, 9);

    // restore working directory
    ASSERT_EQ(0, chdir(cwd.c_str()));
}

TEST(BamFileTest, RelativePathFofnOk)
{
    // cache current working directory, then drill down so we can point to
    // BAMs using relative path
    const string cwd = internal::FileUtils::CurrentWorkingDirectory();
    ASSERT_EQ(0, chdir(tests::Data_Dir.c_str()));

    // dataset from FOFN containing relative paths
    tests::CheckFile(DataSet{ "relative/relative.fofn" }, 9);

    // NOTE: doesn't yet support a FOFN containing an XML with relative paths
//       tests::CheckFile(DataSet{ "relative/relative2.fofn" }, 60);

    // restore working directory
    ASSERT_EQ(0, chdir(cwd.c_str()));
}

TEST(BamFileTest, TruncatedFileThrowsOk)
{
    EXPECT_THROW(BamFile{ tests::GeneratedData_Dir + "/truncated.bam" }, std::runtime_error);
}
