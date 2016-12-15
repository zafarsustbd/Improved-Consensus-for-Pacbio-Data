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

#include <gtest/gtest.h>
#include <pbbam/DataSet.h>
#include <string>
using namespace PacBio;
using namespace PacBio::BAM;
using namespace std;

namespace tests {

static inline
DataSet CreateDataSet(void)
{
    DataSet d;
    d.Name("foo");
    return d;
}

} // namespace tests

TEST(DataSetCoreTest, XmlNameParts)
{
    internal::XmlName name("ns:node_name");
    EXPECT_EQ(boost::string_ref("ns"),           name.Prefix());
    EXPECT_EQ(boost::string_ref("node_name"),    name.LocalName());
    EXPECT_EQ(boost::string_ref("ns:node_name"), name.QualifiedName());

    internal::XmlName bareName("node_name");
    EXPECT_EQ(boost::string_ref(""),          bareName.Prefix());
    EXPECT_EQ(boost::string_ref("node_name"), bareName.LocalName());
    EXPECT_EQ(boost::string_ref("node_name"), bareName.QualifiedName());

    internal::XmlName leadingColon(":node_name");
    EXPECT_EQ(boost::string_ref(""),           leadingColon.Prefix());
    EXPECT_EQ(boost::string_ref(":node_name"), leadingColon.LocalName());
    EXPECT_EQ(boost::string_ref(":node_name"), leadingColon.QualifiedName());
}

TEST(DataSetCoreTest, DefaultsOk)
{
    DataSet dataset;
    EXPECT_EQ(DataSet::GENERIC, dataset.Type());
    EXPECT_FALSE(dataset.CreatedAt().empty());
    EXPECT_FALSE(dataset.MetaType().empty());
    EXPECT_FALSE(dataset.TimeStampedName().empty());
    EXPECT_FALSE(dataset.UniqueId().empty());
    EXPECT_FALSE(dataset.Version().empty());

    EXPECT_EQ(0, dataset.TimeStampedName().find("pacbio_dataset_"));

    EXPECT_TRUE(dataset.Format().empty());
    EXPECT_TRUE(dataset.ModifiedAt().empty());
    EXPECT_TRUE(dataset.Name().empty());
    EXPECT_TRUE(dataset.ResourceId().empty());
    EXPECT_TRUE(dataset.Tags().empty());
    EXPECT_EQ(0, dataset.ExternalResources().Size());
    EXPECT_EQ(0, dataset.Filters().Size());
    EXPECT_EQ(0, dataset.SubDataSets().Size());

    EXPECT_EQ(string{"3.0.1"}, dataset.Version());
}

TEST(DataSetCoreTest, TimeStampedNamesOk)
{
    DataSet dataset;
    AlignmentSet alignmentSet;
    BarcodeSet barcodeSet;
    ContigSet contigSet;
    ConsensusAlignmentSet consensusAlignmentSet;
    ConsensusReadSet consensusReadSet;
    HdfSubreadSet hdfSubreadSet;
    ReferenceSet referenceSet;
    SubreadSet subreadSet;

    EXPECT_EQ(0, dataset.TimeStampedName().find("pacbio_dataset_dataset-"));
    EXPECT_EQ(0, alignmentSet.TimeStampedName().find("pacbio_dataset_alignmentset-"));
    EXPECT_EQ(0, barcodeSet.TimeStampedName().find("pacbio_dataset_barcodeset-"));
    EXPECT_EQ(0, contigSet.TimeStampedName().find("pacbio_dataset_contigset-"));
    EXPECT_EQ(0, consensusAlignmentSet.TimeStampedName().find("pacbio_dataset_consensusalignmentset-"));
    EXPECT_EQ(0, consensusReadSet.TimeStampedName().find("pacbio_dataset_consensusreadset-"));
    EXPECT_EQ(0, hdfSubreadSet.TimeStampedName().find("pacbio_dataset_hdfsubreadset-"));
    EXPECT_EQ(0, referenceSet.TimeStampedName().find("pacbio_dataset_referenceset-"));
    EXPECT_EQ(0, subreadSet.TimeStampedName().find("pacbio_dataset_subreadset-"));
}

