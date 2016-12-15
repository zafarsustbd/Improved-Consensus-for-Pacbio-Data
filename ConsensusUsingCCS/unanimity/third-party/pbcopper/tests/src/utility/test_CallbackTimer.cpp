
#include <pbcopper/utility/CallbackTimer.h>
#include <gtest/gtest.h>
#include <thread>

using namespace PacBio;
using namespace PacBio::Utility;

namespace tests {

class CallbackCounter
{
public:
    CallbackCounter(void) : hits_(0) { }
    uint32_t Count(void) const { return hits_; }
    void Ping(void) { ++hits_; }

private:
    uint32_t hits_;
};

CallbackCounter freeFunctionCounter;

void tst_callback(void)
{ freeFunctionCounter.Ping(); }

auto ping = [](tests::CallbackCounter& counter)
{
    counter.Ping();
};

} // namespace tests

TEST(Utility_CallbackTimer, scheduled_callbacks_fired_as_expected)
{
    //
    // NOTE: This test case does allow for some wiggle in execution
    //       timing, due to inherent fuzziness in timing such precision without
    //       a lot of extra low-level work.
    //

    CallbackTimer t;
    tests::CallbackCounter counterA;
    tests::CallbackCounter counterB;
    tests::CallbackCounter counterC;
    tests::CallbackCounter counterSS;

    // Timer fires once on lambda, 2ms from now
    const auto a = t.Schedule(2, 0, [&](){ tests::ping(counterA); });
    EXPECT_TRUE(t.IsActive(a));

    // Timer fires every 1ms on lambda, starting 2ms from now
    const auto b = t.Schedule(2, 1, [&]() { tests::ping(counterB); });
    EXPECT_TRUE(t.IsActive(b));

    // Timer fires every 1ms on lambda, starting now
    const auto c = t.Schedule(0, 1, [&]() { tests::ping(counterC); });
    EXPECT_TRUE(t.IsActive(c));

    // Timer firest once on free function, 2ms from now
    auto ff = t.Schedule(2, 0, &tests::tst_callback);
    EXPECT_TRUE(t.IsActive(ff));

    // explicit "single shot" lambda, 2ms from now
    CallbackTimer::SingleShot(2, [&]() { tests::ping(counterSS); });

    // sleep a bit to let periodic timers fire
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_EQ(1, counterA.Count());     // effectively single-shot
    EXPECT_LE(7, counterB.Count());     // >= 7 triggers since scheduling
    EXPECT_LE(9, counterC.Count());     // >= 9 triggers since schedling
    EXPECT_EQ(1, counterSS.Count());    // explicit single-shot
    EXPECT_EQ(1, tests::freeFunctionCounter.Count()); // effectively single-shot

    // single-shot callbacks only called once
    EXPECT_EQ(1, counterA.Count());
    EXPECT_EQ(1, counterSS.Count());
    EXPECT_EQ(1, tests::freeFunctionCounter.Count());
}

TEST(Utility_CallbackTimer, canceled_callbacks_not_called)
{
    CallbackTimer t;
    tests::CallbackCounter counterC;
    const auto c = t.Schedule(0, 1, [&]() { tests::ping(counterC); });
    EXPECT_TRUE(t.IsActive(c));
    t.Cancel(c);
    EXPECT_FALSE(t.IsActive(c));

    // sleep a bit to give callback on counterC a chance to fire, it would have
    // fired ~5x if cancel failed... give an extra firing 'wiggle room' for
    // test harness
    const auto preSleepCallbackHitCount = counterC.Count();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    EXPECT_LE(preSleepCallbackHitCount, counterC.Count());
    EXPECT_GE(preSleepCallbackHitCount+1, counterC.Count());
}
