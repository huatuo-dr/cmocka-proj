/**
 * @file test_calc.cpp
 * @brief doctest unit tests for calc module
 *
 * Demonstrates doctest features:
 * - TEST_CASE macro
 * - SUBCASE mechanism (similar to Catch2's SECTION)
 * - TEST_SUITE for grouping tests
 * - Decorators (skip, timeout, description, may_fail)
 * - CHECK vs REQUIRE assertions
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

// C header needs extern "C"
extern "C" {
#include "calc.h"
}

/* ========== Basic TEST_CASE ========== */

TEST_CASE("calc_add returns correct sum") {
    CHECK(calc_add(1, 2) == 3);
    CHECK(calc_add(0, 0) == 0);
    CHECK(calc_add(-1, 1) == 0);
    CHECK(calc_add(-5, -3) == -8);
}

TEST_CASE("calc_subtract returns correct difference") {
    CHECK(calc_subtract(5, 3) == 2);
    CHECK(calc_subtract(0, 0) == 0);
    CHECK(calc_subtract(-1, -1) == 0);
    CHECK(calc_subtract(10, -5) == 15);
}

/* ========== SUBCASE Mechanism ========== */

TEST_CASE("Calculator operations with shared setup using SUBCASE") {
    // This setup runs BEFORE EACH subcase
    int operand_a = 10;
    int operand_b = 5;

    SUBCASE("Addition") {
        REQUIRE(calc_add(operand_a, operand_b) == 15);
    }

    SUBCASE("Subtraction") {
        REQUIRE(calc_subtract(operand_a, operand_b) == 5);
    }

    SUBCASE("Multiplication") {
        REQUIRE(calc_multiply(operand_a, operand_b) == 50);
    }

    SUBCASE("Division") {
        REQUIRE(calc_divide(operand_a, operand_b) == 2);
    }
}

/* ========== Nested SUBCASE ========== */

TEST_CASE("Nested subcases for complex scenarios") {
    int base = 100;

    SUBCASE("With positive operand") {
        int operand = 10;

        SUBCASE("Addition increases value") {
            REQUIRE(calc_add(base, operand) == 110);
        }

        SUBCASE("Subtraction decreases value") {
            REQUIRE(calc_subtract(base, operand) == 90);
        }

        SUBCASE("Multiplication scales value") {
            REQUIRE(calc_multiply(base, operand) == 1000);
        }
    }

    SUBCASE("With negative operand") {
        int operand = -10;

        SUBCASE("Addition decreases value") {
            REQUIRE(calc_add(base, operand) == 90);
        }

        SUBCASE("Subtraction increases value") {
            REQUIRE(calc_subtract(base, operand) == 110);
        }
    }

    SUBCASE("With zero operand") {
        int operand = 0;

        SUBCASE("Addition returns base") {
            REQUIRE(calc_add(base, operand) == base);
        }

        SUBCASE("Multiplication returns zero") {
            REQUIRE(calc_multiply(base, operand) == 0);
        }
    }
}

/* ========== TEST_SUITE for Grouping ========== */

TEST_SUITE("Multiplication Tests") {
    TEST_CASE("Basic multiplication") {
        CHECK(calc_multiply(2, 3) == 6);
        CHECK(calc_multiply(0, 100) == 0);
        CHECK(calc_multiply(1, 999) == 999);
    }

    TEST_CASE("Multiplication with negative numbers") {
        CHECK(calc_multiply(-2, 3) == -6);
        CHECK(calc_multiply(-2, -3) == 6);
        CHECK(calc_multiply(2, -3) == -6);
    }

    TEST_CASE("Multiplication edge cases") {
        CHECK(calc_multiply(0, 0) == 0);
        CHECK(calc_multiply(-1, -1) == 1);
    }
}

TEST_SUITE("Division Tests") {
    TEST_CASE("Basic division") {
        CHECK(calc_divide(10, 2) == 5);
        CHECK(calc_divide(9, 3) == 3);
        CHECK(calc_divide(100, 10) == 10);
    }

    TEST_CASE("Division with negative numbers") {
        CHECK(calc_divide(-10, 2) == -5);
        CHECK(calc_divide(-10, -2) == 5);
        CHECK(calc_divide(10, -2) == -5);
    }

    TEST_CASE("Integer division truncation") {
        CHECK(calc_divide(7, 2) == 3);
        CHECK(calc_divide(10, 3) == 3);
        CHECK(calc_divide(-7, 2) == -3);
    }
}

/* ========== Decorators ========== */

TEST_CASE("Test with description decorator"
          * doctest::description("This test demonstrates the description decorator")) {
    CHECK(calc_add(1, 1) == 2);
}

TEST_CASE("Test with timeout decorator"
          * doctest::timeout(1.0)) {
    // This test must complete within 1 second
    for (int i = 0; i < 1000; i++) {
        CHECK(calc_add(i, 1) == i + 1);
    }
}

TEST_CASE("Test marked as may_fail"
          * doctest::may_fail()) {
    // This test is expected to potentially fail
    // It won't fail the overall test run even if it fails
    CHECK(calc_divide(10, 3) == 3);  // Integer division
}

TEST_CASE("Skipped test example"
          * doctest::skip(true)
          * doctest::description("This test is skipped")) {
    // This test will be skipped
    REQUIRE(false);  // Would fail if not skipped
}

/* ========== TEST_SUITE with Decorators ========== */

TEST_SUITE("Edge Cases" * doctest::description("Tests for boundary conditions")) {
    TEST_CASE("Zero operations") {
        CHECK(calc_add(0, 0) == 0);
        CHECK(calc_subtract(0, 0) == 0);
        CHECK(calc_multiply(0, 0) == 0);
    }

    TEST_CASE("Identity operations") {
        int value = 42;
        CHECK(calc_add(value, 0) == value);
        CHECK(calc_subtract(value, 0) == value);
        CHECK(calc_multiply(value, 1) == value);
        CHECK(calc_divide(value, 1) == value);
    }
}

/* ========== CHECK vs REQUIRE ========== */

TEST_CASE("Difference between CHECK and REQUIRE") {
    // CHECK: continues on failure, reports all failures
    CHECK(calc_add(1, 1) == 2);
    CHECK(calc_add(2, 2) == 4);
    CHECK(calc_add(3, 3) == 6);

    // REQUIRE: stops on failure, critical assertions
    REQUIRE(calc_add(0, 0) == 0);

    // More checks after REQUIRE
    CHECK(calc_subtract(10, 5) == 5);
}

/* ========== Other Assertion Macros ========== */

TEST_CASE("Various assertion macros") {
    SUBCASE("Comparison assertions") {
        CHECK_EQ(calc_add(1, 2), 3);
        CHECK_NE(calc_add(1, 2), 0);
        CHECK_GT(calc_add(1, 2), 2);
        CHECK_LT(calc_add(1, 2), 4);
        CHECK_GE(calc_add(1, 2), 3);
        CHECK_LE(calc_add(1, 2), 3);
    }

    SUBCASE("Boolean assertions") {
        CHECK_FALSE(calc_add(0, 0) != 0);
        CHECK_UNARY(calc_add(1, 0) == 1);
        CHECK_UNARY_FALSE(calc_add(1, 0) == 0);
    }
}
