//
// Created by Dhruv Sirohi on 8/5/25.
//

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <vector>

namespace memory_pool {

template<typename T, std::size_t N>
class MemoryPool {
    static_assert(N > 0, "N must be greater than 0");

    constexpr static auto S = sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*);
    constexpr static auto A = alignof(T) > alignof(void*) ? alignof(T) : alignof(void*);

    constexpr static auto BLOCK_SIZE = (S + A - 1) / A * A;

    alignas(A) std::array<std::byte, N * BLOCK_SIZE> data_;
    void* free_ptr_;

public:

    MemoryPool() : free_ptr_(nullptr) {
        for (std::size_t i = 0; i < N - 1; ++i) {
            std::byte* curr = data_.data() + i * BLOCK_SIZE;
            std::byte* next = curr + BLOCK_SIZE;
            std::memcpy(curr, &next, sizeof(void*));
        }

        // Store nullptr in the last block
        void* nullp = nullptr;
        std::memcpy(data_.data() + (N - 1) * BLOCK_SIZE, &nullp, sizeof(void*));

        // Make free_ptr_ point to the first free block
        free_ptr_ = data_.data();
    }

    T* allocate() {
        if (free_ptr_ == nullptr) {
            throw std::bad_alloc();
        }
        T* ptr = reinterpret_cast<T*>(free_ptr_);
        std::memcpy(&free_ptr_, free_ptr_, sizeof(void*));
        return ptr;
    }

    void deallocate(T* ptr) {
        std::memcpy(ptr, &free_ptr_, sizeof(void*));
        std::memcpy(&free_ptr_, &ptr, sizeof(void*));
    }
};
}
#endif //MEMORY_POOL_H
