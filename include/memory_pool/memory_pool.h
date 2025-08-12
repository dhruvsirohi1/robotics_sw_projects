//
// Created by Dhruv Sirohi on 8/5/25.
//

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <vector>

namespace memory_pool {

template <typename T, std::size_t N = 1024>
class MemoryPool {
    std::vector<std::byte> buffer_;
    void* free_ptr_;
public:
    MemoryPool(): free_ptr_(nullptr) {
        assert(N > 0);
        // Create buffer, with each block big enough to at least hold a memory address
        buffer_.resize(N * std::max(sizeof(T), sizeof(void*)));
        for (std::size_t i = 0; i < N - 1; ++i) {
            std::byte* curr = buffer_.data() + i * std::max(sizeof(T), sizeof(void*));
            std::byte* next = curr + std::max(sizeof(T), sizeof(void*));
            std::memcpy(curr, &next, sizeof(void*));
        }
        // Let the last block point to nullptr
        void* nullp = nullptr;
        std::memcpy(buffer_.data() + (N - 1) * std::max(sizeof(T), sizeof(void*)), &nullp, sizeof(void*));
        // Let the free_ptr_ point to the first block
        std::memcpy(&free_ptr_, &buffer_, sizeof(void*));
    }

    T* allocate() {
        if (free_ptr_ == nullptr) {return nullptr;}
        T* ret = static_cast<T*>(free_ptr_);
        std::memcpy(&free_ptr_, free_ptr_, sizeof(void*));
        return ret;
    }

    void deallocate(T* ptr) {
        // Write the next free block in the ptr to be deallocated
        std::memcpy(ptr, &free_ptr_, sizeof(void*));
        // Make free_ptr_ point to newly free ptr
        std::memcpy(&free_ptr_, &ptr, sizeof(void*));
    }
};
}
#endif //MEMORY_POOL_H
