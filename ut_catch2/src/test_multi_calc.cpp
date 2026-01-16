/**
 * @file test_multi_calc.cpp
 * @brief Catch2 unit tests for multi-calc module with mock support
 *
 * Demonstrates mocking C functions using linker --wrap option:
 * - __wrap_xxx replaces original function
 * - __real_xxx calls original function
 * - Manual mock state management
 *
 * Note: Catch2 does not have built-in mock support like GMock.
 * This file uses the same --wrap approach as other frameworks in this project.
 */

#include <catch2/catch_test_macros.hpp>

// C headers need extern "C"
extern "C" {
#include "calc.h"
#include "multi-calc.h"
}

/* ========== Mock Control Flags ========== */

static bool mock_calc_add = true;
static bool mock_calc_subtract = true;
static bool mock_calc_multiply = true;
static bool mock_calc_divide = true;

// Mock return values
static int mock_add_return = 0;
static int mock_subtract_return = 0;
static int mock_multiply_return = 0;
static int mock_divide_return = 0;

// Call tracking
static int add_call_count = 0;
static int subtract_call_count = 0;
static int multiply_call_count = 0;
static int divide_call_count = 0;

// Argument capture
static int last_add_a = 0, last_add_b = 0;
static int last_subtract_a = 0, last_subtract_b = 0;
static int last_multiply_a = 0, last_multiply_b = 0;
static int last_divide_a = 0, last_divide_b = 0;

/* ========== Real Function Declarations ========== */

extern "C" {
    extern int __real_calc_add(int a, int b);
    extern int __real_calc_subtract(int a, int b);
    extern int __real_calc_multiply(int a, int b);
    extern int __real_calc_divide(int a, int b);
}

/* ========== Wrap Functions ========== */

extern "C" {

int __wrap_calc_add(int a, int b) {
    add_call_count++;
    last_add_a = a;
    last_add_b = b;
    if (mock_calc_add) {
        return mock_add_return;
    }
    return __real_calc_add(a, b);
}

int __wrap_calc_subtract(int a, int b) {
    subtract_call_count++;
    last_subtract_a = a;
    last_subtract_b = b;
    if (mock_calc_subtract) {
        return mock_subtract_return;
    }
    return __real_calc_subtract(a, b);
}

int __wrap_calc_multiply(int a, int b) {
    multiply_call_count++;
    last_multiply_a = a;
    last_multiply_b = b;
    if (mock_calc_multiply) {
        return mock_multiply_return;
    }
    return __real_calc_multiply(a, b);
}

int __wrap_calc_divide(int a, int b) {
    divide_call_count++;
    last_divide_a = a;
    last_divide_b = b;
    if (mock_calc_divide) {
        return mock_divide_return;
    }
    return __real_calc_divide(a, b);
}

} // extern "C"

/* ========== Helper Functions ========== */

static void enable_all_mocks() {
    mock_calc_add = true;
    mock_calc_subtract = true;
    mock_calc_multiply = true;
    mock_calc_divide = true;
}

static void disable_all_mocks() {
    mock_calc_add = false;
    mock_calc_subtract = false;
    mock_calc_multiply = false;
    mock_calc_divide = false;
}

static void reset_mock_state() {
    add_call_count = 0;
    subtract_call_count = 0;
    multiply_call_count = 0;
    divide_call_count = 0;
    mock_add_return = 0;
    mock_subtract_return = 0;
    mock_multiply_return = 0;
    mock_divide_return = 0;
    last_add_a = 0;
    last_add_b = 0;
    last_subtract_a = 0;
    last_subtract_b = 0;
    last_multiply_a = 0;
    last_multiply_b = 0;
    last_divide_a = 0;
    last_divide_b = 0;
}

/* ========== Mock Tests for multi_calc_expression ========== */

// Test: (a + b) * (c - d)
TEST_CASE("multi_calc_expression with mocked dependencies", "[multi-calc][mock]") {
    // Setup for each section
    enable_all_mocks();
    reset_mock_state();

    SECTION("Returns mocked result") {
        mock_add_return = 5;       // calc_add(2, 3) -> 5
        mock_subtract_return = 6;  // calc_subtract(10, 4) -> 6
        mock_multiply_return = 30; // calc_multiply(5, 6) -> 30

        int result = multi_calc_expression(2, 3, 10, 4);

        REQUIRE(result == 30);
    }

    SECTION("Calls dependencies correct number of times") {
        mock_add_return = 10;
        mock_subtract_return = 20;
        mock_multiply_return = 200;

        multi_calc_expression(1, 2, 3, 4);

        REQUIRE(add_call_count == 1);
        REQUIRE(subtract_call_count == 1);
        REQUIRE(multiply_call_count == 1);
        REQUIRE(divide_call_count == 0);  // Not used in expression
    }

    SECTION("Captures correct arguments") {
        mock_add_return = 10;
        mock_subtract_return = 20;
        mock_multiply_return = 200;

        multi_calc_expression(1, 2, 3, 4);

        // Verify arguments passed to calc_add
        REQUIRE(last_add_a == 1);
        REQUIRE(last_add_b == 2);

        // Verify arguments passed to calc_subtract
        REQUIRE(last_subtract_a == 3);
        REQUIRE(last_subtract_b == 4);

        // Verify arguments passed to calc_multiply
        REQUIRE(last_multiply_a == 10);  // Result of add
        REQUIRE(last_multiply_b == 20);  // Result of subtract
    }

    // Cleanup
    disable_all_mocks();
}

