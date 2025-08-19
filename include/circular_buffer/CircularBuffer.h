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

    std::array<T, N> data;
    int head_ = 0;
    int tail_ = 0;
    int size_ = 0;
    int capacity_ = N;

public:

    CircularBuffer() {
        printf("Circular buffer created\n");
    }

    void push(const T& t) {
        data[head_] = t;
        if (size_ == capacity_) {
            tail_ = (head_ + 1) % capacity_;
        }
        head_ = (head_ + 1) % capacity_;
        size_ = size_ + 1 > capacity_ ? capacity_ : size_ + 1;
    }

    T pop() {
        if (size_ == 0) {
            throw std::runtime_error("Empty circular buffer");
        }
        T item = data[tail_];
        tail_ = (tail_ + 1) % capacity_;
        size_--;
        return item;
    }

    bool empty() {
        return size_ == 0;
    }

    T front() const {
        if (size_ == 0) {
            throw std::runtime_error("Empty circular buffer");
        }
        return data[tail_];
    }

    void clear() {
        head_ = 0;
        tail_ = 0;
        size_ = 0;
    }

};
}
#endif //CIRCULARBUFFER_H
