//
// Created by Dhruv Sirohi on 8/5/25.
//

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <vector>

namespace memory_pool {

template <typename T, std::size_t NUM_BLOCKS = 1024>
class MemoryPool {

public:
    MemoryPool(): free_ptr_(nullptr), free_list_(nullptr) {
        data_.resize(NUM_BLOCKS * sizeof(T));
        for (int i = 0; i < NUM_BLOCKS - 1; i++) {
            // Write the address of the next block
            // into the first 8 bytes of the current
            // block
            std::byte *curr = data_.data() + i * sizeof(T);
            const std::byte *next = curr + sizeof(T);
            std::memcpy(curr, &next, sizeof(next));
        }
        void* nullp = nullptr;
        std::memcpy(data_.data() + (NUM_BLOCKS - 1) * sizeof(T), &nullp, sizeof(void *));
        // Write the address of the first free block in free_ptr_
        free_ptr_ = data_.data();
    }

    T* allocate() {
        if (free_ptr_ == nullptr) {return nullptr;}
        printf("free_ptr_ before: %p\n", free_ptr_);
        // Store the ptr to be returned
        T* ptr = static_cast<T*>(free_ptr_);
        // Advance the free_ptr_ by
        // dereferencing the current address it points to
        std::memcpy(&free_ptr_, free_ptr_, sizeof(void*));
        printf("free_ptr_ after: %p\n", free_ptr_);
        return ptr;
    }

    void deallocate(T* ptr) {
        // 1. Make ptr point to free_ptr_
        std::memcpy(ptr , &free_ptr_, sizeof(void*));
        // 2. Make the free_ptr_ point to ptr
        std::memcpy(&free_ptr_, &ptr, sizeof(void*));
    }

private:
    std::vector<std::byte> data_;
    void* free_ptr_;
    void** free_list_;
};
}
#endif //MEMORY_POOL_H
