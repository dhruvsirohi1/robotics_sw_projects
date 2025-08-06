//
// Created by Dhruv Sirohi on 8/5/25.
//

#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H
#include <vector>

namespace memory_pool {

template<typename T, std::size_t BLOCK_COUNT = 1024>
class MemoryPool {
public:
    MemoryPool() {
        static_assert(BLOCK_COUNT > 0);
        printf("Allocating %lu bytes per block for T\n", std::max(sizeof(T), sizeof(void*)));
        const std::size_t block_size = std::max(sizeof(T), sizeof(void*));
        memory_.resize(BLOCK_COUNT * block_size);
        free_list_ = reinterpret_cast<void**>(memory_.data());

        for (std::size_t i = 1; i < BLOCK_COUNT - 1; i += 2) {
            free_list_[i] = (free_list_ + i * block_size);
            // printf("Address of free_list_[i]: %p\n", free_list_[i]);
        }

        free_list_[BLOCK_COUNT - 1] = nullptr;
        free_ptr_ = reinterpret_cast<void*>(free_list_);
    }

    ~MemoryPool() = default;

    /**
     * Allocates a single block of memory.
     **/
    T* allocate() {
        if (!free_ptr_) {
            throw std::bad_alloc();
        }
        void* ptr = free_ptr_;
        free_ptr_ = *reinterpret_cast<void**>(free_ptr_);
        return static_cast<T*>(ptr);
    }

    void deallocate(T* ptr) {
        *reinterpret_cast<void**>(ptr) = free_ptr_;
        free_ptr_ = ptr;
    }

private:
    std::vector<std::byte> memory_;
    void* free_ptr_;
    void** free_list_;
};
}
#endif //MEMORY_POOL_H
