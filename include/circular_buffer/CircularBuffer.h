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
    std::size_t head_;
    std::size_t tail_;
    std::size_t size_;
    std::size_t capacity_;
    mutable std::mutex mutex_;
    mutable std::condition_variable cv_;
    bool closed_ = false;

public:
    CircularBuffer() : head_(0), tail_(0), size_(0), capacity_(N) {
        static_assert(N > 0, "Circular buffer size must be positive");
        data_.resize(N);
    }

    void push(const T& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        data_[head_] = value;
        if (head_ == tail_ && size_ == capacity_) {
            tail_ = (tail_ + 1) % capacity_;
        } else {
            ++size_;
        }
        head_ = (head_ + 1) % capacity_;
        cv_.notify_one();
    }

    std::optional<T> pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock, [this]{ return size_ > 0 || closed_; });
        // if (size_ == 0) throw std::runtime_error("Circular buffer is empty");
        if (closed_) return std::nullopt;
        T item = std::move(data_[tail_]);
        tail_ = (tail_ + 1) % capacity_;
        size_--;
        return item;
    }

    T front() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (size_ == 0) throw std::runtime_error("Circular buffer is empty");
        T item = data_[tail_];
        return item;
    }

    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return size_ == 0;
    }

    bool isFull() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return size_ == capacity_;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }

    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
        cv_.notify_all();
    }
};
}
#endif //CIRCULARBUFFER_H