TEST(DataSetCoreTest, BasicGettersSettersOk)
{
    DataSet dataset;
    dataset.CreatedAt("now");
    dataset.Format("format");
    dataset.MetaType("meta");
    dataset.ModifiedAt("later");
    dataset.Name("foo");
    dataset.ResourceId("path/to/file");
    dataset.Tags("tag tag");
    dataset.TimeStampedName("now:30");
    dataset.UniqueId("uuid");
    dataset.Version("0.0.0");

    EXPECT_EQ(string("now"),          dataset.CreatedAt());
    EXPECT_EQ(string("format"),       dataset.Format());
    EXPECT_EQ(string("meta"),         dataset.MetaType());
    EXPECT_EQ(string("later"),        dataset.ModifiedAt());
    EXPECT_EQ(string("foo"),          dataset.Name());
    EXPECT_EQ(string("path/to/file"), dataset.ResourceId());
    EXPECT_EQ(string("tag tag"),      dataset.Tags());
    EXPECT_EQ(string("now:30"),       dataset.TimeStampedName());
    EXPECT_EQ(string("uuid"),         dataset.UniqueId());
    EXPECT_EQ(string("0.0.0"),        dataset.Version());
}

TEST(DataSetCoreTest, CopyOk)
{
    DataSet d1;
    d1.Name("foo");

    // copy ctor
    DataSet d2(d1);
    EXPECT_EQ(string("foo"), d2.Name());

    // copy assignment
    DataSet d3;
    d3 = d1;
    EXPECT_EQ(string("foo"), d3.Name());
}

TEST(DataSetCoreTest, MoveOk)
{
    DataSet d1;
    d1.Name("foo");

    // move ctor
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpessimizing-move"
#endif
    DataSet d2(std::move(tests::CreateDataSet()));
#ifdef __clang__
#pragma clang diagnostic pop
#endif
    EXPECT_EQ(string("foo"), d2.Name());

    // move assignment
    DataSet d3;
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpessimizing-move"
#endif
    d3 = std::move(tests::CreateDataSet());
#ifdef __clang__
#pragma clang diagnostic pop
#endif 
    EXPECT_EQ(string("foo"), d3.Name());
}

TEST(DataSetCoreTest, AddExternalResources)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.ExternalResources().Size());

    ExternalResource resource1("metatype", "id");
    resource1.Name("file1");

    ExternalResource resource2("metatype", "id2");
    resource2.Name("file2");

    dataset.ExternalResources().Add(resource1);
    dataset.ExternalResources().Add(resource2);
    EXPECT_EQ(2, dataset.ExternalResources().Size());

     // disallow duplicates (checking on ResourceId)
    ExternalResource duplicateResource("metatype", "id");
    dataset.ExternalResources().Add(duplicateResource);
    EXPECT_EQ(2, dataset.ExternalResources().Size());

    // direct access
    const ExternalResources& resources = dataset.ExternalResources();
    EXPECT_EQ(string("file1"), resources[0].Name());
    EXPECT_EQ(string("file2"), resources[1].Name());

    // iterable
    size_t i = 0;
    for (auto r : resources) {
        if (i == 0)
            EXPECT_EQ(string("file1"), r.Name());
        else
            EXPECT_EQ(string("file2"), r.Name());
        ++i;
    }
}

TEST(DataSetCoreTest, EditExternalResources)
{
    DataSet dataset;

    ExternalResource resource("metatype", "id");
    resource.Name("file1");
    dataset.ExternalResources().Add(resource);

    resource.Name("file2").ResourceId("id2");
    dataset.ExternalResources().Add(resource);
    EXPECT_EQ(2, dataset.ExternalResources().Size());

    // edit
    dataset.ExternalResources()[0].Name("some new name");
    EXPECT_EQ(string("some new name"), dataset.ExternalResources()[0].Name());
    EXPECT_EQ(string("file2"),         dataset.ExternalResources()[1].Name());
}

TEST(DataSetCoreTest, NestedExternalResources)
{
    ExternalResource resource("metatype", "filename");
    resource.ExternalResources().Add(ExternalResource("metatype.child",  "filename.child"));
    resource.ExternalResources().Add(ExternalResource("metatype.child2", "filename.child2"));

    const ExternalResources& childResources = resource.ExternalResources();
    EXPECT_EQ(2, childResources.Size());
    EXPECT_EQ(string("metatype.child"),  childResources[0].MetaType());
    EXPECT_EQ(string("metatype.child2"), childResources[1].MetaType());
    EXPECT_EQ(string("filename.child"),  childResources[0].ResourceId());
    EXPECT_EQ(string("filename.child2"), childResources[1].ResourceId());
}

