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

// Author: Lance Hepler

#include <boost/lexical_cast.hpp>
#include <boost/xpressive/xpressive.hpp>

#include <pacbio/data/ChemistryTriple.h>
#include <pacbio/exception/CCSExceptions.h>

using namespace std;

namespace PacBio {
namespace Data {

using namespace PacBio::Exception;

ChemistryTriple::ChemistryTriple(const std::string& bindingKit, const std::string& sequencingKit,
                                 const std::string& changeListID)
{
    if (!SetValues(bindingKit, sequencingKit, changeListID)) {
        throw BadChemistryTriple("unparsable ChemistryTriple(" + bindingKit + ", " + sequencingKit +
                                 ", " + changeListID + ")");
    }
}

bool ChemistryTriple::SetValues(const std::string& bindingKit, const std::string& sequencingKit,
                                const std::string& changeListID)
{
    using namespace boost::xpressive;

    using boost::lexical_cast;

    try {
        BindingKit = lexical_cast<unsigned>(bindingKit);
        SequencingKit = lexical_cast<unsigned>(sequencingKit);

        smatch what;
        // sregex::compile("^(\\d+)\\.(\\d+)");
        sregex re = bos >> (s1 = +_d) >> '.' >> (s2 = +_d);

        if (regex_search(changeListID.begin(), changeListID.end(), what, re)) {
            MajorVersion = lexical_cast<unsigned>(what[1]);
            MinorVersion = lexical_cast<unsigned>(what[2]);
            return true;
        }
    } catch (const boost::bad_lexical_cast& e) {
    }

    return false;
}

}  // namespace Data
}  // namespace PacBio
