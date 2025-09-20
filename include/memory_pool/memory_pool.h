//
// Created by Dhruv Sirohi on 8/5/25.
//

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <vector>

namespace memory_pool {

template <typename T, std::size_t N>
class MemoryPool {

    static_assert(N > 0, "N must be greater than 0");

    static constexpr std::size_t A = alignof(T) > alignof(void*) ? alignof(T) : alignof(void*);
    static constexpr std::size_t S = sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*);

    static constexpr std::size_t BLOCK_SIZE = (S + A - 1) / A * A;

    std::array<std::byte, N * BLOCK_SIZE> data;
    void* free_ptr_;

public:

    MemoryPool() : free_ptr_(nullptr) {
        printf("Memory pool created...\n");

        for (int i = 0; i < N - 1; i++) {
            std::byte* curr = data.data() + i * BLOCK_SIZE;
            std::byte* next = curr + BLOCK_SIZE;
            std::memcpy(curr, &next, sizeof(void*));
        }

        void* nullp = nullptr;
        std::memcpy(data.data() + (N - 1) * sizeof(void*), &nullp, sizeof(void*));

        free_ptr_ = data.data();
    }

    T* allocate() {
        if (free_ptr_ == nullptr) {
            throw std::bad_alloc();
        }
        T* ptr = static_cast<T*>(free_ptr_);
        std::memcpy(&free_ptr_, free_ptr_, sizeof(void*));
        return ptr;
    }

    void deallocate(T* ptr) {
        if (ptr == nullptr) {
            return;
        }
        std::memcpy(ptr, &free_ptr_, sizeof(void*));
        std::memcpy(&free_ptr_, &ptr, sizeof(void*));
    }
};
}
#endif //MEMORY_POOL_H
