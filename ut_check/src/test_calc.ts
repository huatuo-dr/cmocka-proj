/**
 * @file test_calc.ts
 * @brief Unit tests for calc module using Check framework
 *
 * Demonstrates Check features:
 * - Basic assert macros (ck_assert_int_eq, ck_assert_int_ne, etc.)
 * - Loop tests with # test-loop
 * - Suite and TCase organization
 *
 * This file uses checkmk syntax and will be converted to C code.
 */

#include <stdlib.h>
#include <stdint.h>
#include "calc.h"

# suite CalcSuite

/*============================================================================
 * TCase: AddTests - Basic addition tests
 *===========================================================================*/

# tcase AddTests

# test test_add_positive_numbers
    ck_assert_int_eq(calc_add(2, 3), 5);
    ck_assert_int_eq(calc_add(100, 200), 300);
    ck_assert_int_eq(calc_add(0, 0), 0);

# test test_add_negative_numbers
    ck_assert_int_eq(calc_add(-2, -3), -5);
    ck_assert_int_eq(calc_add(-100, -200), -300);

# test test_add_mixed_numbers
    ck_assert_int_eq(calc_add(-5, 3), -2);
    ck_assert_int_eq(calc_add(5, -3), 2);
    ck_assert_int_eq(calc_add(-10, 10), 0);

# test test_add_zero
    ck_assert_int_eq(calc_add(0, 0), 0);
    ck_assert_int_eq(calc_add(5, 0), 5);
    ck_assert_int_eq(calc_add(0, 5), 5);

/*============================================================================
 * TCase: SubtractTests - Basic subtraction tests
 *===========================================================================*/

# tcase SubtractTests

# test test_subtract_positive_numbers
    ck_assert_int_eq(calc_subtract(5, 3), 2);
    ck_assert_int_eq(calc_subtract(100, 30), 70);

# test test_subtract_negative_result
    ck_assert_int_eq(calc_subtract(3, 5), -2);
    ck_assert_int_lt(calc_subtract(10, 20), 0);

# test test_subtract_same_numbers
    ck_assert_int_eq(calc_subtract(5, 5), 0);
    ck_assert_int_eq(calc_subtract(100, 100), 0);

/*============================================================================
 * TCase: MultiplyTests - Basic multiplication tests
 *===========================================================================*/

# tcase MultiplyTests

# test test_multiply_positive_numbers
    ck_assert_int_eq(calc_multiply(2, 3), 6);
    ck_assert_int_eq(calc_multiply(10, 10), 100);

# test test_multiply_with_zero
    ck_assert_int_eq(calc_multiply(0, 100), 0);
    ck_assert_int_eq(calc_multiply(100, 0), 0);
    ck_assert_int_eq(calc_multiply(0, 0), 0);

# test test_multiply_negative_numbers
    ck_assert_int_gt(calc_multiply(-3, -4), 0);
    ck_assert_int_eq(calc_multiply(-3, -4), 12);
    ck_assert_int_lt(calc_multiply(3, -4), 0);
    ck_assert_int_eq(calc_multiply(3, -4), -12);

/*============================================================================
 * TCase: DivideTests - Basic division tests
 *===========================================================================*/

# tcase DivideTests

# test test_divide_exact
    ck_assert_int_eq(calc_divide(10, 2), 5);
    ck_assert_int_eq(calc_divide(100, 10), 10);

# test test_divide_truncation
    ck_assert_int_eq(calc_divide(10, 3), 3);
    ck_assert_int_eq(calc_divide(7, 2), 3);

# test test_divide_by_zero
    ck_assert_int_eq(calc_divide(10, 0), 0);
    ck_assert_int_eq(calc_divide(0, 0), 0);
    ck_assert_int_eq(calc_divide(-5, 0), 0);

# test test_divide_negative_numbers
    ck_assert_int_eq(calc_divide(-10, 2), -5);
    ck_assert_int_eq(calc_divide(10, -2), -5);
    ck_assert_int_eq(calc_divide(-10, -2), 5);

/*============================================================================
 * TCase: LoopTests - Demonstrates Check's loop test feature
 *===========================================================================*/

# tcase LoopTests

# test-loop(0, 10) test_add_loop
    ck_assert_int_eq(calc_add(_i, _i), _i * 2);

# test-loop(1, 6) test_multiply_loop
    ck_assert_int_eq(calc_multiply(_i, 1), _i);
    ck_assert_int_eq(calc_multiply(_i, 0), 0);

# test-loop(0, 5) test_subtract_loop
    ck_assert_int_eq(calc_subtract(_i, _i), 0);

/*============================================================================
 * TCase: EdgeCases - Boundary value tests
 *===========================================================================*/

# tcase EdgeCases

# test test_int32_boundaries
    ck_assert_int_eq(calc_add(INT32_MAX - 1, 1), INT32_MAX);
    ck_assert_int_eq(calc_add(INT32_MIN + 1, -1), INT32_MIN);

# test test_comparison_assertions
    ck_assert_int_lt(calc_subtract(3, 5), 0);
    ck_assert_int_le(calc_subtract(5, 5), 0);
    ck_assert_int_gt(calc_add(1, 1), 1);
    ck_assert_int_ge(calc_add(1, 1), 2);
    ck_assert_int_ne(calc_add(1, 1), 3);
