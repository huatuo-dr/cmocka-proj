/**
 * @file test_multi_calc.cpp
 * @brief doctest unit tests for multi-calc module with mock functions
 *
 * Demonstrates:
 * - Using linker --wrap option for mocking C functions
 * - __wrap_xxx functions intercept calls
 * - __real_xxx functions call original implementation
 * - Mock control flags and return values
 * - Call tracking and argument capture
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// C headers need extern "C"
extern "C" {
#include "calc.h"
#include "multi-calc.h"
}

/* ========== Mock Control Variables ========== */

// Mock enable flags
static bool mock_calc_add_enabled = false;
static bool mock_calc_subtract_enabled = false;
static bool mock_calc_multiply_enabled = false;
static bool mock_calc_divide_enabled = false;

// Mock return values
static int mock_calc_add_return = 0;
static int mock_calc_subtract_return = 0;
static int mock_calc_multiply_return = 0;
static int mock_calc_divide_return = 0;

// Call tracking
static int mock_calc_add_call_count = 0;
static int mock_calc_subtract_call_count = 0;
static int mock_calc_multiply_call_count = 0;
static int mock_calc_divide_call_count = 0;

// Last call arguments
static int mock_calc_add_last_a = 0;
static int mock_calc_add_last_b = 0;
static int mock_calc_subtract_last_a = 0;
static int mock_calc_subtract_last_b = 0;
static int mock_calc_multiply_last_a = 0;
static int mock_calc_multiply_last_b = 0;

/* ========== Mock Helper Functions ========== */

static void reset_all_mocks() {
    // Reset enable flags
    mock_calc_add_enabled = false;
    mock_calc_subtract_enabled = false;
    mock_calc_multiply_enabled = false;
    mock_calc_divide_enabled = false;

    // Reset return values
    mock_calc_add_return = 0;
    mock_calc_subtract_return = 0;
    mock_calc_multiply_return = 0;
    mock_calc_divide_return = 0;

    // Reset call counts
    mock_calc_add_call_count = 0;
    mock_calc_subtract_call_count = 0;
    mock_calc_multiply_call_count = 0;
    mock_calc_divide_call_count = 0;

    // Reset last arguments
    mock_calc_add_last_a = 0;
    mock_calc_add_last_b = 0;
    mock_calc_subtract_last_a = 0;
    mock_calc_subtract_last_b = 0;
    mock_calc_multiply_last_a = 0;
    mock_calc_multiply_last_b = 0;
}

/* ========== Real Function Declarations ========== */

extern "C" {
    extern int __real_calc_add(int a, int b);
    extern int __real_calc_subtract(int a, int b);
    extern int __real_calc_multiply(int a, int b);
    extern int __real_calc_divide(int a, int b);
}

/* ========== Wrap Functions (Mock Implementation) ========== */

extern "C" {

int __wrap_calc_add(int a, int b) {
    mock_calc_add_call_count++;
    mock_calc_add_last_a = a;
    mock_calc_add_last_b = b;

    if (mock_calc_add_enabled) {
        return mock_calc_add_return;
    }
    return __real_calc_add(a, b);
}

int __wrap_calc_subtract(int a, int b) {
    mock_calc_subtract_call_count++;
    mock_calc_subtract_last_a = a;
    mock_calc_subtract_last_b = b;

    if (mock_calc_subtract_enabled) {
        return mock_calc_subtract_return;
    }
    return __real_calc_subtract(a, b);
}

int __wrap_calc_multiply(int a, int b) {
    mock_calc_multiply_call_count++;
    mock_calc_multiply_last_a = a;
    mock_calc_multiply_last_b = b;

    if (mock_calc_multiply_enabled) {
        return mock_calc_multiply_return;
    }
    return __real_calc_multiply(a, b);
}

int __wrap_calc_divide(int a, int b) {
    mock_calc_divide_call_count++;

    if (mock_calc_divide_enabled) {
        return mock_calc_divide_return;
    }
    return __real_calc_divide(a, b);
}

} // extern "C"

/* ========== Tests Without Mock (Real Functions) ========== */

TEST_SUITE("Multi-calc without mock") {

    TEST_CASE("multi_calc_expression with real functions") {
        reset_all_mocks();

        // (a + b) * (c - d) = (1 + 2) * (7 - 4) = 3 * 3 = 9
        int result = multi_calc_expression(1, 2, 7, 4);
        CHECK(result == 9);
    }

    TEST_CASE("multi_calc_average with real functions") {
        reset_all_mocks();

        // (a + b + c) / 3 = (3 + 6 + 9) / 3 = 18 / 3 = 6
        int result = multi_calc_average(3, 6, 9);
        CHECK(result == 6);
    }
}

/* ========== Tests With Mock ========== */

