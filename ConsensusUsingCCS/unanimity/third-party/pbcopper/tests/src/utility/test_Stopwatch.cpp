

#include <pbcopper/utility/Stopwatch.h>
#include <gtest/gtest.h>
#include <thread>

using namespace PacBio;
using namespace PacBio::Utility;

//
// using float compares and 'fuzzy' compares to allow for inherent wiggle room
// in system execution
//

TEST(Utility_Stopwatch, determines_elapsed_time_in_milliseconds)
{
    Stopwatch s;
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    auto elapsed = s.ElapsedMilliseconds();
    ASSERT_FLOAT_EQ(3, elapsed);
}

TEST(Utility_Stopwatch, determines_elapsed_time_in_seconds)
{
    // seconds
    Stopwatch s;
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    auto elapsed = s.ElapsedSeconds();
    ASSERT_FLOAT_EQ(3/1000, elapsed);
}

TEST(Utility_Stopwatch, determines_elapsed_time_in_user_precision)
{
    //
    // NOTE: from http://en.cppreference.com/w/cpp/thread/sleep_for :
    //
    //   "Blocks the execution of the current thread for at least the specified
    //   sleep_duration. A steady clock is used to measure the duration. This
    //   function may block for longer than sleep_duration due to scheduling or
    //   resource contention delays."
    //
    // This means we know that we'll sleep here for at least 3ms, but can't
    // really know **exactly** know how much longer. To have a reliable unit
    // test, working at the microsecond-level of granularity, we'll have to give
    // up checking for (even fuzzy) equality and check that we're in the right
    // ballpark.
    //
    // Remember that we're _not_ testing std::chrono directly here, only that our
    // Stopwatch provides the requested duration.
    //

    Stopwatch s;
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    auto elapsed = s.Elapsed<std::chrono::microseconds>();
    ASSERT_LE(3000, elapsed);
    ASSERT_LE(elapsed, 30000);
}

TEST(Utility_Stopwatch, determines_elapsed_time_since_reset)
{
    Stopwatch s;
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    s.Reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
    auto elapsed = s.ElapsedMilliseconds();
    ASSERT_FLOAT_EQ(3, elapsed);
}