/* ========== Mock Tests for multi_calc_average ========== */

// Test: (a + b + c) / 3
TEST_CASE("multi_calc_average with mocked dependencies", "[multi-calc][mock]") {
    enable_all_mocks();
    reset_mock_state();

    SECTION("Returns mocked result") {
        mock_add_return = 6;      // Both adds return 6
        mock_divide_return = 2;   // Final divide returns 2

        int result = multi_calc_average(1, 2, 3);

        REQUIRE(result == 2);
    }

    SECTION("Calls add twice and divide once") {
        mock_add_return = 100;
        mock_divide_return = 33;

        multi_calc_average(10, 20, 30);

        REQUIRE(add_call_count == 2);     // Called twice for sum
        REQUIRE(divide_call_count == 1);  // Called once for average
        REQUIRE(multiply_call_count == 0);
        REQUIRE(subtract_call_count == 0);
    }

    SECTION("Divides sum by 3") {
        mock_add_return = 99;
        mock_divide_return = 33;

        multi_calc_average(10, 20, 30);

        REQUIRE(last_divide_a == 99);  // Sum from mocked add
        REQUIRE(last_divide_b == 3);   // Divisor is always 3
    }

    disable_all_mocks();
}

/* ========== Hybrid Tests (Real + Mock) ========== */

TEST_CASE("multi_calc with real functions", "[multi-calc][real]") {
    disable_all_mocks();
    reset_mock_state();

    SECTION("Expression with real functions") {
        // (2 + 3) * (10 - 4) = 5 * 6 = 30
        int result = multi_calc_expression(2, 3, 10, 4);
        REQUIRE(result == 30);
    }

    SECTION("Average with real functions") {
        // (1 + 2 + 3) / 3 = 6 / 3 = 2
        int result = multi_calc_average(1, 2, 3);
        REQUIRE(result == 2);
    }
}

TEST_CASE("Partial mock - only multiply mocked", "[multi-calc][mock][partial]") {
    reset_mock_state();

    // Use real add and subtract, mock multiply
    mock_calc_add = false;
    mock_calc_subtract = false;
    mock_calc_multiply = true;
    mock_multiply_return = 999;

    // Real: 2+3=5, 10-4=6, Mock: 5*6=999
    int result = multi_calc_expression(2, 3, 10, 4);

    REQUIRE(result == 999);
    REQUIRE(last_multiply_a == 5);   // Real add result
    REQUIRE(last_multiply_b == 6);   // Real subtract result

    disable_all_mocks();
}

/* ========== BDD Style Mock Tests ========== */

SCENARIO("Mocking calc dependencies for expression", "[multi-calc][mock][bdd]") {

    GIVEN("Mocked calc functions") {
        enable_all_mocks();
        reset_mock_state();

        AND_GIVEN("Predefined mock return values") {
            mock_add_return = 100;
            mock_subtract_return = 50;
            mock_multiply_return = 5000;

            WHEN("multi_calc_expression is called") {
                int result = multi_calc_expression(10, 20, 30, 40);

                THEN("The result comes from mocked multiply") {
                    REQUIRE(result == 5000);
                }

                AND_THEN("All expected functions were called") {
                    REQUIRE(add_call_count == 1);
                    REQUIRE(subtract_call_count == 1);
                    REQUIRE(multiply_call_count == 1);
                }
            }
        }

        // Cleanup
        disable_all_mocks();
    }
}

SCENARIO("Comparing mocked vs real results", "[multi-calc][mock][bdd]") {

    GIVEN("A set of input values") {
        int a = 1, b = 2, c = 3;

        WHEN("Using mocked functions") {
            enable_all_mocks();
            reset_mock_state();
            mock_add_return = 100;
            mock_divide_return = 66;

            int mock_result = multi_calc_average(a, b, c);

            THEN("The result is the mocked value") {
                REQUIRE(mock_result == 66);
            }

            disable_all_mocks();
        }

        WHEN("Using real functions") {
            disable_all_mocks();
            reset_mock_state();

            int real_result = multi_calc_average(a, b, c);

            THEN("The result is the real calculation") {
                // (1 + 2 + 3) / 3 = 2
                REQUIRE(real_result == 2);
            }
        }
    }
}