TEST(DataSetCoreTest, AddFilters)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.Filters().Size());

    Filter filter;
    filter.Properties().Add(Property("rq", "0.85", ">"));
    filter.Properties().Add(Property("RNAME", "chr1", "=="));
    EXPECT_EQ(2, filter.Properties().Size());

    Filter filter2;
    filter2.Properties().Add(Property("rq", "0.50", ">="));
    filter2.Properties().Add(Property("RNAME", "chr2", "!="));
    EXPECT_EQ(2, filter2.Properties().Size());

    dataset.Filters().Add(filter);
    dataset.Filters().Add(filter2);

    const Filters& filters = dataset.Filters();
    EXPECT_EQ(2, filters.Size());
    EXPECT_EQ(2, filters[0].Properties().Size());
    EXPECT_EQ(2, filters[1].Properties().Size());

    // direct access
    const Property& p0 = filters[0].Properties()[0];
    EXPECT_EQ(string("rq"),   p0.Name());
    EXPECT_EQ(string("0.85"), p0.Value());
    EXPECT_EQ(string(">"),    p0.Operator());

    const Property& p1 = filters[0].Properties()[1];
    EXPECT_EQ(string("RNAME"), p1.Name());
    EXPECT_EQ(string("chr1"),  p1.Value());
    EXPECT_EQ(string("=="),    p1.Operator());

    const Property& p2 = filters[1].Properties()[0];
    EXPECT_EQ(string("rq"),   p2.Name());
    EXPECT_EQ(string("0.50"), p2.Value());
    EXPECT_EQ(string(">="),   p2.Operator());

    const Property& p3 = filters[1].Properties()[1];
    EXPECT_EQ(string("RNAME"), p3.Name());
    EXPECT_EQ(string("chr2"),  p3.Value());
    EXPECT_EQ(string("!="),    p3.Operator());

    // iteratable
    size_t i = 0;
    size_t j = 0;
    for (const Filter& f : filters) {
        if (i == 0) {
            const Properties& properties = f.Properties();
            for (const Property& p : properties) {
                if (j == 0) {
                    EXPECT_EQ(string("rq"),   p.Name());
                    EXPECT_EQ(string("0.85"), p.Value());
                    EXPECT_EQ(string(">"),    p.Operator());
                } else {
                    EXPECT_EQ(string("RNAME"), p.Name());
                    EXPECT_EQ(string("chr1"),  p.Value());
                    EXPECT_EQ(string("=="),    p.Operator());
                }
                ++j;
            }
        } else {
            const Properties& properties = f.Properties();
            for (const Property& p : properties) {
                if (j == 0) {
                    EXPECT_EQ(string("rq"),   p.Name());
                    EXPECT_EQ(string("0.50"), p.Value());
                    EXPECT_EQ(string(">="),   p.Operator());
                } else {
                    EXPECT_EQ(string("RNAME"), p.Name());
                    EXPECT_EQ(string("chr2"),  p.Value());
                    EXPECT_EQ(string("!="),    p.Operator());
                }
                ++j;
            }
        }
        ++i;
        j = 0;
    }

}

TEST(DataSetCoreTest, EditFilters)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.Filters().Size());

    Filter filter;
    filter.Properties().Add(Property("rq", "0.85", ">"));
    filter.Properties().Add(Property("RNAME", "chr1", "=="));
    EXPECT_EQ(2, filter.Properties().Size());

    Filter filter2;
    filter2.Properties().Add(Property("rq", "0.50", ">="));
    filter2.Properties().Add(Property("RNAME", "chr2", "!="));
    EXPECT_EQ(2, filter2.Properties().Size());

    dataset.Filters().Add(filter);
    dataset.Filters().Add(filter2);
    EXPECT_EQ(2, dataset.Filters().Size());
    EXPECT_EQ(2, dataset.Filters()[0].Properties().Size());
    EXPECT_EQ(2, dataset.Filters()[1].Properties().Size());

    // edit property in-place
    Property& p = dataset.Filters()[0].Properties()[0];
    p.Name("someNewName");
    p.Value("someNewValue");
    p.Operator("==");

    const Property& p0 = dataset.Filters()[0].Properties()[0];
    EXPECT_EQ(string("someNewName"),  p0.Name());
    EXPECT_EQ(string("someNewValue"), p0.Value());
    EXPECT_EQ(string("=="),           p0.Operator());

    const Property& p1 = dataset.Filters()[0].Properties()[1];
    EXPECT_EQ(string("RNAME"), p1.Name());
    EXPECT_EQ(string("chr1"),  p1.Value());
    EXPECT_EQ(string("=="),    p1.Operator());

    const Property& p2 = dataset.Filters()[1].Properties()[0];
    EXPECT_EQ(string("rq"),   p2.Name());
    EXPECT_EQ(string("0.50"), p2.Value());
    EXPECT_EQ(string(">="),   p2.Operator());

    const Property& p3 = dataset.Filters()[1].Properties()[1];
    EXPECT_EQ(string("RNAME"), p3.Name());
    EXPECT_EQ(string("chr2"),  p3.Value());
    EXPECT_EQ(string("!="),    p3.Operator());
}

