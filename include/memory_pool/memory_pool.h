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

    static constexpr auto A = alignof(T) > alignof(void*) ? alignof(T) : alignof(void*);
    static constexpr auto B = sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*);

    static constexpr auto BLOCK_SIZE = (B + A - 1) / A * A;

    std::array<std::byte, N * BLOCK_SIZE> data_;
    void* free_ptr_;

public:
    MemoryPool() : free_ptr_(nullptr) {
        printf("MemoryPool::MemoryPool()\n");

        // Store the address of the next free block in the current block
        for (int i = 0; i < N - 1; i++) {
            std::byte* current = data_.data() + i * BLOCK_SIZE;
            std::byte* next = current + BLOCK_SIZE;
            std::memcpy(current, &next, sizeof(void*));
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
