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

#include "JsonFormatter.h"
#include "json.hpp"
#include <pbbam/PbiFile.h>
#include <iostream>
#include <sstream>
using namespace pbindexdump;
using namespace PacBio::BAM;
using namespace std;

namespace pbindexdump {


} // namespace pbindexdump

JsonFormatter::JsonFormatter(const Settings& settings)
    : IFormatter(settings)
    , index_(settings.inputPbiFilename_)
{ }

void JsonFormatter::FormatMetadata(void)
{
    auto version = string{ };
    switch (index_.Version()) {
        case PbiFile::Version_3_0_0 : version = "3.0.0"; break;
        case PbiFile::Version_3_0_1 : version = "3.0.1"; break;
        default:
            throw runtime_error("unsupported PBI version encountered");
    }

    nlohmann::json fileSections;
    fileSections.push_back("BasicData");
    if (index_.HasBarcodeData())   fileSections.push_back("BarcodeData");
    if (index_.HasMappedData())    fileSections.push_back("MappedData");
    if (index_.HasReferenceData()) fileSections.push_back("ReferenceData");

    json_["version"]      = version;
    json_["fileSections"] = fileSections;
    json_["numReads"]     = index_.NumReads();
}

void JsonFormatter::FormatRaw(void)
{
    const PbiRawBasicData& basicData = index_.BasicData();
    json_["basicData"]["rgId"]       = basicData.rgId_;
    json_["basicData"]["qStart"]     = basicData.qStart_;
    json_["basicData"]["qEnd"]       = basicData.qEnd_;
    json_["basicData"]["holeNumber"] = basicData.holeNumber_;
    json_["basicData"]["readQual"]   = basicData.readQual_;
    json_["basicData"]["ctxtFlag"]   = basicData.ctxtFlag_;
    json_["basicData"]["fileOffset"] = basicData.fileOffset_;

    if (index_.HasBarcodeData()) {
        const PbiRawBarcodeData& barcodeData = index_.BarcodeData();
        json_["barcodeData"]["bcForward"] = barcodeData.bcForward_;
        json_["barcodeData"]["bcReverse"] = barcodeData.bcReverse_;
        json_["barcodeData"]["bcQuality"] = barcodeData.bcQual_;
    }

    if (index_.HasMappedData()) {
        const PbiRawMappedData& mappedData = index_.MappedData();

        // casts to force -1 if unmapped
        json_["mappedData"]["tId"]    = mappedData.tId_;
        json_["mappedData"]["tStart"] = mappedData.tStart_;
        json_["mappedData"]["tEnd"]   = mappedData.tEnd_;

        json_["mappedData"]["aStart"]    = mappedData.aStart_;
        json_["mappedData"]["aEnd"]      = mappedData.aEnd_;
        json_["mappedData"]["revStrand"] = mappedData.revStrand_;
        json_["mappedData"]["nM"]        = mappedData.nM_;
        json_["mappedData"]["nMM"]       = mappedData.nMM_;
        json_["mappedData"]["mapQV"]     = mappedData.mapQV_;
    }
}

void JsonFormatter::FormatRecords(void)
{
    nlohmann::json reads;
    const uint32_t numReads = index_.NumReads();
    const bool hasBarcodeData = index_.HasBarcodeData();
    const bool hasMappedData  = index_.HasMappedData();
    for (uint32_t i = 0; i < numReads; ++i) {

        nlohmann::json read;

        // common data
        const PbiRawBasicData& basicData = index_.BasicData();
        read["rgId"]        = basicData.rgId_[i];
        read["qStart"]      = basicData.qStart_[i];
        read["qEnd"]        = basicData.qEnd_[i];
        read["holeNumber"]  = basicData.holeNumber_[i];
        read["readQuality"] = basicData.readQual_[i];
        read["contextFlag"] = basicData.ctxtFlag_[i];
        read["fileOffset"]  = basicData.fileOffset_[i];

        // barcode data, if present
        if (hasBarcodeData) {
            const PbiRawBarcodeData& barcodeData = index_.BarcodeData();
            read["bcForward"] = barcodeData.bcForward_[i];
            read["bcReverse"] = barcodeData.bcReverse_[i];
            read["bcQuality"] = barcodeData.bcQual_[i];
        }

        // mapping data, if present
        if (hasMappedData) {
            const PbiRawMappedData& mappedData = index_.MappedData();

            // casts to force -1 if unmapped
            read["tId"]    = static_cast<int32_t>(mappedData.tId_[i]);
            read["tStart"] = static_cast<int32_t>(mappedData.tStart_[i]);
            read["tEnd"]   = static_cast<int32_t>(mappedData.tEnd_[i]);

            read["aStart"] = mappedData.aStart_[i];
            read["aEnd"]   = mappedData.aEnd_[i];
            read["nM"]     = mappedData.nM_[i];
            read["nMM"]    = mappedData.nMM_[i];
            read["mapQuality"]    = mappedData.mapQV_[i];
            read["reverseStrand"] = mappedData.revStrand_[i];
        }

        reads.push_back(std::move(read));
    }
    json_["reads"] = reads;
}

void JsonFormatter::FormatReferences(void)
{
    if (index_.HasReferenceData()) {
        const PbiRawReferenceData& referenceData = index_.ReferenceData();
        nlohmann::json references;
        for (const PbiReferenceEntry& entry : referenceData.entries_) {
            nlohmann::json element;
            element["tId"]      = static_cast<int32_t>(entry.tId_);
            element["beginRow"] = static_cast<int32_t>(entry.beginRow_);
            element["endRow"]   = static_cast<int32_t>(entry.endRow_);
            references.push_back(std::move(element));
        }
        json_["references"] = references;
    }
}

void JsonFormatter::Print(void)
{
    cout << json_.dump(settings_.jsonIndentLevel_) << endl;
}

void JsonFormatter::Run(void)
{
    FormatMetadata();
    FormatReferences();

    if (settings_.jsonRaw_)
        FormatRaw();
    else
        FormatRecords();

    Print();
}
