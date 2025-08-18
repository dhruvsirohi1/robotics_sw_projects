//
// Created by Dhruv Sirohi on 8/13/25.
//
#include "CircularBuffer.h"

#include <gtest/gtest.h>
#include <thread>
#include <future>
#include <chrono>
#include <atomic>
#include <vector>
#include <optional>

using namespace std::chrono_literals;
using circular_buffer::CircularBuffer;

// Helper: wait on a future with timeout and return optional<T>
template <class T>
std::optional<T> get_with_timeout(std::future<T>& fut, std::chrono::milliseconds dur) {
    if (fut.wait_for(dur) == std::future_status::ready) {
        return fut.get();
    }
    return std::nullopt;
}

// ---------- Tests ----------

TEST(CircularBufferBasics, PushPopFIFO_SingleThread) {
    CircularBuffer<int, 3> q;

    q.push(1);
    q.push(2);
    q.push(3);

    auto a = q.pop(); ASSERT_TRUE(a.has_value()); EXPECT_EQ(*a, 1);
    auto b = q.pop(); ASSERT_TRUE(b.has_value()); EXPECT_EQ(*b, 2);
    auto c = q.pop(); ASSERT_TRUE(c.has_value()); EXPECT_EQ(*c, 3);

    EXPECT_TRUE(q.empty());
}

TEST(CircularBufferBasics, OverwriteOnFullBehavior) {
    // Capacity 3; fourth push overwrites oldest
    CircularBuffer<int, 3> q;

    q.push(10);
    q.push(20);
    q.push(30);
    q.push(40); // overwrites 10

    // Buffer should hold 20,30,40 in that order
    auto a = q.pop(); ASSERT_TRUE(a.has_value()); EXPECT_EQ(*a, 20);
    auto b = q.pop(); ASSERT_TRUE(b.has_value()); EXPECT_EQ(*b, 30);
    auto c = q.pop(); ASSERT_TRUE(c.has_value()); EXPECT_EQ(*c, 40);
    EXPECT_TRUE(q.empty());
}

TEST(CircularBufferBlocking, PopBlocksUntilPush) {
    CircularBuffer<int, 2> q;

    // Start a popping thread that should block until we push
    auto fut = std::async(std::launch::async, [&](){
        return q.pop();  // should block until a push arrives
    });

    // Ensure it’s blocking (should not be ready yet)
    EXPECT_FALSE(get_with_timeout(fut, 50ms).has_value());

    // Now push and expect the future to resolve quickly
    q.push(42);

    auto res = get_with_timeout(fut, 500ms);
    ASSERT_TRUE(res.has_value());
    EXPECT_EQ(*res, 42);
}

TEST(CircularBufferShutdown, CloseUnblocksPopAndReturnsNulloptWhenEmpty) {
    CircularBuffer<int, 2> q;

    // Launch a thread that waits to pop
    auto fut = std::async(std::launch::async, [&](){
        return q.pop();  // should return nullopt after close() if empty
    });

    // Let it get to wait()
    std::this_thread::sleep_for(50ms);

    q.close();

    auto res = get_with_timeout(fut, 500ms);
    ASSERT_TRUE(res.has_value()); // future finished
    EXPECT_FALSE(res->has_value()); // pop() returned std::nullopt
}

TEST(CircularBufferAPI, FrontAndClear) {
    CircularBuffer<int, 4> q;

    q.push(1);
    q.push(2);
    EXPECT_FALSE(q.empty());
    EXPECT_EQ(q.front(), 1);

    q.clear();
    EXPECT_TRUE(q.empty());

    // After clear, push works again
    q.push(7);
    EXPECT_EQ(q.front(), 7);
    auto v = q.pop(); ASSERT_TRUE(v.has_value()); EXPECT_EQ(*v, 7);
}

TEST(CircularBufferConcurrency, BasicMPMCDoesNotDeadlock) {
    // NOTE: Your buffer overwrites when full (non-blocking producers),
    // so we don't assert on exact counts, only that we make forward progress and terminate.
    CircularBuffer<int, 128> q;

    constexpr int producers = 4;
    constexpr int consumers = 4;
    constexpr int per_prod  = 1000;

    std::atomic<int> consumed{0};
    std::atomic<bool> done{false};

    // consumers
    std::vector<std::thread> threads;
    for (int i = 0; i < consumers; ++i) {
        threads.emplace_back([&]{
            while (true) {
                auto item = q.pop();
                if (!item.has_value()) break; // closed and drained
                ++consumed;
            }
        });
    }

    // producers
    std::vector<std::thread> prod_threads;
    for (int p = 0; p < producers; ++p) {
        prod_threads.emplace_back([&, p]{
            for (int i = 0; i < per_prod; ++i) {
                q.push(p * per_prod + i);
                // tiny pause increases interleaving probability
                if ((i & 63) == 0) std::this_thread::sleep_for(1ms);
            }
        });
    }

    for (auto& t : prod_threads) t.join();

    q.close(); // wake consumers so they can exit once drained

    for (auto& t : threads) t.join();

    // We produced this many in total:
    int produced = producers * per_prod;

    // With overwrite-on-full, we can lose items; ensure we consumed a plausible amount (>0 && <= produced)
    EXPECT_GT(consumed.load(), 0);
    EXPECT_LE(consumed.load(), produced);
}
