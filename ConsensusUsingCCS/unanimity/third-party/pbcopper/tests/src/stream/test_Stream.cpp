
#include "OStreamRedirector.h"
#include <pbcopper/stream/Stream.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <cmath>
using namespace PacBio;
using namespace PacBio::Stream;

Source<char> stringInput = [](Sink<char> s)
{
    const std::string original = { "Test String " };
    for (auto e : original)
        s(static_cast<char>(e));
};

Source<char> newlinedStringInput = [](Sink<char> s)
{
    const std::string original = {"Some\nfile\nwith\nnewlines\n"};
    for (auto e : original)
        s(static_cast<char>(e));
};

Source<std::string> stringListInput = [](Sink<std::string> s)
{
    const std::vector<std::string> input = { "string1", "string2", "foo", "bar" };
    for (auto e : input)
        s(e);
};

Sink<char> consoleOutput = [](char c)
{
    std::cerr << c;
};

Sink<std::string> consoleStringOutput = [](std::string s)
{
    std::cerr << s;
};

Sink<std::string> newlinedStringOutput = [](std::string s)
{
    consoleStringOutput(s);
    std::cerr << std::endl;
};

Transform<char, char> toAllCaps = [](Sink<char> si, char c)
{
    si(toupper(c));
};

Transform<char, std::string> getLines = [](Sink<std::string> si, char c)
{
    static std::string current;
    Sink<char> collectChar = [&](char c)
    {
        current.push_back(c);
    };

    if (c == '\n')
    {
        si(current);
        current.clear();
    }
    else
        collectChar(c);
};

Transform<char, std::string> getWords = [](Sink<std::string> si, char c)
{
    static std::string current;
    Sink<char> collectChar = [&](char c)
    {
        current.push_back(c);
    };

    if (c == ' ')
    {
        si(current);
        current.clear();
    }
    else
        collectChar(c);
};

Transform<std::string, char> wordToChars = [](Sink<char> si, std::string s)
{
    for (auto e : s)
        si(static_cast<char>(e));
    si(' ');
};

// connect source -> sink
// connect source -> transform -> sink // same type
// connect source -> transform -> sink // different type
// connect source -> transform -> transform -> sink // same types
// connect source -> transform -> transform -> sink // differing types
// connect source -> transform -> transform -> sink // use named sub-streams

TEST(Stream_Stream, input_to_console)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    stringInput >> consoleOutput;

    EXPECT_EQ("Test String ", s.str());
}

TEST(Stream_Stream, char_input_to_all_caps)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    stringInput >> toAllCaps >> consoleOutput;

    EXPECT_EQ("TEST STRING ", s.str());
}

TEST(Stream_Stream, stringlist_to_chars)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    stringListInput >> wordToChars >> consoleOutput;

    EXPECT_EQ("string1 string2 foo bar ", s.str());
}

TEST(Stream_Stream, char_input_split_into_words_and_printed_to_console)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    stringInput >> getWords >> newlinedStringOutput;

    EXPECT_EQ("Test\nString\n", s.str());
}

TEST(Stream_Stream, newlined_input_printed_to_console)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    newlinedStringInput >> getLines >> newlinedStringOutput;

    EXPECT_EQ("Some\nfile\nwith\nnewlines\n", s.str());
}

TEST(Stream_Stream, composing_transform_chains)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    newlinedStringInput >> getLines >> wordToChars >> consoleOutput;

    EXPECT_EQ("Some file with newlines ", s.str());
}

TEST(Stream_Stream, composing_named_transform_chains)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    Source<std::string> inputLines = newlinedStringInput >> getLines;
    Sink<std::string> wordOutput = wordToChars >> consoleOutput;
    inputLines >> wordOutput;

    EXPECT_EQ("Some file with newlines ", s.str());
}

Source<int> intList = [](Sink<int> out)
{
    const std::vector<int> ints = { 1, 2, 3, 4, 5 };
    for (auto i : ints)
        out(i);
};


Sink<int> intConsoleOutput = [](int i)
{
    std::cerr << i;
};

Sink<int> spacedIntConsoleOutput = [](int i)
{
    intConsoleOutput(i);
    std::cerr << " ";
};

Sink<double> doubleConsoleOutput = [](double d)
{
    std::cerr << d;
};

Sink<double> spacedDoubleConsoleOutput = [](double d)
{
    doubleConsoleOutput(d);
    std::cerr << " ";
};

Transform<int, int> tripled = [](Sink<int> out, int i)
{
    out(i*3);
};

Transform<int, double> squareRoot = [](Sink<double> out, int i)
{
    out( sqrt(i) );
};

Transform<double, std::string> addCookies = [](Sink<std::string> out, double d)
{
    const std::string result = std::to_string(d) + "cookies";
    out(result);
};

struct MyClass
{
    static void Increment(const Sink<int>& out, const int& i)
    {
        out(i + 1);
    }
};

TEST(Stream_Stream, arithmetic_stream_to_console)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    intList >> spacedIntConsoleOutput;

    EXPECT_EQ("1 2 3 4 5 ", s.str());
}

TEST(Stream_Stream, arithmetic_transformed_stream_to_console)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    intList >> tripled
            >> spacedIntConsoleOutput;

    EXPECT_EQ("3 6 9 12 15 ", s.str());
}

TEST(Stream_Stream, arithmetic_transformed_to_different_type_stream_to_console)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    intList >> tripled
            >> squareRoot
            >> spacedDoubleConsoleOutput;

    EXPECT_EQ("1.73205 2.44949 3 3.4641 3.87298 ", s.str());
}

TEST(Stream_Stream, append_text_to_arithmetic_stream)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    intList >> tripled
            >> squareRoot
            >> addCookies
            >> newlinedStringOutput;

    EXPECT_EQ("1.732051cookies\n"
              "2.449490cookies\n"
              "3.000000cookies\n"
              "3.464102cookies\n"
              "3.872983cookies\n", s.str());
}

TEST(Stream_Stream, append_text_to_arithmetic_stream_then_modify)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    intList >> tripled                 // x * 3
            >> squareRoot              // sqrt(x)
            >> addCookies              // string result = to_string(x) + "cookies"
            >> wordToChars             // converts strings to chars
            >> toAllCaps               // toupper(c)
            >> getWords                // chars back to strings, split on delim = ' '
            >> newlinedStringOutput;   // cerr << s << endl

    EXPECT_EQ("1.732051COOKIES\n"
              "2.449490COOKIES\n"
              "3.000000COOKIES\n"
              "3.464102COOKIES\n"
              "3.872983COOKIES\n", s.str());
}

TEST(Stream_Stream, stream_works_with_static_public_member_function)
{
    std::stringstream s;
    tests::CerrRedirect redirect(s.rdbuf());

    intList >> Transform<int,int>(&MyClass::Increment)
            >> tripled                 // x * 3
            >> squareRoot              // sqrt(x)
            >> addCookies              // string result = to_string(x) + "cookies"
            >> wordToChars             // converts strings to chars
            >> toAllCaps               // toupper(c)
            >> getWords                // chars back to strings, split on delim = ' '
            >> newlinedStringOutput;   // cerr << s << endl

    EXPECT_EQ("2.449490COOKIES\n"
              "3.000000COOKIES\n"
              "3.464102COOKIES\n"
              "3.872983COOKIES\n"
              "4.242641COOKIES\n", s.str());
}

