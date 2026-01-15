/**
 * @file test_multi_calc.ts
 * @brief Unit tests for multi-calc module with Mock using Check framework
 *
 * Demonstrates Check features:
 * - Mock using --wrap linker option
 * - Checked fixtures (# main-pre for setup)
 * - Hybrid testing: mixing real and mock functions
 *
 * This file uses checkmk syntax and will be converted to C code.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "multi-calc.h"

/*============================================================================
 * External declarations for mock control
 * These are defined in mock_helpers.c
 *===========================================================================*/

extern void enable_all_mocks(void);
extern void disable_all_mocks(void);
extern void set_mock_calc_add(bool enable);
extern void set_mock_calc_subtract(bool enable);
extern void set_mock_calc_multiply(bool enable);
extern void set_mock_calc_divide(bool enable);

extern void push_mock_return(int value);
extern void clear_mock_returns(void);

# suite MultiCalcSuite

/*============================================================================
 * TCase: ExpressionMockTests - Tests for multi_calc_expression with mock
 * Formula: (a + b) * (c - d)
 *===========================================================================*/

# tcase ExpressionMockTests

# test test_expression_normal_mock
    enable_all_mocks();
    clear_mock_returns();
    push_mock_return(5);
    push_mock_return(6);
    push_mock_return(30);
    int result = multi_calc_expression(2, 3, 10, 4);
    ck_assert_int_eq(result, 30);

# test test_expression_subtract_zero_mock
    enable_all_mocks();
    clear_mock_returns();
    push_mock_return(10);
    push_mock_return(0);
    push_mock_return(0);
    int result = multi_calc_expression(5, 5, 7, 7);
    ck_assert_int_eq(result, 0);

# test test_expression_negative_mock
    enable_all_mocks();
    clear_mock_returns();
    push_mock_return(3);
    push_mock_return(-3);
    push_mock_return(-9);
    int result = multi_calc_expression(1, 2, 2, 5);
    ck_assert_int_eq(result, -9);

/*============================================================================
 * TCase: AverageMockTests - Tests for multi_calc_average with mock
 * Formula: (a + b + c) / 3
 *===========================================================================*/

# tcase AverageMockTests

# test test_average_normal_mock
    enable_all_mocks();
    clear_mock_returns();
    push_mock_return(30);
    push_mock_return(60);
    push_mock_return(20);
    int result = multi_calc_average(10, 20, 30);
    ck_assert_int_eq(result, 20);

# test test_average_truncation_mock
    enable_all_mocks();
    clear_mock_returns();
    push_mock_return(2);
    push_mock_return(3);
    push_mock_return(1);
    int result = multi_calc_average(1, 1, 1);
    ck_assert_int_eq(result, 1);

# test test_average_abnormal_mock
    enable_all_mocks();
    clear_mock_returns();
    push_mock_return(999);
    push_mock_return(1000);
    push_mock_return(333);
    int result = multi_calc_average(1, 2, 3);
    ck_assert_int_eq(result, 333);

/*============================================================================
 * TCase: RealFunctionTests - Tests using real implementations (no mock)
 *===========================================================================*/

# tcase RealFunctionTests

# test test_expression_real
    disable_all_mocks();
    int result = multi_calc_expression(2, 3, 10, 4);
    ck_assert_int_eq(result, 30);

# test test_expression_real_more
    disable_all_mocks();
    int result = multi_calc_expression(5, 5, 8, 3);
    ck_assert_int_eq(result, 50);

# test test_average_real
    disable_all_mocks();
    int result = multi_calc_average(10, 20, 30);
    ck_assert_int_eq(result, 20);

# test test_average_real_more
    disable_all_mocks();
    int result = multi_calc_average(7, 8, 9);
    ck_assert_int_eq(result, 8);

/*============================================================================
 * TCase: PartialMockTests - Tests with partial mocking
 *===========================================================================*/

# tcase PartialMockTests

# test test_partial_mock_multiply_only
    disable_all_mocks();
    set_mock_calc_multiply(true);
    clear_mock_returns();
    push_mock_return(999);
    int result = multi_calc_expression(2, 3, 10, 4);
    ck_assert_int_eq(result, 999);

# test test_partial_mock_divide_only
    disable_all_mocks();
    set_mock_calc_divide(true);
    clear_mock_returns();
    push_mock_return(100);
    int result = multi_calc_average(10, 20, 30);
    ck_assert_int_eq(result, 100);

/*============================================================================
 * TCase: CompareTests - Compare mock vs real results
 *===========================================================================*/

# tcase CompareTests

# test test_compare_mock_vs_real
    int mock_result, real_result;

    enable_all_mocks();
    clear_mock_returns();
    push_mock_return(100);
    push_mock_return(200);
    push_mock_return(66);
    mock_result = multi_calc_average(1, 2, 3);
    ck_assert_int_eq(mock_result, 66);

    disable_all_mocks();
    real_result = multi_calc_average(1, 2, 3);
    ck_assert_int_eq(real_result, 2);

    ck_assert_int_ne(mock_result, real_result);
