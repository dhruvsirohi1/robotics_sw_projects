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
    // static_assert(sizeof(T) > sizeof(void*), " T must be greater than sizeof(void*)");

    constexpr static auto A  = std::max(alignof(T), alignof(void*));
    constexpr static auto S  = std::max(sizeof(T), sizeof(void*));

    constexpr static auto BLOCK_SIZE = (S + A - 1) / A * A;

    std::array<std::byte, N * BLOCK_SIZE> data_;
    void* free_ptr_;

public:

    MemoryPool() : free_ptr_(nullptr) {
        // Store the address of the (n+1)th block in the nth block
        for (int i = 0; i < N - 1; i++) {
            std::byte* curr = data_.data() + (i * sizeof(T));
            std::byte* next = curr + sizeof(T);
            std::memcpy(curr, &next, sizeof(T));
        }

        // Store nullptr in the last
        void* nullp = nullptr;
        std::memcpy(data_.data() + (N - 1) * sizeof(T), &nullp, sizeof(void*));
        // Store the address of the first block in free_ptr_
        free_ptr_ = data_.data();
    }

    T* allocate() {
        if (free_ptr_ == nullptr) {
            throw std::runtime_error("No memory pool available");
        }
        T* ptr = static_cast<T*>(free_ptr_);
        std::memcpy(&free_ptr_, free_ptr_, sizeof(void*));
        return ptr;
    }

    void deallocate(T* ptr) {
        if (ptr == nullptr) {
            throw std::runtime_error("Can't deallocate nullptr");
        }
        std::memcpy(ptr, &free_ptr_, sizeof(void*));
        std::memcpy(&free_ptr_, &ptr, sizeof(void*));
    }
};
}
#endif //MEMORY_POOL_H
