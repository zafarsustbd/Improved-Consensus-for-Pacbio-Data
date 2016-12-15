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

#include "TestData.h"
#include <boost/any.hpp>
#include <gtest/gtest.h>
#include <pbbam/EntireFileQuery.h>
#include <pbbam/GenomicIntervalQuery.h>
#include <pbbam/ZmwQuery.h>
#include <pbbam/ZmwGroupQuery.h>
#include <pbbam/DataSet.h>
#include <string>
using namespace PacBio;
using namespace PacBio::BAM;
using namespace std;

const string alignedBamFn      = tests::Data_Dir + "/aligned.bam";
const string aligned2BamFn     = tests::Data_Dir + "/aligned2.bam";
const string alignedCopyBamFn  = tests::GeneratedData_Dir + "/aligned.bam";
const string aligned2CopyBamFn = tests::GeneratedData_Dir + "/aligned2.bam";

const string group_fofn   = tests::Generated_Dir + "/group.fofn";
const string group_file1  = tests::Data_Dir + "/group/test1.bam";
const string group_file2  = tests::Data_Dir + "/group/test2.bam";
const string group_file3  = tests::Data_Dir + "/group/test3.bam";

const vector<string> group_file1_names =
{
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/24962/0_427"
};

const vector<string> group_file2_names =
{
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/14743/2114_2531",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/14743/2579_4055",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/14743/4101_5571",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/14743/5615_6237"
};

const vector<string> group_file3_names =
{
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/45203/0_893",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/45203/0_893",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/46835/3759_4005",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/46835/4052_4686",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/46835/4732_4869",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/47698/9482_9628",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/47698/9675_10333",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/47698/10378_10609",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/49050/48_1132",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/49050/48_1132",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/49194/0_798",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/49194/845_1541",
    "m140905_042212_sidney_c100564852550000001823085912221377_s1_X0/49521/0_134"
};

static inline
bool InGroup(const string& name, const vector<string>& group)
{
    for (const string& s : group) {
        if (s == name)
            return true;
    }
    return false;
}

TEST(DataSetQueryTest, EntireFileQueryTest)
{
    // single file
    EXPECT_NO_THROW(
    {
        BamFile bamFile(alignedBamFn);

        DataSet dataset;
        dataset.ExternalResources().Add(bamFile);

        int count =0;
        EntireFileQuery query(dataset); // from DataSet object
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(4, count);

        count = 0;
        EntireFileQuery query2(alignedBamFn); // from BAM filename
        for (const BamRecord& record : query2) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(4, count);

        count = 0;
        EntireFileQuery query3(bamFile); // from BamFile object
        for (const BamRecord& record : query3) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(4, count);
    });

    // duplicate file attempt
    EXPECT_NO_THROW(
    {
        BamFile bamFile(alignedBamFn);

        DataSet dataset;
        dataset.ExternalResources().Add(bamFile);
        dataset.ExternalResources().Add(bamFile);

        int count =0;
        EntireFileQuery query(dataset);
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(4, count); // same as single
    });

    // true multi-file dataset
    EXPECT_NO_THROW(
    {
        BamFile file1(group_file1); // 1 read
        BamFile file2(group_file2); // 4 reads
        BamFile file3(group_file3); // 13 reads

        DataSet dataset;
        dataset.ExternalResources().Add(file1);
        dataset.ExternalResources().Add(file2);
        dataset.ExternalResources().Add(file3);

        int count = 0;
        EntireFileQuery query(dataset);
        for (const BamRecord& record : query) {

            // ensure sequential merge of files
            if (count == 0)     EXPECT_TRUE(InGroup(record.FullName(), group_file1_names));
            else if (count < 5) EXPECT_TRUE(InGroup(record.FullName(), group_file2_names));
            else                EXPECT_TRUE(InGroup(record.FullName(), group_file3_names));

            ++count;
        }
        EXPECT_EQ(18, count);
    });

    // same as above, from FOFN
    EXPECT_NO_THROW(
    {
        int count = 0;

        DataSet dataset(group_fofn);
        EntireFileQuery query(dataset);
        for (const BamRecord& record : query) {

            // ensure sequential merge of files
            if (count == 0)     EXPECT_TRUE(InGroup(record.FullName(), group_file1_names));
            else if (count < 5) EXPECT_TRUE(InGroup(record.FullName(), group_file2_names));
            else                EXPECT_TRUE(InGroup(record.FullName(), group_file3_names));

            ++count;
        }
        EXPECT_EQ(18, count);
    });
}