TEST_SUITE("Multi-calc with mock") {

    TEST_CASE("multi_calc_expression with mocked calc_add") {
        reset_all_mocks();

        // Mock calc_add to return fixed value
        mock_calc_add_enabled = true;
        mock_calc_add_return = 100;

        // (a + b) * (c - d) where (a + b) is mocked to return 100
        // = 100 * (10 - 5) = 100 * 5 = 500
        int result = multi_calc_expression(1, 2, 10, 5);

        CHECK(result == 500);
        CHECK(mock_calc_add_call_count == 1);
        CHECK(mock_calc_add_last_a == 1);
        CHECK(mock_calc_add_last_b == 2);
    }

    TEST_CASE("multi_calc_expression with mocked calc_subtract") {
        reset_all_mocks();

        // Mock calc_subtract to return fixed value
        mock_calc_subtract_enabled = true;
        mock_calc_subtract_return = 50;

        // (a + b) * (c - d) where (c - d) is mocked to return 50
        // = (3 + 4) * 50 = 7 * 50 = 350
        int result = multi_calc_expression(3, 4, 100, 1);

        CHECK(result == 350);
        CHECK(mock_calc_subtract_call_count == 1);
        CHECK(mock_calc_subtract_last_a == 100);
        CHECK(mock_calc_subtract_last_b == 1);
    }

    TEST_CASE("multi_calc_expression with mocked calc_multiply") {
        reset_all_mocks();

        // Mock calc_multiply to return fixed value
        mock_calc_multiply_enabled = true;
        mock_calc_multiply_return = 999;

        // (a + b) * (c - d) where multiply is mocked to return 999
        int result = multi_calc_expression(1, 2, 5, 3);

        CHECK(result == 999);
        CHECK(mock_calc_multiply_call_count == 1);
        // Verify arguments passed to multiply
        CHECK(mock_calc_multiply_last_a == 3);  // 1 + 2
        CHECK(mock_calc_multiply_last_b == 2);  // 5 - 3
    }

    TEST_CASE("multi_calc_average with mocked calc_add") {
        reset_all_mocks();

        // Mock calc_add to return fixed values
        mock_calc_add_enabled = true;
        mock_calc_add_return = 30;

        // Average uses add twice: ((a + b) + c) / 3
        // With mock: (30 + c) but second add also returns 30
        // So result = 30 / 3 = 10
        int result = multi_calc_average(1, 2, 3);

        CHECK(result == 10);
        CHECK(mock_calc_add_call_count == 2);  // add called twice
    }

    TEST_CASE("multi_calc_average with mocked calc_divide") {
        reset_all_mocks();

        // Mock calc_divide to return fixed value
        mock_calc_divide_enabled = true;
        mock_calc_divide_return = 42;

        // Sum is real, but division is mocked
        int result = multi_calc_average(10, 20, 30);

        CHECK(result == 42);
        CHECK(mock_calc_divide_call_count == 1);
    }
}

/* ========== BDD Style Mock Tests ========== */

SCENARIO("Testing multi_calc_expression with controlled dependencies") {

    GIVEN("All mock functions are reset") {
        reset_all_mocks();

        WHEN("calc_add is mocked to return 10") {
            mock_calc_add_enabled = true;
            mock_calc_add_return = 10;

            AND_WHEN("calc_subtract is mocked to return 5") {
                mock_calc_subtract_enabled = true;
                mock_calc_subtract_return = 5;

                THEN("multi_calc_expression should return 10 * 5 = 50") {
                    int result = multi_calc_expression(0, 0, 0, 0);
                    REQUIRE(result == 50);
                }
            }
        }

        WHEN("Only calc_multiply is mocked") {
            mock_calc_multiply_enabled = true;
            mock_calc_multiply_return = 1000;

            THEN("Result should be the mocked multiply value") {
                // Real: (2 + 3) * (8 - 4) = 5 * 4 = 20
                // Mocked: returns 1000 regardless
                int result = multi_calc_expression(2, 3, 8, 4);
                REQUIRE(result == 1000);
            }
        }
    }
}

/* ========== Call Count Verification ========== */

TEST_CASE("Verify function call counts") {
    reset_all_mocks();

    SUBCASE("multi_calc_expression calls pattern") {
        multi_calc_expression(1, 2, 3, 4);

        // Expression: (a + b) * (c - d)
        // Should call: add once, subtract once, multiply once
        CHECK(mock_calc_add_call_count == 1);
        CHECK(mock_calc_subtract_call_count == 1);
        CHECK(mock_calc_multiply_call_count == 1);
        CHECK(mock_calc_divide_call_count == 0);
    }

    SUBCASE("multi_calc_average calls pattern") {
        reset_all_mocks();
        multi_calc_average(1, 2, 3);

        // Average: (a + b + c) / 3
        // Should call: add twice, divide once
        CHECK(mock_calc_add_call_count == 2);
        CHECK(mock_calc_subtract_call_count == 0);
        CHECK(mock_calc_multiply_call_count == 0);
        CHECK(mock_calc_divide_call_count == 1);
    }
}

/* ========== Mixed Real and Mock ========== */

TEST_CASE("Selective mocking - only some functions mocked") {
    reset_all_mocks();

    // Only mock add, let subtract and multiply use real implementations
    mock_calc_add_enabled = true;
    mock_calc_add_return = 20;

    // (a + b) * (c - d) = 20 * (10 - 3) = 20 * 7 = 140
    int result = multi_calc_expression(999, 999, 10, 3);

    CHECK(result == 140);
    CHECK(mock_calc_add_call_count == 1);
    CHECK(mock_calc_subtract_call_count == 1);
    CHECK(mock_calc_multiply_call_count == 1);
}
