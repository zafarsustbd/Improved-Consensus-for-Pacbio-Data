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

#include "../common/OptionParser.h"
#include "../common/BamFileMerger.h"
#include "PbMergeVersion.h"
#include <cassert>
#include <iostream>
using namespace std;

namespace pbmerge {

class Settings
{
public:
    static Settings FromCommandLine(optparse::OptionParser& parser,
                                    int argc, char* argv[])
    {
        pbmerge::Settings settings;
        const optparse::Values options = parser.parse_args(argc, argv);

        // input
        const vector<string> positionalArgs = parser.args();
        if (positionalArgs.empty())
            settings.errors_.push_back("at least input one file must be specified");
        else
            settings.inputFilenames_ = positionalArgs;

        // output
        if (options.is_set("output"))
            settings.outputFilename_ = options["output"];
        else
            settings.outputFilename_ = "-"; // stdout

        // PBI?
        if (settings.outputFilename_ == "-")
            settings.createPbi_ = false; // always skip PBI if writing to stdout
        else {
            if (options.is_set("no_pbi"))
                settings.createPbi_ = !options.get("no_pbi"); // user-disabled
            else
                settings.createPbi_ = true; // not specified, go ahead and generate by default
        }

        return settings;
    }

public:
    std::vector<std::string> inputFilenames_;
    std::string outputFilename_;
    bool createPbi_;
    std::vector<std::string> errors_;

private:
    Settings(void) { }
};

} // namespace pbmerge

int main(int argc, char* argv[])
{
    // setup help & options
    optparse::OptionParser parser;
    parser.description("pbmerge merges PacBio BAM files. If the input is DataSetXML, "
                       "any filters will be applied. If no output filename is specified, "
                       "new BAM will be written to stdout."
                       );
    parser.prog("pbmerge");
    parser.usage("pbmerge [options] [-o <out.bam>] <INPUT>");
    parser.version(pbmerge::Version);
    parser.add_version_option(true);
    parser.add_help_option(true);

    auto ioGroup = optparse::OptionGroup(parser, "Input/Output");
    ioGroup.add_option("-o")
           .dest("output")
           .metavar("output")
           .help("Output BAM filename. ");
    ioGroup.add_option("--no-pbi")
            .dest("no_pbi")
            .action("store_true")
            .help("Set this option to skip PBI index file creation. PBI creation is "
                  "automatically skipped if no output filename is provided."
                  );
    ioGroup.add_option("")
           .dest("input")
           .metavar("INPUT")
           .help("Input may be one of:\n"
                 "    DataSetXML, list of BAM files, or FOFN\n\n"
                 "    fofn: pbmerge -o merged.bam bams.fofn\n\n"
                 "    bams: pbmerge -o merged.bam 1.bam 2.bam 3.bam\n\n"
                 "    xml:  pbmerge -o merged.bam foo.subreadset.xml\n\n"
                 );
    parser.add_option_group(ioGroup);

    // parse command line for settings
    const pbmerge::Settings settings = pbmerge::Settings::FromCommandLine(parser, argc, argv);
    if (!settings.errors_.empty()) {
        cerr << endl;
        for (const auto e : settings.errors_)
            cerr << "ERROR: " << e << endl;
        cerr << endl;
        parser.print_help();
        return EXIT_FAILURE;
    }

    // run tool
    try {
        // setup our @PG entry to add to header
        PacBio::BAM::ProgramInfo mergeProgram;
        mergeProgram.Id(string("pbmerge-")+pbmerge::Version)
                    .Name("pbmerge")
                    .Version(pbmerge::Version);

        PacBio::BAM::DataSet dataset;
        if (settings.inputFilenames_.size() == 1)
            dataset = PacBio::BAM::DataSet(settings.inputFilenames_.front());
        else
            dataset = PacBio::BAM::DataSet(settings.inputFilenames_);


        PacBio::BAM::common::BamFileMerger::Merge(dataset,
                                                  settings.outputFilename_,
                                                  mergeProgram,
                                                  settings.createPbi_);


//        PacBio::BAM::common::BamFileMerger merger(dataset,
//                                                  settings.outputFilename_,
//                                                  mergeProgram,
//                                                  settings.createPbi_);
//        merger.Merge();

        return EXIT_SUCCESS;
    }
    catch (std::exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}