TEST(DataSetQueryTest, GenomicIntervalQueryTest)
{
    const string rname = "lambda_NEB3011";

    // single file
    EXPECT_NO_THROW(
    {
        DataSet dataset(alignedBamFn); // from BAM filename

        // count records
        int count = 0;
        GenomicInterval interval(rname, 5000, 6000);
        GenomicIntervalQuery query(interval, dataset);
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(2, count);

        // adjust interval and pass back in
        count = 0;
        interval.Start(9000);
        interval.Stop(9500);
        query.Interval(interval);
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(2, count);

        // unknown ref
        count = 0;
        interval.Name("does not exist");
        interval.Start(0);
        interval.Stop(100);
        EXPECT_THROW(query.Interval(interval), std::exception);
        for (const BamRecord& record : query) { // iteration is still safe, just returns no data
            (void)record;
            ++count;
        }
        EXPECT_EQ(0, count);

        // adjust again - make sure we can read a real region after an invalid one
        interval.Name(rname);
        interval.Start(5000);
        interval.Stop(6000);
        query.Interval(interval);
        count = 0;
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(2, count);
    });

    // duplicate file
    EXPECT_NO_THROW(
    {
        BamFile bamFile(alignedBamFn);

        DataSet dataset;
        dataset.ExternalResources().Add(bamFile);
        dataset.ExternalResources().Add(bamFile);

        // count records & also ensure sorted merge
        int count = 0;
        int prevId = 0;
        int prevPos = 0;

        GenomicInterval interval(rname, 5000, 6000);
        GenomicIntervalQuery query(interval, dataset);
        for (const BamRecord& record : query) {

            EXPECT_TRUE(record.ReferenceId()   >= prevId);
            EXPECT_TRUE(record.ReferenceStart() >= prevPos);

            prevId = record.ReferenceId();
            prevPos = record.ReferenceStart();
            ++count;
        }
        EXPECT_EQ(2, count); // same as single file

        // adjust interval and pass back in
        count = 0;
        interval.Start(9000);
        interval.Stop(10000);
        query.Interval(interval);
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(2, count); // same as single file

        // unknown ref
        count = 0;
        interval.Name("does not exist");
        interval.Start(0);
        interval.Stop(100);
        EXPECT_THROW(query.Interval(interval), std::exception);
        for (const BamRecord& record : query) { // iteration is still safe, just returns no data
            (void)record;
            ++count;
        }
        EXPECT_EQ(0, count); // same as single file

        // adjust again - make sure we can read a real region after an invalid one
        interval.Name(rname);
        interval.Start(5000);
        interval.Stop(5300);
        query.Interval(interval);
        count = 0;
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(2, count); // same as single file
    });

    // multi file BAM (same record content for easy testing, but different filename(ResourceId)
    EXPECT_NO_THROW(
    {
        BamFile bamFile(alignedBamFn);
        BamFile copyFile(alignedCopyBamFn);

        DataSet dataset;
        dataset.ExternalResources().Add(bamFile);
        dataset.ExternalResources().Add(copyFile);

        // count records & also ensure sorted merge
        int count = 0;
        int prevId = 0;
        int prevPos = 0;

        GenomicInterval interval(rname, 5000, 6000);
        GenomicIntervalQuery query(interval, dataset);
        for (const BamRecord& record : query) {

            EXPECT_TRUE(record.ReferenceId()   >= prevId);
            EXPECT_TRUE(record.ReferenceStart() >= prevPos);

            prevId = record.ReferenceId();
            prevPos = record.ReferenceStart();
            ++count;
        }
        EXPECT_EQ(4, count); // single file * 2

        // adjust interval and pass back in
        count = 0;
        interval.Start(9000);
        interval.Stop(10000);
        query.Interval(interval);
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(4, count); // single file * 2

        // unknown ref
        count = 0;
        interval.Name("does not exist");
        interval.Start(0);
        interval.Stop(100);
        EXPECT_THROW(query.Interval(interval), std::exception);
        for (const BamRecord& record : query) { // iteration is still safe, just returns no data
            (void)record;
            ++count;
        }
        EXPECT_EQ(0, count); // single file * 2

        // adjust again - make sure we can read a real region after an invalid one
        interval.Name(rname);
        interval.Start(5000);
        interval.Stop(5300);
        query.Interval(interval);
        count = 0;
        for (const BamRecord& record : query) {
            (void)record;
            ++count;
        }
        EXPECT_EQ(4, count); // single file * 2
    });
}

