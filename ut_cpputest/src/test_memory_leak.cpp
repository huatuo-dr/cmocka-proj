/**
 * @file test_memory_leak.cpp
 * @brief Memory leak detection demonstration using CppUTest
 *
 * CppUTest has built-in memory leak detection that automatically checks
 * for memory leaks after each test. This file demonstrates:
 * - How CppUTest detects memory leaks
 * - How to properly manage memory in tests
 * - How to temporarily disable leak detection if needed
 *
 * NOTE: The "DetectLeak" test is intentionally commented out because
 * it would cause the test suite to fail. Uncomment it to see how
 * CppUTest reports memory leaks.
 */

#include "CppUTest/TestHarness.h"
#include "CppUTest/MemoryLeakDetectorNewMacros.h"

#include <cstring>

/*============================================================================
 * Test Group: MemoryLeakDemo - Memory leak detection demonstrations
 *===========================================================================*/

TEST_GROUP(MemoryLeakDemo)
{
    void setup()
    {
        // CppUTest automatically takes a memory snapshot here
    }

    void teardown()
    {
        // CppUTest automatically checks for leaks here
    }
};

/**
 * Test that properly allocates and frees memory - should PASS
 */
TEST(MemoryLeakDemo, NoLeakWithNew)
{
    char* buffer = new char[100];
    std::memset(buffer, 0, 100);
    buffer[0] = 'A';

    CHECK_EQUAL('A', buffer[0]);

    delete[] buffer;  // Properly freed
}

/**
 * Test that properly allocates and frees memory using malloc - should PASS
 */
TEST(MemoryLeakDemo, NoLeakWithMalloc)
{
    char* buffer = (char*)std::malloc(100);
    std::memset(buffer, 0, 100);
    buffer[0] = 'B';

    CHECK_EQUAL('B', buffer[0]);

    std::free(buffer);  // Properly freed
}

/**
 * Test with multiple allocations - should PASS
 */
TEST(MemoryLeakDemo, MultipleAllocationsNoLeak)
{
    int* arr1 = new int[10];
    int* arr2 = new int[20];
    int* arr3 = new int[30];

    arr1[0] = 1;
    arr2[0] = 2;
    arr3[0] = 3;

    CHECK_EQUAL(1, arr1[0]);
    CHECK_EQUAL(2, arr2[0]);
    CHECK_EQUAL(3, arr3[0]);

    delete[] arr3;
    delete[] arr2;
    delete[] arr1;
}

/**
 * Test with object allocation - should PASS
 */
TEST(MemoryLeakDemo, ObjectAllocationNoLeak)
{
    struct Point {
        int x;
        int y;
    };

    Point* p = new Point();
    p->x = 10;
    p->y = 20;

    CHECK_EQUAL(10, p->x);
    CHECK_EQUAL(20, p->y);

    delete p;
}

/*============================================================================
 * Test Group: MemoryLeakExpected - Tests with expected leaks
 *
 * Sometimes you need to test code that intentionally leaks memory,
 * or you're testing third-party code that has known leaks.
 * CppUTest provides ways to handle this.
 *===========================================================================*/

TEST_GROUP(MemoryLeakExpected)
{
};

/**
 * Test that expects a specific number of leaks
 * This is useful when testing code that intentionally doesn't free memory
 */
TEST(MemoryLeakExpected, ExpectOneLeak)
{
    // Tell CppUTest to expect 1 memory leak
    EXPECT_N_LEAKS(1);

    // This allocation will not be freed - but test will PASS
    // because we told CppUTest to expect it
    char* intentional_leak = new char[50];
    intentional_leak[0] = 'X';
    (void)intentional_leak;  // Suppress unused warning
}

/**
 * Test that expects multiple leaks
 */
TEST(MemoryLeakExpected, ExpectMultipleLeaks)
{
    EXPECT_N_LEAKS(3);

    // Three intentional leaks
    int* leak1 = new int(1);
    int* leak2 = new int(2);
    int* leak3 = new int(3);

    CHECK_EQUAL(1, *leak1);
    CHECK_EQUAL(2, *leak2);
    CHECK_EQUAL(3, *leak3);

    // Not freeing - but test will PASS due to EXPECT_N_LEAKS
}

/*============================================================================
 * Commented Out: Actual Memory Leak Test
 *
 * Uncomment the following test to see how CppUTest reports memory leaks.
 * WARNING: This test will FAIL when run!
 *===========================================================================*/

#if 0  // Change to 1 to enable the leak test

TEST_GROUP(MemoryLeakActual)
{
};

/**
 * Test that actually leaks memory - will FAIL
 *
 * When run, CppUTest will report something like:
 *
 * TEST(MemoryLeakActual, DetectLeak)
 * Memory leak(s) found.
 * Alloc num (1) Leak size: 100 Allocated at: test_memory_leak.cpp:XXX
 *
 * Total number of leaks: 1
 */
TEST(MemoryLeakActual, DetectLeak)
{
    char* leak = new char[100];
    leak[0] = 'L';
    (void)leak;

    // Forgot to delete[] leak; - CppUTest will catch this!

    CHECK_EQUAL('L', leak[0]);
}

#endif
