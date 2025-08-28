//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <vector>

namespace circular_buffer {

template <typename T, std::size_t N>
class CircularBuffer {
    static_assert(N > 0, "N must be positive");

    std::array<T, N> buffer_;
    std::size_t size_{0};
    std::size_t capacity_{N};
    std::size_t head_{0};
    std::size_t tail_{0};

public:

    CircularBuffer() {
        printf("Circular buffer created\n");
    }

    void push(const T& value) {
        buffer_[head_] = value;
        if (size_ == capacity_) {
            tail_ = (head_ + 1) % capacity_;
        }
        head_ = (head_ + 1) % capacity_;
        size_ = std::min(size_ + 1, capacity_);
    }

    T front() {
        if (size_ == 0) {
            throw std::runtime_error("Empty buffer");
        }
        return buffer_[tail_];
    }

    T pop() {
        if (size_ == 0) {
            throw std::runtime_error("Empty buffer");
        }
        T item = buffer_[tail_];
        tail_ = (tail_ + 1) % capacity_;
        size_--;
        return item;
    }

    void clear() {
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }

    bool empty() {
        return size_ == 0;
    }

    bool full() {
        return size_ == capacity_;
    }
};
}
#endif //CIRCULARBUFFER_H
