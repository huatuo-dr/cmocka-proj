/**
 * @file test_multi_calc.cpp
 * @brief Unit tests for multi-calc module with CppUTest Mock
 *
 * Demonstrates CppUTest Mock features:
 * - mock().expectOneCall() to set expectations
 * - mock().actualCall() in wrapped functions
 * - Parameter verification with withParameter()
 * - Return value setting with andReturnValue()
 * - Call count verification with checkExpectations()
 * - Hybrid testing: mixing real and mock functions
 */

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
    #include "multi-calc.h"
    #include "calc.h"
}

/*============================================================================
 * Real Function Declarations
 *
 * When using --wrap, the linker provides __real_xxx to access original functions
 *===========================================================================*/

extern "C" {
    extern int __real_calc_add(int a, int b);
    extern int __real_calc_subtract(int a, int b);
    extern int __real_calc_multiply(int a, int b);
    extern int __real_calc_divide(int a, int b);
}

/*============================================================================
 * Mock Control Flags
 *
 * These flags control whether to use mock or real implementation
 *===========================================================================*/

static bool mock_calc_add_enabled = true;
static bool mock_calc_subtract_enabled = true;
static bool mock_calc_multiply_enabled = true;
static bool mock_calc_divide_enabled = true;

/*============================================================================
 * Mock Functions - Using __wrap_ prefix
 *
 * These functions use CppUTest Mock API and can switch between mock and real
 *===========================================================================*/

extern "C" {

int __wrap_calc_add(int a, int b)
{
    if (mock_calc_add_enabled) {
        return mock().actualCall("calc_add")
                     .withParameter("a", a)
                     .withParameter("b", b)
                     .returnIntValueOrDefault(0);
    } else {
        return __real_calc_add(a, b);
    }
}

int __wrap_calc_subtract(int a, int b)
{
    if (mock_calc_subtract_enabled) {
        return mock().actualCall("calc_subtract")
                     .withParameter("a", a)
                     .withParameter("b", b)
                     .returnIntValueOrDefault(0);
    } else {
        return __real_calc_subtract(a, b);
    }
}

int __wrap_calc_multiply(int a, int b)
{
    if (mock_calc_multiply_enabled) {
        return mock().actualCall("calc_multiply")
                     .withParameter("a", a)
                     .withParameter("b", b)
                     .returnIntValueOrDefault(0);
    } else {
        return __real_calc_multiply(a, b);
    }
}

int __wrap_calc_divide(int a, int b)
{
    if (mock_calc_divide_enabled) {
        return mock().actualCall("calc_divide")
                     .withParameter("a", a)
                     .withParameter("b", b)
                     .returnIntValueOrDefault(0);
    } else {
        return __real_calc_divide(a, b);
    }
}

} // extern "C"

/*============================================================================
 * Helper Functions - Enable/Disable all mocks
 *===========================================================================*/

static void enable_all_mocks()
{
    mock_calc_add_enabled = true;
    mock_calc_subtract_enabled = true;
    mock_calc_multiply_enabled = true;
    mock_calc_divide_enabled = true;
}

static void disable_all_mocks()
{
    mock_calc_add_enabled = false;
    mock_calc_subtract_enabled = false;
    mock_calc_multiply_enabled = false;
    mock_calc_divide_enabled = false;
}

/*============================================================================
 * Test Group: MultiCalcMock - Pure Mock Tests
 *===========================================================================*/