// TODO: implement me
TEST(DataSetQueryTest, QNameQueryTest)
{
    EXPECT_TRUE(true);
}

TEST(DataSetQueryTest, ZmwQueryTest)
{
    const std::vector<int32_t> whitelist = { 13473, 30983 };

    // single file
    EXPECT_NO_THROW(
    {
        BamFile bamFile(aligned2BamFn);
        ASSERT_TRUE(bamFile.PacBioIndexExists());
        DataSet dataset(bamFile);

        int count = 0;
        ZmwQuery query(whitelist, dataset);
        for (const BamRecord& record: query) {
            const int32_t holeNumber = record.HoleNumber();
            EXPECT_TRUE(holeNumber == 13473 || holeNumber == 30983);
            ++count;
        }
        EXPECT_EQ(4, count);
    });

    // multi-file
    EXPECT_NO_THROW(
    {
        BamFile bamFile(aligned2BamFn);
        BamFile bamFile2(aligned2CopyBamFn);
        ASSERT_TRUE(bamFile.PacBioIndexExists());
        ASSERT_TRUE(bamFile2.PacBioIndexExists());

        DataSet dataset;
        dataset.ExternalResources().Add(ExternalResource(bamFile));
        dataset.ExternalResources().Add(ExternalResource(bamFile2));

        int count = 0;
        ZmwQuery query(whitelist, dataset);
        for (const BamRecord& r : query) {
            const auto holeNumber = r.HoleNumber();
            EXPECT_TRUE(holeNumber == 13473 || holeNumber == 30983);
            ++count;
        }
        EXPECT_EQ(8, count);
    });
}

TEST(DataSetQueryTest, ZmwGroupQueryTest)
{
    const std::vector<int32_t> whitelist = { 13473, 30983 };

    // single-file
    EXPECT_NO_THROW(
    {
        BamFile bamFile(aligned2BamFn);
        ASSERT_TRUE(bamFile.PacBioIndexExists());
        DataSet dataset(bamFile);

        int count = 0;
        int32_t groupZmw = -1;
        ZmwGroupQuery query(whitelist, dataset);
        for (const vector<BamRecord>& group : query)  {
            for (const BamRecord& record: group) {
                const auto holeNumber = record.HoleNumber();
                if (groupZmw == -1)
                    groupZmw = holeNumber;
                EXPECT_TRUE(holeNumber == 13473 || holeNumber == 30983);
                EXPECT_EQ(groupZmw, holeNumber);
                ++count;
            }
            groupZmw = -1;
        }
        EXPECT_EQ(4, count);
    });

    // multi-file
    EXPECT_NO_THROW(
    {
        BamFile bamFile(aligned2BamFn);
        BamFile bamFile2(aligned2CopyBamFn);
        ASSERT_TRUE(bamFile.PacBioIndexExists());
        ASSERT_TRUE(bamFile2.PacBioIndexExists());

        DataSet dataset;
        dataset.ExternalResources().Add(ExternalResource(bamFile));
        dataset.ExternalResources().Add(ExternalResource(bamFile2));

        int totalCount = 0;
        int numRecordsInGroup = 0;
        int groupCount = 0;
        int32_t groupZmw = -1;
        ZmwGroupQuery query(whitelist, dataset);
        for (const vector<BamRecord>& group : query)  {
            for (const BamRecord& record: group) {
                const auto holeNumber = record.HoleNumber();
                ++numRecordsInGroup;
                if (groupZmw == -1)
                    groupZmw = holeNumber;
                EXPECT_TRUE(holeNumber == 13473 || holeNumber == 30983);
                EXPECT_EQ(groupZmw, holeNumber);
                ++totalCount;
            }
            if (groupCount == 0)
                EXPECT_EQ(4, numRecordsInGroup);
            else if (groupCount == 1)
                EXPECT_EQ(4, numRecordsInGroup);
            else
                EXPECT_TRUE(false); // should not get here
            numRecordsInGroup = 0;
            ++groupCount;
            groupZmw = -1;
        }
        EXPECT_EQ(8, totalCount);
    });
}
