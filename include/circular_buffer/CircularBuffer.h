//
// Created by Dhruv Sirohi on 8/4/25.
//

#ifndef CIRCULARBUFFER_H
#define CIRCULARBUFFER_H

#include <vector>

namespace circular_buffer {

    template<typename T, std::size_t N>
    class CircularBuffer {

        static_assert(N > 0, "N must be positive");

        int head_ = 0;
        int tail_ = 0;
        int capacity_ = N;
        int size_ = 0;
        std::array<T, N> data_;

    public:

        CircularBuffer() {}

        void push(const T value) {
            data_[head_] = value;
            if (size_ == capacity_) {
                tail_ = (head_ + 1) % capacity_;
            }
            head_ = (head_ + 1) % capacity_;
            size_ = size_ + 1 > capacity_ ? capacity_ : size_ + 1;
        }

        std::optional<T> pop() {
            if (size_ == 0) {
                throw std::runtime_error("Circular buffer is empty");
            }
            T item = data_[tail_];
            tail_ = (tail_ + 1) % capacity_;
            size_--;
            return item;
        }

        bool empty() const {
            return size_ == 0;
        }

        T front() {
            if (size_ == 0) throw std::runtime_error("Circular buffer is empty");
            return data_[tail_];
        }

        void clear() {
            head_ = 0;
            tail_ = 0;
            size_ = 0;
        }
    };
}
#endif //CIRCULARBUFFER_H
