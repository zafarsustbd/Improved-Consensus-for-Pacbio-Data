// Copyright (c) 2015, Pacific Biosciences of California, Inc.
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
// DISCLAIMED. IN NO EVENT  SHALL PACIFIC BIOSCIENCES OR ITS
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
// USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
// OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

// Author: Derek Barnett

#include "BamFileMerger.h"

#include <pbbam/BamHeader.h>
#include <pbbam/BamReader.h>
#include <pbbam/BamRecord.h>
#include <pbbam/BamWriter.h>
#include <pbbam/CompositeBamReader.h>
#include <pbbam/PbiBuilder.h>

#include <deque>
#include <memory>
#include <stdexcept>
#include <cassert>

namespace PacBio {
namespace BAM {
namespace common {

// ICollator

class ICollator
{
public:
    ~ICollator(void) { }

    bool GetNext(BamRecord& record)
    {
        // nothing left to read
        if (mergeItems_.empty())
            return false;

        // non-destructive 'pop' of first item from queue
        auto firstIter = mergeItems_.begin();
        auto firstItem = PacBio::BAM::internal::CompositeMergeItem{ std::move(firstIter->reader),
                                                                    std::move(firstIter->record)
                                                                  };
        mergeItems_.pop_front();

        // store its record in our output record
        std::swap(record, firstItem.record);

        // try fetch 'next' from first item's reader
        // if successful, re-insert it into container & re-sort on our new values
        // otherwise, this item will go out of scope & reader destroyed
        if (firstItem.reader->GetNext(firstItem.record)) {
            mergeItems_.push_front(std::move(firstItem));
            UpdateSort();
        }

        // return success
        return true;
    }

protected:
    std::deque<PacBio::BAM::internal::CompositeMergeItem> mergeItems_;

protected:
    ICollator(std::vector<std::unique_ptr<PacBio::BAM::BamReader> >&& readers)
    {
        for (auto&& reader : readers) {
            auto item = internal::CompositeMergeItem{std::move(reader)};
            if (item.reader->GetNext(item.record))
                mergeItems_.push_back(std::move(item));
        }
    }

    virtual void UpdateSort(void) =0;
};

// QNameCollator

struct QNameSorter : std::binary_function<internal::CompositeMergeItem,
                                          internal::CompositeMergeItem,
                                          bool>
{
    bool operator()(const internal::CompositeMergeItem& lhs,
                    const internal::CompositeMergeItem& rhs)
    {
        const BamRecord& l = lhs.record;
        const BamRecord& r = rhs.record;

        // movie name
        const int cmp = l.MovieName().compare(r.MovieName());
        if (cmp != 0)
            return cmp < 0;

        // hole number
        const auto lhsZmw = l.HoleNumber();
        const auto rhsZmw = r.HoleNumber();
        if (lhsZmw != rhsZmw)
            return lhsZmw < rhsZmw;

        // shuffle CCS reads after all others
        const auto lhsReadType = l.Type();
        const auto rhsReadType = r.Type();
        if (lhsReadType == RecordType::CCS)
            return false;
        if (rhsReadType == RecordType::CCS)
            return true;

        // sort on qStart, then finally qEnd
        const auto lhsQStart = l.QueryStart();
        const auto rhsQStart = r.QueryStart();
        return lhsQStart < rhsQStart;
    }
};

class QNameCollator : public ICollator
{
public:
    QNameCollator(std::vector<std::unique_ptr<PacBio::BAM::BamReader>>&& readers)
        : ICollator(std::move(readers))
    { UpdateSort(); }

    void UpdateSort(void)
    { std::sort(mergeItems_.begin(), mergeItems_.end(), QNameSorter{ }); }
};

// AlignedCollator

class AlignedCollator : public ICollator
{
public:
    AlignedCollator(std::vector<std::unique_ptr<PacBio::BAM::BamReader>>&& readers)
        : ICollator(std::move(readers))
    { UpdateSort(); }

    void UpdateSort(void)
    { std::sort(mergeItems_.begin(), mergeItems_.end(), PacBio::BAM::PositionSorter{ }); }
};

// BamFileMerger

inline
void BamFileMerger::Merge(const DataSet& dataset,
                          const std::string& outputFilename,
                          const ProgramInfo& mergeProgram,
                          bool createPbi)
{
    const PbiFilter filter = PbiFilter::FromDataSet(dataset);

    std::vector<std::string> inputFilenames_;
    const auto& bamFiles = dataset.BamFiles();
    inputFilenames_.reserve(bamFiles.size());
    for (const auto& file : bamFiles)
        inputFilenames_.push_back(file.Filename());

    if (inputFilenames_.empty())
        throw std::runtime_error("no input filenames provided to BamFileMerger");

    if (outputFilename.empty())
        throw std::runtime_error("no output filename provide to BamFileMerger");


    // attempt open input files
    std::vector<std::unique_ptr<BamReader> > readers;
    readers.reserve(inputFilenames_.size());
    for (const auto& fn : inputFilenames_) {
        if (filter.IsEmpty())
            readers.emplace_back(new BamReader(fn));
        else
            readers.emplace_back(new PbiIndexedBamReader(filter, fn));
    }

    // read headers
    std::vector<BamHeader> headers;
    headers.reserve(readers.size());
    for (auto&& reader : readers)
        headers.push_back(reader->Header());

    assert(!readers.empty());
    assert(!headers.empty());

    // merge headers
    BamHeader mergedHeader = headers.front();
    const std::string& usingSortOrder = mergedHeader.SortOrder();
    const bool isCoordinateSorted = (usingSortOrder == "coordinate");
    for (size_t i = 1; i < headers.size(); ++i) {
        const BamHeader& header = headers.at(i);
        if (header.SortOrder() != usingSortOrder)
            throw std::runtime_error("BAM file sort orders do not match, aborting merge");
        mergedHeader += headers.at(i);
    }
    if (mergeProgram.IsValid())
        mergedHeader.AddProgram(mergeProgram);

    // setup collator, based on sort order
    std::unique_ptr<ICollator> collator;
    if (isCoordinateSorted)
        collator.reset(new AlignedCollator(std::move(readers)));
    else
        collator.reset(new QNameCollator(std::move(readers)));
    // NOTE: readers *moved*, so no longer accessible here

    // do merge, creating PBI on-the-fly
    if (createPbi && (outputFilename != "-")) {

        // TODO: this implementation recalculates all PBI values, when we really
        //       only need to collate entries and update offsets

        BamWriter writer(outputFilename, mergedHeader);
        PbiBuilder builder{ (outputFilename + ".pbi"),
                            mergedHeader.NumSequences(),
                            isCoordinateSorted
                          };
        BamRecord record;
        int64_t vOffset = 0;
        while (collator->GetNext(record)) {
            writer.Write(record, &vOffset);
            builder.AddRecord(record, vOffset);
        }
    }

    // otherwise just merge BAM
    else {
        BamWriter writer(outputFilename, mergedHeader);
        BamRecord record;
        while (collator->GetNext(record))
            writer.Write(record);
    }
}

} // namespace common
} // namespace BAM
} // namespace PacBio
