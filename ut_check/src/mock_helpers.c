/**
 * @file mock_helpers.c
 * @brief Mock helper functions for Check unit tests
 *
 * Provides __wrap_xxx functions and mock control utilities.
 * Used with --wrap linker option to intercept calc_* function calls.
 */

#include <stdbool.h>
#include <string.h>

/*============================================================================
 * Real Function Declarations
 * When using --wrap, the linker provides __real_xxx to access original functions
 *===========================================================================*/

extern int __real_calc_add(int a, int b);
extern int __real_calc_subtract(int a, int b);
extern int __real_calc_multiply(int a, int b);
extern int __real_calc_divide(int a, int b);

/*============================================================================
 * Mock Control Flags
 *===========================================================================*/

static bool mock_calc_add_flag = true;
static bool mock_calc_subtract_flag = true;
static bool mock_calc_multiply_flag = true;
static bool mock_calc_divide_flag = true;

/*============================================================================
 * Mock Return Value Queue
 * Simple FIFO queue for mock return values
 *===========================================================================*/

#define MAX_MOCK_RETURNS 64
static int mock_return_queue[MAX_MOCK_RETURNS];
static int mock_return_head = 0;
static int mock_return_tail = 0;
static int mock_return_count = 0;

/**
 * Push a return value to the mock queue
 */
void push_mock_return(int value) {
    if (mock_return_count < MAX_MOCK_RETURNS) {
        mock_return_queue[mock_return_tail] = value;
        mock_return_tail = (mock_return_tail + 1) % MAX_MOCK_RETURNS;
        mock_return_count++;
    }
}

/**
 * Pop a return value from the mock queue
 */
static int pop_mock_return(void) {
    if (mock_return_count > 0) {
        int value = mock_return_queue[mock_return_head];
        mock_return_head = (mock_return_head + 1) % MAX_MOCK_RETURNS;
        mock_return_count--;
        return value;
    }
    return 0;  // Default return value if queue is empty
}

/**
 * Clear the mock return queue
 */
void clear_mock_returns(void) {
    mock_return_head = 0;
    mock_return_tail = 0;
    mock_return_count = 0;
}

/*============================================================================
 * Mock Control Functions
 *===========================================================================*/

void enable_all_mocks(void) {
    mock_calc_add_flag = true;
    mock_calc_subtract_flag = true;
    mock_calc_multiply_flag = true;
    mock_calc_divide_flag = true;
}

void disable_all_mocks(void) {
    mock_calc_add_flag = false;
    mock_calc_subtract_flag = false;
    mock_calc_multiply_flag = false;
    mock_calc_divide_flag = false;
}

void set_mock_calc_add(bool enable) {
    mock_calc_add_flag = enable;
}

void set_mock_calc_subtract(bool enable) {
    mock_calc_subtract_flag = enable;
}

void set_mock_calc_multiply(bool enable) {
    mock_calc_multiply_flag = enable;
}

void set_mock_calc_divide(bool enable) {
    mock_calc_divide_flag = enable;
}

/*============================================================================
 * Wrapped Functions
 * These intercept calls to calc_* functions when linked with --wrap
 *===========================================================================*/

int __wrap_calc_add(int a, int b) {
    if (mock_calc_add_flag) {
        return pop_mock_return();
    } else {
        return __real_calc_add(a, b);
    }
}

int __wrap_calc_subtract(int a, int b) {
    if (mock_calc_subtract_flag) {
        return pop_mock_return();
    } else {
        return __real_calc_subtract(a, b);
    }
}

int __wrap_calc_multiply(int a, int b) {
    if (mock_calc_multiply_flag) {
        return pop_mock_return();
    } else {
        return __real_calc_multiply(a, b);
    }
}

int __wrap_calc_divide(int a, int b) {
    if (mock_calc_divide_flag) {
        return pop_mock_return();
    } else {
        return __real_calc_divide(a, b);
    }
}
