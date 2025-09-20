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
    std::array<T, N> buffer;

    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t size = 0;
    std::size_t capacity = N;
    std::mutex mutex;

public:

    CircularBuffer() {
        // buffer.resize(N);
        printf("Circular buffer created...\n");
    }

    void push(T value) {
        std::lock_guard<std::mutex> lock(mutex);
        buffer[head] = value;
        if (size == capacity) {
            tail = (head + 1) % capacity;
        }
        head = (head + 1) % capacity;
        size = size + 1 > capacity ? capacity : size + 1;
    }

    T pop() {
        std::lock_guard<std::mutex> lock(mutex);
        if (size == 0) {
            throw std::runtime_error("Empty buffer");
        }
        T val = buffer[tail];
        tail = (tail + 1) % capacity;
        size--;
        return val;
    }

    T front() {
        std::lock_guard<std::mutex> lock(mutex);
        if (size == 0) {
            throw std::runtime_error("Empty buffer");
        }
        return buffer[tail];
    }

    bool empty() {
        std::lock_guard<std::mutex> lock(mutex);
        return size == 0;
    }

    void clear() {
        size = 0;
        head = 0;
        tail = 0;
    }
};
}
#endif //CIRCULARBUFFER_H