TEST(DataSetCoreTest, AddSubDataSets)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.SubDataSets().Size());

    DataSetBase sub1;
    sub1.Name("subset_1");

    DataSetBase sub2;
    sub2.Name("subset_2");

    dataset.SubDataSets().Add(sub1);
    dataset.SubDataSets().Add(sub2);
    EXPECT_EQ(2, dataset.SubDataSets().Size());

    // direct access
    const SubDataSets& subdatasets = dataset.SubDataSets();
    EXPECT_EQ(string("subset_1"), subdatasets[0].Name());
    EXPECT_EQ(string("subset_2"), subdatasets[1].Name());

    // iterable
    size_t i = 0;
    for (const DataSetBase& ds : subdatasets) {
        if (i == 0)
            EXPECT_EQ(string("subset_1"), ds.Name());
        else
            EXPECT_EQ(string("subset_2"), ds.Name());
        ++i;
    }
}

TEST(DataSetCoreTest, EditSubDataSets)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.SubDataSets().Size());

    DataSetBase sub1;
    sub1.Name("subset_1");

    DataSetBase sub2;
    sub2.Name("subset_2");

    dataset.SubDataSets().Add(sub1);
    dataset.SubDataSets().Add(sub2);
    EXPECT_EQ(2, dataset.SubDataSets().Size());

    // edit
    dataset.SubDataSets()[0].Name("subset_1_edited");

    // direct access
    const SubDataSets& subdatasets = dataset.SubDataSets();
    EXPECT_EQ(string("subset_1_edited"), subdatasets[0].Name());
    EXPECT_EQ(string("subset_2"), subdatasets[1].Name());

    // iterable
    size_t i = 0;
    for (const DataSetBase& ds : subdatasets) {
        if (i == 0)
            EXPECT_EQ(string("subset_1_edited"), ds.Name());
        else
            EXPECT_EQ(string("subset_2"), ds.Name());
        ++i;
    }
}

TEST(DataSetCoreTest, RemoveExternalResources)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.ExternalResources().Size());

    ExternalResource resource1("metatype", "id");
    resource1.Name("file1");

    ExternalResource resource2("metatype", "id2");
    resource2.Name("file2");

    dataset.ExternalResources().Add(resource1);
    dataset.ExternalResources().Add(resource2);
    EXPECT_EQ(2, dataset.ExternalResources().Size());

    // remove
    dataset.ExternalResources().Remove(resource1);
    EXPECT_EQ(1, dataset.ExternalResources().Size());

    // direct access
    const ExternalResources& resources = dataset.ExternalResources();
    EXPECT_EQ(string("file2"), resources[0].Name());

    // iterable
    size_t i = 0;
    for (auto r : resources) {
        if (i == 0)
            EXPECT_EQ(string("file2"), r.Name());
        ++i;
    }
}

TEST(DataSetCoreTest, RemoveFilters)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.Filters().Size());

    Filter filter;
    filter.Properties().Add(Property("rq", "0.85", ">"));
    filter.Properties().Add(Property("RNAME", "chr1", "=="));
    EXPECT_EQ(2, filter.Properties().Size());

    Filter filter2;
    filter2.Properties().Add(Property("rq", "0.50", ">="));
    filter2.Properties().Add(Property("RNAME", "chr2", "!="));
    EXPECT_EQ(2, filter2.Properties().Size());

    dataset.Filters().Add(filter);
    dataset.Filters().Add(filter2);
    EXPECT_EQ(2, dataset.Filters().Size());

    // remove
    dataset.Filters().Remove(filter);
    EXPECT_EQ(1, dataset.Filters().Size());

    const Filters& filters = dataset.Filters();
    EXPECT_EQ(2, filters[0].Properties().Size());
}

TEST(DataSetCoreTest, RemoveSubDataSets)
{
    DataSet dataset;
    EXPECT_EQ(0, dataset.SubDataSets().Size());

    DataSetBase sub1;
    sub1.Name("subset_1");

    DataSetBase sub2;
    sub2.Name("subset_2");

    dataset.SubDataSets().Add(sub1);
    dataset.SubDataSets().Add(sub2);
    EXPECT_EQ(2, dataset.SubDataSets().Size());

    // remove
    dataset.SubDataSets().Remove(sub2);
    EXPECT_EQ(1, dataset.SubDataSets().Size());
}
