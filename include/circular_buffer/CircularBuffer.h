//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <vector>

namespace circular_buffer {

template<typename T, std::size_t N>
class CircularBuffer {
    std::vector<T> data_;
    int head_;
    int tail_;
    int size_;
    int capacity_;

public:
    CircularBuffer() : head_(0), tail_(0), size_(0), capacity_(N) {
        static_assert(N > 0, "Circular buffer size must be positive");
        data_.reserve(N);
    }

    void push(const T& value) {
        data_[head_] = value;
        size_ = std::min(size_ + 1, capacity_);
        if (head_ == tail_ && isFull()) {
            tail_ = (tail_ + 1) % capacity_;
        }
        head_ = (head_ + 1) % capacity_;
    }

    T pop() {
        if (empty()) throw std::runtime_error("Circular buffer is empty");
        T item = data_[tail_];
        tail_ = (tail_ + 1) % capacity_;
        size_--;
        return item;
    }

    T front() {
        if (empty()) throw std::runtime_error("Circular buffer is empty");
        T item = data_[tail_];
        return item;
    }

    bool empty() const {
        return size_ == 0;
    }

    bool isFull() const {
        return size_ == capacity_;
    }

    void clear() {
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }
};
}
#endif //CIRCULARBUFFER_H
