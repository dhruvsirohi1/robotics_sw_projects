//
// Created by Dhruv Sirohi on 8/6/25.
//

#include <gtest/gtest.h>
#include <memory_pool/memory_pool.h>
#include <string>

using namespace memory_pool;

struct TestStruct {
    int id;
    std::string name;

    TestStruct(int i, std::string n) : id(i), name(std::move(n)) {}
};

TEST(MemoryPoolTest, AllocateDeallocate) {
    MemoryPool<TestStruct, 5> pool;

    TestStruct* obj = pool.allocate();
    TestStruct* obj2 = pool.allocate();
    new (obj) TestStruct(1, "robot");
    new (obj2) TestStruct(2, "robot2");
    printf("Size of TestStruct: %lu\n", sizeof(TestStruct));
    printf("Address of obj1: %p\n", obj);
    printf("Address of obj2: %p\n", obj2);
    EXPECT_EQ(obj->id, 1);
    EXPECT_EQ(obj->name, "robot");

    obj->~TestStruct();   // manually call destructor
    obj2->~TestStruct();
    pool.deallocate(obj);
    pool.deallocate(obj2);
    TestStruct* c = pool.allocate();
    new (c) TestStruct(30, "C");
    EXPECT_EQ(c->id, 30);
    c->~TestStruct();
    pool.deallocate(c);
    printf("\n\nwor\n\n");
}

TEST(MemoryPoolTest, MemoryReuse) {
    printf("Test 2\n");
    MemoryPool<TestStruct, 2> pool;

    TestStruct* a = pool.allocate();
    new (a) TestStruct(10, "A");

    TestStruct* b = pool.allocate();
    new (b) TestStruct(20, "B");

    a->~TestStruct();
    b->~TestStruct();
    pool.deallocate(a);
    pool.deallocate(b);

    // Allocate again and check reuse
    TestStruct* c = pool.allocate();
    new (c) TestStruct(30, "C");
    EXPECT_EQ(c->id, 30);
    c->~TestStruct();
    pool.deallocate(c);
}

// TEST(MemoryPoolTest, ExhaustionThrows) {
//     MemoryPool<int, 1> pool;
//     int* a = pool.allocate();
//     EXPECT_THROW(pool.allocate(), std::bad_alloc);
//     pool.deallocate(a);
// }

TEST(MemoryPoolTest, MultipleAllocFreeCycle) {
    printf("Test 3\n");
    MemoryPool<int, 3> pool;
    for (int i = 0; i < 1000; ++i) {
        int* a = pool.allocate();
        *a = i;
        pool.deallocate(a);
    }
    SUCCEED();  // If it survives 1000 cycles, it's good
}
