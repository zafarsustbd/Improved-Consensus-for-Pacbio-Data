#include "pbcopper/cli/HelpPrinter.h"
#include "pbcopper/cli/Interface.h"
#include "pbcopper/json/JSON.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
using namespace PacBio;
using namespace PacBio::CLI;
using namespace PacBio::JSON;
using namespace std;

namespace PacBio {
namespace CLI {
namespace internal {

static
bool shouldIncludeDefaultValue(const Json& defaultValue)
{
    // omit if switch OR null
    if (defaultValue.is_boolean() || defaultValue.is_null())
        return false;

    // omit if empty string default on string-type option
    if (defaultValue.is_string()) {
        const string val = defaultValue;
        if (val.empty())
            return false;
    }

    // otherwise include default value
    return true;
}

static
string formatOption(string optionOutput,
                    size_t longestOptionLength,
                    string description,
                    Json defaultValue)
{
    struct winsize ws;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    const size_t ncol = (ws.ws_col < 2) ? 79 : ws.ws_col - 1;

    stringstream result("");
    auto fullDescription = description;

    // maybe add default value to description
    if (shouldIncludeDefaultValue(defaultValue)) {
        fullDescription += " [";
        fullDescription += defaultValue.dump();
        fullDescription += "]";
    }

    // option names
    optionOutput.resize(longestOptionLength, ' ');
    result << "  " << optionOutput << " ";

    // maybe wrap description
    const auto indent = result.str().length();
    auto lineStart     = size_t{ 0 };
    auto lastBreakable = string::npos;
    const auto max     = size_t{ ncol - indent };
    auto x             = size_t{ 0 };
    const auto len = fullDescription.length();

    for (size_t i = 0; i < len; ++i) {
        ++x;
        const auto c = fullDescription.at(i);
        if (isspace(c))
            lastBreakable = i;

        auto breakAt = string::npos;
        auto nextLineStart = string::npos;

        if (x > max && lastBreakable != string::npos) {
            breakAt = lastBreakable;
            nextLineStart = lastBreakable + 1;
        } else if ( (x > max-1 && lastBreakable == string::npos) || i == len-1) {
            breakAt = i + 1;
            nextLineStart = breakAt;
        } else if (c == '\n') {
            breakAt = i;
            nextLineStart = i+1;
        }

        if (breakAt != string::npos) {
            const auto numChars = breakAt - lineStart;
            if (lineStart > 0)
                result << string(indent, ' ');
            result << fullDescription.substr(lineStart, numChars) << endl;
            x = 0;
            lastBreakable = string::npos;
            lineStart = nextLineStart;
            if (lineStart < len && isspace(fullDescription.at(lineStart)))
                ++lineStart;
            i = lineStart;
        }
    }
    return result.str();
}

static
string formatOptionNames(const Option& option)
{
    stringstream optionOutput("");
    auto first = true;
    for(const auto& name : option.Names()) {
        if (first)
            first = false;
        else
            optionOutput << ",";

        if (name.size() == 1)
            optionOutput << "-";
        else
            optionOutput << "--";

        optionOutput << name;
    }

    if (!option.ValueName().empty())
        optionOutput << " <" << option.ValueName() << ">";

    return optionOutput.str();
}

static
string makeHelpText(const Interface& interface)
{
    stringstream result("");

    const auto options = interface.RegisteredOptions();
    const auto posArgs = interface.RegisteredPositionalArgs();

    // setup usage output
    {
        stringstream usage("");
        usage << interface.ApplicationName();
        if (!options.empty())
            usage << " [options]";
        for (const auto& posArg : posArgs)
            usage << " " << posArg.syntax_;
        result << "Usage: " << usage.str() << endl;
    }

    // description
    const auto appDescription = interface.ApplicationDescription();
    if (!appDescription.empty())
        result << appDescription << endl;

    // empty line
    result << endl;

    // options
    if (!options.empty())
        result << "Options: " << endl;

    auto optionOutputList = vector<string>{ };
    auto longestOptionOutputLength = size_t{ 0 };

    // registered options
    for(const auto& option : options) {
        const auto optionOutputString = formatOptionNames(option);
        optionOutputList.push_back(optionOutputString);
        longestOptionOutputLength = std::max(longestOptionOutputLength,
                                             optionOutputString.size());
    }

    // spacer
    ++longestOptionOutputLength;

    // registered options
    for (size_t i = 0; i < options.size(); ++i) {
        const auto& option = options.at(i);
        if (option.IsHidden())
            continue;
        result << formatOption(optionOutputList.at(i),
                               longestOptionOutputLength,
                               option.Description(),
                               option.DefaultValue());
    }

    // positional args
    if (!posArgs.empty()) {
        if (!options.empty())
            result << endl;
        result << "Arguments: " << endl;
        for (const auto& posArg : posArgs) {
            result << formatOption(posArg.name_,
                                   longestOptionOutputLength,
                                   posArg.description_,
                                   Json());
        }
    }

    return result.str();
}

} // namespace internal
} // namespace CLI
} // namespace PacBio

void HelpPrinter::Print(const Interface& interface, ostream& out)
{
    out << internal::makeHelpText(interface) << endl;
}
