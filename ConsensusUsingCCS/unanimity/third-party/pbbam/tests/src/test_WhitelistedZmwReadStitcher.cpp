// Copyright (c) 2014-2016, Pacific Biosciences of California, Inc.
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
#include <pbbam/PbiRawData.h>
#include <pbbam/virtual/WhitelistedZmwReadStitcher.h>
#include <string>
using namespace PacBio;
using namespace PacBio::BAM;
using namespace std;

namespace PacBio {
namespace BAM {
namespace tests {

static
void Compare(const BamRecord& b1, const BamRecord& b2)
{
    EXPECT_TRUE(b1.HasDeletionQV());
    EXPECT_TRUE(b1.HasDeletionTag());
    EXPECT_TRUE(b1.HasInsertionQV());
    EXPECT_TRUE(b1.HasMergeQV());
    EXPECT_TRUE(b1.HasSubstitutionQV());
    EXPECT_TRUE(b1.HasSubstitutionTag());
    EXPECT_TRUE(b1.HasLabelQV());
    EXPECT_TRUE(b1.HasAltLabelQV());
    EXPECT_TRUE(b1.HasAltLabelTag());
    EXPECT_TRUE(b1.HasPkmean());
    EXPECT_TRUE(b1.HasPkmid());
    EXPECT_TRUE(b1.HasPulseCall());
    EXPECT_TRUE(b1.HasIPD());
    EXPECT_TRUE(b1.HasPulseWidth());
    EXPECT_TRUE(b1.HasPrePulseFrames());
    EXPECT_TRUE(b1.HasPulseCallWidth());
    EXPECT_TRUE(b1.HasPulseMergeQV());

    EXPECT_TRUE(b2.HasDeletionQV());
    EXPECT_TRUE(b2.HasDeletionTag());
    EXPECT_TRUE(b2.HasInsertionQV());
    EXPECT_TRUE(b2.HasMergeQV());
    EXPECT_TRUE(b2.HasSubstitutionQV());
    EXPECT_TRUE(b2.HasSubstitutionTag());
    EXPECT_TRUE(b2.HasLabelQV());
    EXPECT_TRUE(b2.HasAltLabelQV());
    EXPECT_TRUE(b2.HasAltLabelTag());
    EXPECT_TRUE(b2.HasPkmean());
    EXPECT_TRUE(b2.HasPkmid());
    EXPECT_TRUE(b2.HasPulseCall());
    EXPECT_TRUE(b2.HasIPD());
    EXPECT_TRUE(b2.HasPulseWidth());
    EXPECT_TRUE(b2.HasPrePulseFrames());
    EXPECT_TRUE(b2.HasPulseCallWidth());
    EXPECT_TRUE(b2.HasPulseMergeQV());

    EXPECT_EQ(b1.FullName(),        b2.FullName());
    EXPECT_EQ(b1.HoleNumber(),      b2.HoleNumber());
    EXPECT_EQ(b1.NumPasses(),       b2.NumPasses());
    EXPECT_EQ(b1.Sequence(),        b2.Sequence());
    EXPECT_EQ(b1.Qualities(),       b2.Qualities());
    EXPECT_EQ(b1.DeletionQV(),      b2.DeletionQV());
    EXPECT_EQ(b1.DeletionTag(),     b2.DeletionTag());
    EXPECT_EQ(b1.InsertionQV(),     b2.InsertionQV());
    EXPECT_EQ(b1.MergeQV(),         b2.MergeQV());
    EXPECT_EQ(b1.SubstitutionQV(),  b2.SubstitutionQV());
    EXPECT_EQ(b1.SubstitutionTag(), b2.SubstitutionTag());
    EXPECT_EQ(b1.LabelQV(),         b2.LabelQV());
    EXPECT_EQ(b1.AltLabelQV(),      b2.AltLabelQV());
    EXPECT_EQ(b1.AltLabelTag(),     b2.AltLabelTag());
    EXPECT_EQ(b1.Pkmean(),          b2.Pkmean());
    EXPECT_EQ(b1.Pkmid(),           b2.Pkmid());
    EXPECT_EQ(b1.PulseCall(),       b2.PulseCall());
    EXPECT_EQ(b1.IPD(),             b2.IPD());
    EXPECT_EQ(b1.PulseWidth(),      b2.PulseWidth());
    EXPECT_EQ(b1.PrePulseFrames(),  b2.PrePulseFrames());
    EXPECT_EQ(b1.PulseCallWidth(),  b2.PulseCallWidth());
    EXPECT_EQ(b1.ReadGroup(),       b2.ReadGroup());
    EXPECT_EQ(b1.PulseMergeQV(),    b2.PulseMergeQV());
}

} // namespace tests
} // namespace BAM
} // namespace PacBio

TEST(WhitelistedZmwReadStitching, EmptyList)
{
    const std::vector<int32_t> whitelist = { };
    WhitelistedZmwReadStitcher stitcher(whitelist,
                                        tests::Data_Dir + "/polymerase/internal.subreads.bam",
                                        tests::Data_Dir + "/polymerase/internal.scraps.bam");
    EXPECT_FALSE(stitcher.HasNext());
    EXPECT_TRUE(stitcher.NextRaw().empty());
}

