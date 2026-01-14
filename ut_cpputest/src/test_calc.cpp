/**
 * @file test_calc.cpp
 * @brief Unit tests for calc module using CppUTest
 *
 * Demonstrates CppUTest features:
 * - TEST_GROUP and TEST macros
 * - Basic assertions (CHECK_EQUAL, CHECK_TRUE, etc.)
 * - Setup and teardown methods
 */

#include "CppUTest/TestHarness.h"

extern "C" {
    #include "calc.h"
}

/*============================================================================
 * Test Group: CalcAdd - Tests for calc_add function
 *===========================================================================*/

TEST_GROUP(CalcAdd)
{
    // Setup is called before each test
    void setup() {}

    // Teardown is called after each test
    void teardown() {}
};

TEST(CalcAdd, PositiveNumbers)
{
    CHECK_EQUAL(5, calc_add(2, 3));
    CHECK_EQUAL(300, calc_add(100, 200));
}

TEST(CalcAdd, NegativeNumbers)
{
    CHECK_EQUAL(-5, calc_add(-2, -3));
    CHECK_EQUAL(-300, calc_add(-100, -200));
}

TEST(CalcAdd, MixedNumbers)
{
    CHECK_EQUAL(-2, calc_add(-5, 3));
    CHECK_EQUAL(2, calc_add(5, -3));
}

TEST(CalcAdd, Zero)
{
    CHECK_EQUAL(0, calc_add(0, 0));
    CHECK_EQUAL(5, calc_add(5, 0));
    CHECK_EQUAL(5, calc_add(0, 5));
}

/*============================================================================
 * Test Group: CalcSubtract - Tests for calc_subtract function
 *===========================================================================*/

TEST_GROUP(CalcSubtract)
{
};

TEST(CalcSubtract, PositiveNumbers)
{
    CHECK_EQUAL(2, calc_subtract(5, 3));
    CHECK_EQUAL(70, calc_subtract(100, 30));
}

TEST(CalcSubtract, NegativeResult)
{
    CHECK_EQUAL(-2, calc_subtract(3, 5));
    CHECK_EQUAL(-100, calc_subtract(0, 100));
}

TEST(CalcSubtract, NegativeNumbers)
{
    CHECK_EQUAL(-2, calc_subtract(-5, -3));
    CHECK_EQUAL(2, calc_subtract(-3, -5));
}

TEST(CalcSubtract, Zero)
{
    CHECK_EQUAL(0, calc_subtract(0, 0));
    CHECK_EQUAL(5, calc_subtract(5, 0));
    CHECK_EQUAL(-5, calc_subtract(0, 5));
}

/*============================================================================
 * Test Group: CalcMultiply - Tests for calc_multiply function
 *===========================================================================*/

TEST_GROUP(CalcMultiply)
{
};

TEST(CalcMultiply, PositiveNumbers)
{
    CHECK_EQUAL(6, calc_multiply(2, 3));
    CHECK_EQUAL(20000, calc_multiply(100, 200));
}

TEST(CalcMultiply, NegativeNumbers)
{
    CHECK_EQUAL(6, calc_multiply(-2, -3));
    CHECK_EQUAL(-6, calc_multiply(-2, 3));
    CHECK_EQUAL(-6, calc_multiply(2, -3));
}

TEST(CalcMultiply, Zero)
{
    CHECK_EQUAL(0, calc_multiply(0, 0));
    CHECK_EQUAL(0, calc_multiply(5, 0));
    CHECK_EQUAL(0, calc_multiply(0, 5));
}

/*============================================================================
 * Test Group: CalcDivide - Tests for calc_divide function
 *===========================================================================*/

TEST_GROUP(CalcDivide)
{
};

TEST(CalcDivide, PositiveNumbers)
{
    CHECK_EQUAL(2, calc_divide(6, 3));
    CHECK_EQUAL(5, calc_divide(100, 20));
}

TEST(CalcDivide, NegativeNumbers)
{
    CHECK_EQUAL(2, calc_divide(-6, -3));
    CHECK_EQUAL(-2, calc_divide(-6, 3));
    CHECK_EQUAL(-2, calc_divide(6, -3));
}

TEST(CalcDivide, IntegerDivision)
{
    // Integer division truncates toward zero
    CHECK_EQUAL(3, calc_divide(10, 3));
    CHECK_EQUAL(0, calc_divide(1, 2));
}

TEST(CalcDivide, DivideByZero)
{
    // Division by zero returns 0 (as defined in our SDK)
    CHECK_EQUAL(0, calc_divide(10, 0));
    CHECK_EQUAL(0, calc_divide(0, 0));
}
