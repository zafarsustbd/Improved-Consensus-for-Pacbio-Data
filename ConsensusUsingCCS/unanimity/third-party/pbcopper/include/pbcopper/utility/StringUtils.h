#ifndef PBCOPPER_UTILITY_STRINGUTILS_H
#define PBCOPPER_UTILITY_STRINGUTILS_H

#include <string>
#include <vector>

namespace PacBio {
namespace Utility {

std::string Join(const std::vector<std::string>& input,
                 const std::string& delim);

std::string Join(const std::vector<std::string>& input,
                 const char delim);

std::vector<std::string> Split(const std::string& line,
                               const char delim = '\t');

} // namespace Utility
} // namespace PacBio

#include "internal/StringUtils-inl.h"

#endif // PBCOPPER_UTILITY_STRINGUTILS_H
