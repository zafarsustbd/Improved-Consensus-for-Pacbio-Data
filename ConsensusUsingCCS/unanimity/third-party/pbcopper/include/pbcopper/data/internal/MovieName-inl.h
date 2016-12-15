
#ifndef PBCOPPER_DATA_MOVIENAME_INL_H
#define PBCOPPER_DATA_MOVIENAME_INL_H

#include "pbcopper/data/MovieName.h"
#include "pbcopper/utility/StringUtils.h"
#include <iostream>
#include <stdexcept>
#include <cassert>

namespace PacBio {
namespace Data {

struct MovieName::PartsCache
{
    boost::string_ref instrumentName_;
    boost::string_ref runStartTime_;
};

// NOTE: We're not going to re-calculate cache in copies until actually needed.
//       We can't completely re-use the original cache, since the string_refs
//       point to other string::c_str(). So we won't pay for the calculation
//       until the components are again requested. That should happen less often
//       than moving these guys around.

inline MovieName::MovieName(void)
    : partsCache_(nullptr)
{ }

inline MovieName::MovieName(const MovieName& other)
    : movieName_(other.movieName_)
    , partsCache_(nullptr)
{ }

inline MovieName::MovieName(MovieName&& other) noexcept
    : movieName_(std::move(other.movieName_))
    , partsCache_(std::move(other.partsCache_))
{ }

inline MovieName& MovieName::operator=(const MovieName& other)
{
    movieName_ = other.movieName_;
    partsCache_.reset(nullptr);
    return *this;
}

inline MovieName& MovieName::operator=(MovieName&& other) noexcept
{
    movieName_ = std::move(other.movieName_);
    partsCache_ = std::move(other.partsCache_);
    return *this;
}

inline MovieName::MovieName(const std::string& name)
    : movieName_(name)
    , partsCache_(nullptr)
{ }

inline MovieName::MovieName(std::string&& name)
    : movieName_(std::move(name))
    , partsCache_(nullptr)
{ }

inline MovieName::~MovieName(void) { }

inline boost::string_ref MovieName::InstrumentName(void) const
{
    if (!partsCache_)
        UpdatePartsCache();
    assert(partsCache_);
    return partsCache_->instrumentName_;
}

inline boost::string_ref  MovieName::RunStartTime(void) const
{
    if (!partsCache_)
        UpdatePartsCache();
    assert(partsCache_);
    return partsCache_->runStartTime_;
}

inline std::string MovieName::ToStdString(void)const
{ return movieName_; }

inline bool operator==(const MovieName& lhs, const MovieName& rhs)
{ return lhs.ToStdString() == rhs.ToStdString(); }

inline bool operator!=(const MovieName& lhs, const MovieName& rhs)
{ return !(lhs == rhs); }

inline bool operator<(const MovieName& lhs, const MovieName& rhs)
{ return lhs.ToStdString() < rhs.ToStdString(); }

inline std::ostream& operator<<(std::ostream& os, const MovieName& movieName)
{
    os << movieName.ToStdString();
    return os;
}

inline std::istream& operator>>(std::istream& is, MovieName& movieName)
{
    auto s = std::string{ };
    is >> s;
    movieName = MovieName{ std::move(s) };
    return is;
}

} // namespace Data
} // namespace PacBio

#endif // PBCOPPER_DATA_MOVIENAME_INL_H
