//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <vector>

namespace circular_buffer {
template <typename T>
class CircularBuffer {
    int capacity_;
    int size_;
    std::vector<T> buffer_;
    int head_;
    int tail_;
    bool is_full_;

public:

    explicit CircularBuffer(int capacity):
    capacity_(capacity),
    size_(0),
    buffer_(capacity),
    head_(0),
    tail_(0),
    is_full_(false) {
    };

    void push(T element) {
        buffer_[head_] = element;
        head_ = (head_ + 1) % capacity_;
        if (is_full_) {
            tail_ = (tail_ + 1) % capacity_;
        }
        size_ = std::min(size_ + 1, capacity_);
        is_full_ = tail_ == head_;
    }

    T pop() {
        if (size_ == 0) {
            throw std::runtime_error("CircularBuffer: pop() from empty buffer");
        }
        T element = buffer_[tail_];
        tail_ = (tail_ + 1) % capacity_;
        size_--;
        is_full_ = false;
        return element;
    }

    const T& front() const {
        if (size_ == 0) {
            throw std::runtime_error("CircularBuffer: front() from empty buffer");
        }
        return buffer_[tail_];
    }

    size_t size() const {
        return size_;
    }

    void clear() {
        head_ = tail_;
        is_full_ = false;
        size_ = 0;
    }

    bool empty() const {
        return size_ == 0;
    }
};
}
#endif //CIRCULARBUFFER_H
