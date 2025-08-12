//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <vector>

namespace circular_buffer {

template <typename T>
class CircularBuffer {
    std::vector<T> data_;
    int size_;
    int capacity_;
    int head_;
    int tail_;
public:

    CircularBuffer(int capacity) :
    size_(0), capacity_(capacity){
        head_ = tail_ = 0;
        data_.reserve(capacity);
    }

    bool push(const T& item) {
        data_[head_] = item;
        size_ = std::min(size_ + 1, capacity_);
        if (head_ == tail_ && size_ == capacity_) {
            tail_ = (tail_ + 1) % capacity_;
        }
        head_ = (head_ + 1) % capacity_;
        return true;
    }

    T front() {
        if (empty()) throw std::runtime_error("Buffer is empty");
        T front = data_[tail_];
        return front;
    }

    T pop() {
        if (empty()) throw std::runtime_error("Buffer is empty");
        T item = data_[tail_];
        tail_ = (tail_ + 1) % capacity_;
        size_ = std::min(size_ - 1, capacity_);
        return item;
    }

    bool isFull() const {
        return head_ == tail_;
    }

    bool empty() const {
        return size_ == 0;
    }

    void clear() {
        head_ = tail_ = 0;
        size_ = 0;
    }
};
}
#endif //CIRCULARBUFFER_H
