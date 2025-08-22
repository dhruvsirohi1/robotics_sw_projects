//
// Created by Dhruv Sirohi on 8/21/25.
//

#ifndef SPSCINTBUFFER_H
#define SPSCINTBUFFER_H
#include <array>
#include <cstddef>

namespace spsc_int_buffer {

class SpScIntBuffer {

    static constexpr std::size_t N = 1024;
    static constexpr std::size_t mask_ = N - 1;
    std::array<int, N> data_{};
    std::atomic<std::size_t> head_{0};
    std::atomic<std::size_t> tail_{0};

public:

    SpScIntBuffer() {
        printf("SpScIntBuffer::SpScIntBuffer()\n");
    }

    bool push(int value) {
        std::size_t tail = tail_.load(std::memory_order_relaxed);
        std::size_t head = head_.load(std::memory_order_acquire);

        if (tail - head == N) { // full
            return false;
        }
        data_[tail & mask_] = value;
        tail_.store(tail + 1, std::memory_order_release);
        return true;
    }

    bool pop(int& out) {
        std::size_t head = head_.load(std::memory_order_relaxed);
        std::size_t tail = tail_.load(std::memory_order_acquire);
        if (head == tail) { // empty
            return false;
        }
        out = data_[head & mask_];
        head_.store(head + 1, std::memory_order_release);
        return true;
    }

    bool empty() const {
        std::size_t head = head_.load(std::memory_order_acquire);
        std::size_t tail = tail_.load(std::memory_order_acquire);
        return head == tail;
    }

    bool full() const {
        std::size_t head = head_.load(std::memory_order_acquire);
        std::size_t tail = tail_.load(std::memory_order_acquire);
        return (tail - head) == N;
    }
};
}

#endif //SPSCINTBUFFER_H
