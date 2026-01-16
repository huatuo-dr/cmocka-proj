/**
 * @file test_calc.cpp
 * @brief Catch2 unit tests for calc module
 *
 * Demonstrates Catch2 features:
 * - TEST_CASE macro for defining test cases
 * - SECTION macro for sharing setup/teardown code
 * - REQUIRE macro (stops on failure)
 * - CHECK macro (continues on failure)
 * - Various assertion macros
 */

#include <catch2/catch_test_macros.hpp>

// C header needs extern "C"
extern "C" {
#include "calc.h"
}

/* ========== Basic Tests (TEST_CASE macro) ========== */

TEST_CASE("calc_add returns correct sum", "[calc][add]") {
    // Positive numbers
    REQUIRE(calc_add(2, 3) == 5);
    REQUIRE(calc_add(100, 200) == 300);

    // Negative numbers
    REQUIRE(calc_add(-2, -3) == -5);
    REQUIRE(calc_add(-100, -200) == -300);

    // Mixed numbers
    REQUIRE(calc_add(-5, 3) == -2);
    REQUIRE(calc_add(5, -3) == 2);

    // With zero
    REQUIRE(calc_add(0, 0) == 0);
    REQUIRE(calc_add(5, 0) == 5);
    REQUIRE(calc_add(0, 5) == 5);
}

TEST_CASE("calc_subtract returns correct difference", "[calc][subtract]") {
    REQUIRE(calc_subtract(5, 3) == 2);
    REQUIRE(calc_subtract(100, 50) == 50);
    REQUIRE(calc_subtract(3, 5) == -2);
    REQUIRE(calc_subtract(0, 0) == 0);
}

TEST_CASE("calc_multiply returns correct product", "[calc][multiply]") {
    REQUIRE(calc_multiply(3, 4) == 12);
    REQUIRE(calc_multiply(10, 10) == 100);
    REQUIRE(calc_multiply(5, 0) == 0);
    REQUIRE(calc_multiply(0, 5) == 0);
    REQUIRE(calc_multiply(-3, 4) == -12);
    REQUIRE(calc_multiply(-3, -4) == 12);
}

TEST_CASE("calc_divide returns correct quotient", "[calc][divide]") {
    REQUIRE(calc_divide(10, 2) == 5);
    REQUIRE(calc_divide(100, 10) == 10);
    REQUIRE(calc_divide(7, 3) == 2);   // Integer truncation
    REQUIRE(calc_divide(10, 3) == 3);
    REQUIRE(calc_divide(10, 0) == 0);  // Division by zero returns 0
    REQUIRE(calc_divide(-10, 2) == -5);
    REQUIRE(calc_divide(10, -2) == -5);
    REQUIRE(calc_divide(-10, -2) == 5);
}

/* ========== SECTION Demo (Shared Setup) ========== */

TEST_CASE("Calculator operations with shared setup", "[calc][section]") {
    // This setup code runs before EACH section
    int operand_a = 10;
    int operand_b = 5;

    SECTION("Addition") {
        REQUIRE(calc_add(operand_a, operand_b) == 15);
        REQUIRE(calc_add(operand_a, 0) == operand_a);
    }

    SECTION("Subtraction") {
        REQUIRE(calc_subtract(operand_a, operand_b) == 5);
        REQUIRE(calc_subtract(operand_a, operand_a) == 0);
    }

    SECTION("Multiplication") {
        REQUIRE(calc_multiply(operand_a, operand_b) == 50);
        REQUIRE(calc_multiply(operand_a, 1) == operand_a);
    }

    SECTION("Division") {
        REQUIRE(calc_divide(operand_a, operand_b) == 2);
        REQUIRE(calc_divide(operand_a, 1) == operand_a);
    }
}

/* ========== Nested SECTION Demo ========== */

TEST_CASE("Nested sections for complex scenarios", "[calc][section][nested]") {
    int base = 100;

    SECTION("With positive operand") {
        int operand = 10;

        SECTION("Addition increases value") {
            int result = calc_add(base, operand);
            REQUIRE(result > base);
            REQUIRE(result == 110);
        }

        SECTION("Subtraction decreases value") {
            int result = calc_subtract(base, operand);
            REQUIRE(result < base);
            REQUIRE(result == 90);
        }

        SECTION("Multiplication scales value") {
            int result = calc_multiply(base, operand);
            REQUIRE(result == base * operand);
        }
    }

    SECTION("With negative operand") {
        int operand = -10;

        SECTION("Addition decreases value") {
            int result = calc_add(base, operand);
            REQUIRE(result < base);
            REQUIRE(result == 90);
        }

        SECTION("Subtraction increases value") {
            int result = calc_subtract(base, operand);
            REQUIRE(result > base);
            REQUIRE(result == 110);
        }
    }

    SECTION("With zero operand") {
        int operand = 0;

        SECTION("Addition returns original") {
            REQUIRE(calc_add(base, operand) == base);
        }

        SECTION("Subtraction returns original") {
            REQUIRE(calc_subtract(base, operand) == base);
        }

        SECTION("Multiplication returns zero") {
            REQUIRE(calc_multiply(base, operand) == 0);
        }
    }
}

/* ========== CHECK vs REQUIRE Demo ========== */

TEST_CASE("CHECK continues after failure, REQUIRE stops", "[calc][assertions]") {
    // CHECK continues execution even if assertion fails
    CHECK(calc_add(1, 1) == 2);
    CHECK(calc_add(2, 2) == 4);
    CHECK(calc_add(3, 3) == 6);

    // REQUIRE stops execution if assertion fails
    REQUIRE(calc_add(4, 4) == 8);

    // These will still run because REQUIRE above passed
    CHECK(calc_subtract(10, 5) == 5);
    CHECK(calc_multiply(3, 3) == 9);
}

/* ========== Comparison Assertions Demo ========== */

TEST_CASE("Various comparison assertions", "[calc][assertions]") {
    int result = calc_add(10, 5);

    // Equality
    REQUIRE(result == 15);

    // Inequality
    REQUIRE(result != 10);

    // Relational
    REQUIRE(result > 10);
    REQUIRE(result >= 15);
    REQUIRE(result < 20);
    REQUIRE(result <= 15);

    // Boolean expressions
    REQUIRE((result > 0) == true);
    REQUIRE((result < 0) == false);
}

/* ========== REQUIRE_FALSE Demo ========== */

TEST_CASE("REQUIRE_FALSE for negative assertions", "[calc][assertions]") {
    REQUIRE_FALSE(calc_add(1, 1) == 3);
    REQUIRE_FALSE(calc_multiply(5, 0) != 0);
    REQUIRE_FALSE(calc_divide(10, 2) == 6);
}