TEST(WhitelistedZmwReadStitching, SingleValue)
{
    const std::vector<int32_t> whitelist = { 200000 };
    WhitelistedZmwReadStitcher stitcher(whitelist,
                                        tests::Data_Dir + "/polymerase/internal.subreads.bam",
                                        tests::Data_Dir + "/polymerase/internal.scraps.bam");

    // create virtual record
    EXPECT_TRUE(stitcher.HasNext());
    auto virtualRecord = stitcher.Next();
    EXPECT_FALSE(stitcher.HasNext());

    // fetch original polymerase read (2nd record)
    BamFile polyBam(tests::Data_Dir + "/polymerase/internal.polymerase.bam");
    EntireFileQuery polyQuery(polyBam);
    auto begin = polyQuery.begin();
    auto end = polyQuery.end();
    EXPECT_TRUE(begin != end);
    ++begin;
    EXPECT_TRUE(begin != end);
    auto polyRecord = *begin++;

    EXPECT_EQ(200000, virtualRecord.HoleNumber());

    tests::Compare(polyRecord, virtualRecord);
}

TEST(WhitelistedZmwReadStitching, UnknownZmw)
{
    const std::vector<int32_t> whitelist { 42 }; // ZMW not in our files
    WhitelistedZmwReadStitcher stitcher(whitelist,
                                        tests::Data_Dir + "/polymerase/internal.subreads.bam",
                                        tests::Data_Dir + "/polymerase/internal.scraps.bam");

    EXPECT_FALSE(stitcher.HasNext());
    EXPECT_TRUE(stitcher.NextRaw().empty());
}

TEST(WhitelistedZmwReadStitching, MultiValue)
{
    const std::vector<int32_t> whitelist = { 100000, 300000 };
    WhitelistedZmwReadStitcher stitcher(whitelist,
                                        tests::Data_Dir + "/polymerase/internal.subreads.bam",
                                        tests::Data_Dir + "/polymerase/internal.scraps.bam");


    // create virtual records
    EXPECT_TRUE(stitcher.HasNext());
    auto virtualRecord1 = stitcher.Next();
    EXPECT_TRUE(stitcher.HasNext());
    auto virtualRecord2 = stitcher.Next();
    EXPECT_FALSE(stitcher.HasNext());

    // fetch original polymerase reads (2nd record)
    BamFile polyBam(tests::Data_Dir + "/polymerase/internal.polymerase.bam");
    EntireFileQuery polyQuery(polyBam);
    auto begin = polyQuery.begin();
    auto end = polyQuery.end();

    EXPECT_TRUE(begin != end);
    auto polyRecord1 = *begin++;
    EXPECT_TRUE(begin != end);
    ++begin;
    EXPECT_TRUE(begin != end);
    auto polyRecord2 = *begin++;
    EXPECT_TRUE(begin == end);

    EXPECT_EQ(100000, virtualRecord1.HoleNumber());
    EXPECT_EQ(300000, virtualRecord2.HoleNumber());

    tests::Compare(polyRecord1, virtualRecord1);
    tests::Compare(polyRecord2, virtualRecord2);
}

TEST(WhitelistedZmwReadStitching, MultiValue_MixedKnownAndUnknown)
{
    const std::vector<int32_t> whitelist { 42, 200000, 24 };
    WhitelistedZmwReadStitcher stitcher(whitelist,
                                        tests::Data_Dir + "/polymerase/internal.subreads.bam",
                                        tests::Data_Dir + "/polymerase/internal.scraps.bam");

    // everything below should behave exactly as 'SingleValueOk' test,
    // as the unknown ZMWs will have been removed during construction

    // create virtual record
    EXPECT_TRUE(stitcher.HasNext());
    auto virtualRecord = stitcher.Next();
    EXPECT_FALSE(stitcher.HasNext());

    // fetch original polymerase read (2nd record)
    BamFile polyBam(tests::Data_Dir + "/polymerase/internal.polymerase.bam");
    EntireFileQuery polyQuery(polyBam);
    auto begin = polyQuery.begin();
    auto end = polyQuery.end();
    EXPECT_TRUE(begin != end);
    ++begin;
    EXPECT_TRUE(begin != end);
    auto polyRecord = *begin++;

    EXPECT_EQ(200000, virtualRecord.HoleNumber());

    tests::Compare(polyRecord, virtualRecord);
}

TEST(WhitelistedZmwReadStitching, EmptyScrapsFileOk)
{
    const std::vector<int32_t> whitelist = { 10944689, 10944690 };
    const std::string primaryBamFn = tests::Data_Dir + "/polymerase/scrapless.subreads.bam" ;
    const std::string scrapsBamFn  = tests::Data_Dir + "/polymerase/scrapless.scraps.bam" ;

    int count = 0;
    WhitelistedZmwReadStitcher stitcher(whitelist, primaryBamFn, scrapsBamFn);
    while (stitcher.HasNext()) {
        auto record = stitcher.Next();
        (void)record;
        ++count;
    }
    EXPECT_EQ(2, count);

    const BamFile primaryBam(primaryBamFn);
    const BamFile scrapsBam(scrapsBamFn);
    const PbiRawData primaryIdx(primaryBam.PacBioIndexFilename());
    const PbiRawData scrapsIdx(scrapsBam.PacBioIndexFilename());
    EXPECT_EQ(3, primaryIdx.NumReads());
    EXPECT_EQ(0, scrapsIdx.NumReads());
}
