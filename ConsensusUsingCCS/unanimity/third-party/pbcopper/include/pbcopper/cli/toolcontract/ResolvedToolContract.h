#ifndef PBCOPPER_CLI_TOOLCONTRACT_RESOLVEDTOOLCONTRACT_H
#define PBCOPPER_CLI_TOOLCONTRACT_RESOLVEDTOOLCONTRACT_H

#include "pbcopper/cli/Results.h"
#include <iosfwd>

namespace PacBio {
namespace CLI {
namespace ToolContract {

namespace internal { class RtcPrivate; }

class ResolvedToolContract
{
public:
    ResolvedToolContract(const Interface& interface);
    ~ResolvedToolContract(void);

public:
    Results Parse(std::istream& in);

private:
    std::unique_ptr<internal::RtcPrivate> d_;
};

} // namespace ToolContract
} // namespace CLI
} // namespace PacBio

#endif // PBCOPPER_CLI_TOOLCONTRACT_RESOLVEDTOOLCONTRACT_H