TEST_GROUP(MultiCalcMock)
{
    void setup()
    {
        enable_all_mocks();
    }

    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

/**
 * Test multi_calc_expression with all functions mocked
 * Expression: (a + b) * (c - d) = (2 + 3) * (10 - 4) = 5 * 6 = 30
 */
TEST(MultiCalcMock, ExpressionWithFullMock)
{
    // Set expectations in order of function calls
    mock().expectOneCall("calc_add")
          .withParameter("a", 2)
          .withParameter("b", 3)
          .andReturnValue(5);

    mock().expectOneCall("calc_subtract")
          .withParameter("a", 10)
          .withParameter("b", 4)
          .andReturnValue(6);

    mock().expectOneCall("calc_multiply")
          .withParameter("a", 5)
          .withParameter("b", 6)
          .andReturnValue(30);

    int result = multi_calc_expression(2, 3, 10, 4);

    CHECK_EQUAL(30, result);
}

/**
 * Test multi_calc_expression with mocked error simulation
 * Simulate calc_add returning an error value
 */
TEST(MultiCalcMock, ExpressionWithMockedError)
{
    // Mock calc_add to return unexpected value
    mock().expectOneCall("calc_add")
          .withParameter("a", 1)
          .withParameter("b", 2)
          .andReturnValue(999);  // Mocked error value

    mock().expectOneCall("calc_subtract")
          .withParameter("a", 3)
          .withParameter("b", 4)
          .andReturnValue(-1);

    mock().expectOneCall("calc_multiply")
          .withParameter("a", 999)
          .withParameter("b", -1)
          .andReturnValue(-999);

    int result = multi_calc_expression(1, 2, 3, 4);

    CHECK_EQUAL(-999, result);
}

/**
 * Test multi_calc_average with all functions mocked
 * Average: (a + b + c) / 3 = (10 + 20 + 30) / 3 = 60 / 3 = 20
 */
TEST(MultiCalcMock, AverageWithFullMock)
{
    // First add: a + b
    mock().expectOneCall("calc_add")
          .withParameter("a", 10)
          .withParameter("b", 20)
          .andReturnValue(30);

    // Second add: (a + b) + c
    mock().expectOneCall("calc_add")
          .withParameter("a", 30)
          .withParameter("b", 30)
          .andReturnValue(60);

    // Divide by 3
    mock().expectOneCall("calc_divide")
          .withParameter("a", 60)
          .withParameter("b", 3)
          .andReturnValue(20);

    int result = multi_calc_average(10, 20, 30);

    CHECK_EQUAL(20, result);
}

/*============================================================================
 * Test Group: MultiCalcReal - Tests using real functions
 *===========================================================================*/

TEST_GROUP(MultiCalcReal)
{
    void setup()
    {
        disable_all_mocks();
    }

    void teardown()
    {
        mock().clear();
    }
};

/**
 * Test multi_calc_expression with real functions
 * Expression: (2 + 3) * (10 - 4) = 5 * 6 = 30
 */
TEST(MultiCalcReal, ExpressionWithRealFunctions)
{
    int result = multi_calc_expression(2, 3, 10, 4);
    CHECK_EQUAL(30, result);
}

/**
 * Test multi_calc_average with real functions
 * Average: (1 + 2 + 3) / 3 = 6 / 3 = 2
 */
TEST(MultiCalcReal, AverageWithRealFunctions)
{
    int result = multi_calc_average(1, 2, 3);
    CHECK_EQUAL(2, result);
}

/**
 * Test multi_calc_average with larger numbers
 * Average: (100 + 200 + 300) / 3 = 600 / 3 = 200
 */
TEST(MultiCalcReal, AverageWithLargerNumbers)
{
    int result = multi_calc_average(100, 200, 300);
    CHECK_EQUAL(200, result);
}

/*============================================================================
 * Test Group: MultiCalcHybrid - Partial mock tests
 *===========================================================================*/

TEST_GROUP(MultiCalcHybrid)
{
    void setup()
    {
        // Start with all mocks disabled
        disable_all_mocks();
    }

    void teardown()
    {
        mock().checkExpectations();
        mock().clear();
    }
};

/**
 * Test with only multiply mocked (add and subtract are real)
 */
TEST(MultiCalcHybrid, ExpressionWithPartialMock)
{
    // Enable only multiply mock
    mock_calc_multiply_enabled = true;

    // Real: calc_add(2, 3) = 5
    // Real: calc_subtract(10, 4) = 6
    // Mock: calc_multiply(5, 6) = 999
    mock().expectOneCall("calc_multiply")
          .withParameter("a", 5)
          .withParameter("b", 6)
          .andReturnValue(999);

    int result = multi_calc_expression(2, 3, 10, 4);

    CHECK_EQUAL(999, result);
}

/**
 * Compare mock vs real results
 */
TEST(MultiCalcHybrid, CompareMockVsReal)
{
    // First, use real functions
    disable_all_mocks();
    int real_result = multi_calc_average(10, 20, 30);
    CHECK_EQUAL(20, real_result);

    // Now use mock
    enable_all_mocks();

    mock().expectOneCall("calc_add")
          .withParameter("a", 10)
          .withParameter("b", 20)
          .andReturnValue(30);
    mock().expectOneCall("calc_add")
          .withParameter("a", 30)
          .withParameter("b", 30)
          .andReturnValue(60);
    mock().expectOneCall("calc_divide")
          .withParameter("a", 60)
          .withParameter("b", 3)
          .andReturnValue(100);  // Mock returns different value

    int mock_result = multi_calc_average(10, 20, 30);
    CHECK_EQUAL(100, mock_result);

    // Verify they are different
    CHECK(real_result != mock_result);
}
