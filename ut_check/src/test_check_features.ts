/**
 * @file test_check_features.ts
 * @brief Demonstrates Check framework's unique features
 *
 * This file showcases features that are unique to Check framework:
 * - Signal tests: expect a test to raise a specific signal
 * - Exit tests: expect a test to exit with a specific code
 * - Timeout tests: tests that exceed time limits
 * - Loop tests: parameterized tests with loop variable
 *
 * Note: Signal and exit tests require fork mode (default on Linux).
 * Signal numbers: SIGSEGV=11, SIGABRT=6, SIGFPE=8
 *
 * This file uses checkmk syntax and will be converted to C code.
 */

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

# suite CheckFeaturesSuite

/*============================================================================
 * TCase: SignalTests - Tests that expect specific signals
 *
 * These tests demonstrate Check's ability to catch signals.
 * The test PASSES if the expected signal is raised.
 * Note: checkmk requires numeric signal values, not macros
 *===========================================================================*/

# tcase SignalTests

/* Test expects SIGSEGV (signal 11) - segmentation fault
 * This test PASSES because it correctly raises SIGSEGV */
#test-signal(11) test_null_pointer_dereference
    int *p = NULL;
    *p = 42;

/* Test expects SIGABRT (signal 6) - abort signal */
#test-signal(6) test_abort_signal
    abort();

/*============================================================================
 * TCase: ExitTests - Tests that expect specific exit codes
 *
 * These tests demonstrate Check's ability to verify exit codes.
 * The test PASSES if exit() is called with the expected code.
 *===========================================================================*/

# tcase ExitTests

/* Test expects exit code 0 (success) */
#test-exit(0) test_exit_success
    exit(EXIT_SUCCESS);

/* Test expects exit code 1 (failure) */
#test-exit(1) test_exit_failure
    exit(EXIT_FAILURE);

/* Test expects exit code 42 (custom code) */
#test-exit(42) test_exit_custom_code
    exit(42);

/*============================================================================
 * TCase: LoopTests - Parameterized tests using loop variable
 *
 * Loop tests run the same test multiple times with different values.
 * The loop variable '_i' is available inside the test.
 *===========================================================================*/

# tcase LoopTests

/* Loop from 0 to 9 (10 iterations) */
#test-loop(0, 10) test_loop_basic
    ck_assert_int_ge(_i, 0);
    ck_assert_int_lt(_i, 10);

/* Loop from 1 to 5 (5 iterations) */
#test-loop(1, 6) test_loop_factorial_base
    ck_assert_int_eq(_i * 1, _i);

/* Loop to test array bounds */
#test-loop(0, 50) test_loop_array_access
    int arr[50];
    memset(arr, 0, sizeof(arr));
    arr[_i] = _i * 2;
    ck_assert_int_eq(arr[_i], _i * 2);

/*============================================================================
 * TCase: TimeoutTests - Tests with timeout behavior
 *===========================================================================*/

# tcase TimeoutTests

/* A quick test that should complete well within any timeout */
# test test_quick_operation
    int sum = 0;
    int i;
    for (i = 0; i < 1000; i++) {
        sum += i;
    }
    ck_assert_int_eq(sum, 499500);

/* A slightly longer test but still fast */
# test test_moderate_operation
    int product = 1;
    int i;
    for (i = 1; i <= 10; i++) {
        product *= i;
    }
    ck_assert_int_eq(product, 3628800);

/*============================================================================
 * TCase: MarkPointTests - Tests demonstrating mark_point()
 *===========================================================================*/

# tcase MarkPointTests

# test test_with_mark_points
    int arr[5] = {1, 2, 3, 4, 5};

    mark_point();
    ck_assert_int_eq(arr[0], 1);

    mark_point();
    ck_assert_int_eq(arr[1], 2);

    mark_point();
    ck_assert_int_eq(arr[2], 3);

    mark_point();
    ck_assert_int_eq(arr[3], 4);

    mark_point();
    ck_assert_int_eq(arr[4], 5);

/*============================================================================
 * TCase: AssertMessageTests - Tests with custom failure messages
 *===========================================================================*/

# tcase AssertMessageTests

# test test_assert_with_message
    int expected = 42;
    int actual = 42;

    ck_assert_msg(actual == expected,
                  "Expected %d but got %d", expected, actual);

# test test_multiple_conditions
    int a = 10, b = 20, c = 30;

    ck_assert_msg(a < b, "a should be less than b");
    ck_assert_msg(b < c, "b should be less than c");
    ck_assert_msg(a < c, "a should be less than c (transitive)");
