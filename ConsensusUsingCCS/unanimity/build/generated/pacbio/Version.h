// Copyright (c) 2016, Pacific Biosciences of California, Inc.
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

// Author: Armin Töpfer

#pragma once

#include <algorithm>
#include <string>

namespace PacBio {
namespace {
constexpr char   UnyVersion[]    = "2.0.5";
constexpr char   UnyGitSha1[]    = "e75faf0-dirty";
constexpr char   UnyChangelog[]  = "# UNANIMITY - CHANGELOG;;## [2.0.4];;### Added; - Add pbcopper's ToolContract, summary is no longer a second output file; - Differentiate between .xml and .bam output type; - Enforce .pbi generation;;## [2.0.3];;### Added; - Switch from cpp-optparse to pbcopper, use pbcopper's CLI parsing;;## [2.0.2];;### Added; - Fix index errors in the Hirschberg aligner; - Added a cleaner interface for AddRead/GetTemplate;;## [2.0.1];;### Added; - Add new ReleaseWithAssert CMAKE_BUILD_TYPE; - Bump version (to cc2 + ccs); - Unify CCS and CC2 versioning under unanimity; - Cleanup python/swig generation; - Cleanup version handling;;## [0.0.1];;### Added; - Unify code base, refactor directory structure; - Add pbccs, ConsensusCore2, pbsparse, and pbchimera; - Code coverage report; - Initial framework including pbbam, htslib, pbcopper";
constexpr size_t UnyVersionMajor = 2;
constexpr size_t UnyVersionMinor = 0;
constexpr size_t UnyVersionPatch = 5;
}

std::string UnanimityVersion()
{ return UnyVersion; }

std::string UnanimityGitSha1()
{ return UnyGitSha1; }

std::string UnanimityChangelog()
{
    std::string changelog(UnyChangelog);
    std::replace(changelog.begin(), changelog.end(), ';', '\n');
    return changelog;
}

} // ::PacBio
