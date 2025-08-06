//
// Created by Dhruv Sirohi on 8/4/25.
//
#include <gtest/gtest.h>
#include <circular_buffer/CircularBuffer.h>

using namespace circular_buffer;

TEST(CircularBufferTest, PushAndPopBasic) {
    CircularBuffer<int> buffer(3);
    buffer.push(1);
    buffer.push(2);
    buffer.push(3);

    EXPECT_EQ(buffer.pop(), 1);
    EXPECT_EQ(buffer.pop(), 2);
    EXPECT_EQ(buffer.pop(), 3);
    EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, OverwriteWhenFull) {
    CircularBuffer<int> buffer(2);
    buffer.push(10);
    buffer.push(20);
    buffer.push(30);  // overwrites 10

    EXPECT_EQ(buffer.pop(), 20);
    EXPECT_EQ(buffer.pop(), 30);
    EXPECT_TRUE(buffer.empty());
}

TEST(CircularBufferTest, FrontPeeksWithoutPop) {
    CircularBuffer<std::string> buffer(2);
    buffer.push("a");
    buffer.push("b");

    EXPECT_EQ(buffer.front(), "a");
    buffer.pop();
    EXPECT_EQ(buffer.front(), "b");
}

TEST(CircularBufferTest, ClearWorks) {
    CircularBuffer<int> buffer(2);
    buffer.push(1);
    buffer.push(2);
    buffer.clear();
    EXPECT_TRUE(buffer.empty());
    EXPECT_THROW(buffer.pop(), std::runtime_error);
}

TEST(CircularBufferTest, ExceptionsOnEmpty) {
    CircularBuffer<int> buffer(1);
    EXPECT_THROW(buffer.pop(), std::runtime_error);
    EXPECT_THROW(buffer.front(), std::runtime_error);
}
