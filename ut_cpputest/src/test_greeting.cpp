/**
 * @file test_greeting.cpp
 * @brief Unit tests for greeting module using CppUTest
 *
 * Demonstrates CppUTest features:
 * - String comparison with STRCMP_EQUAL
 * - Pointer checks with CHECK and POINTERS_EQUAL
 * - Setup/teardown with resource management
 */

#include "CppUTest/TestHarness.h"

extern "C" {
    #include "greeting.h"
}

/*============================================================================
 * Test Group: GreetingHello - Tests for greeting_hello function
 *===========================================================================*/

TEST_GROUP(GreetingHello)
{
    const char* result;

    void setup()
    {
        result = nullptr;
    }

    void teardown()
    {
        result = nullptr;
    }
};

TEST(GreetingHello, BasicName)
{
    result = say_hello("World");
    CHECK(result != nullptr);
    STRCMP_EQUAL("Hello, World!", result);
}

TEST(GreetingHello, DifferentNames)
{
    result = say_hello("Alice");
    STRCMP_EQUAL("Hello, Alice!", result);

    result = say_hello("Bob");
    STRCMP_EQUAL("Hello, Bob!", result);

    result = say_hello("CppUTest");
    STRCMP_EQUAL("Hello, CppUTest!", result);
}

TEST(GreetingHello, EmptyName)
{
    result = say_hello("");
    CHECK(result != nullptr);
    // SDK returns "stranger" for empty name
    STRCMP_EQUAL("Hello, stranger!", result);
}

TEST(GreetingHello, NullName)
{
    result = say_hello(nullptr);
    // SDK returns "stranger" for null input
    CHECK(result != nullptr);
    STRCMP_EQUAL("Hello, stranger!", result);
}

/*============================================================================
 * Test Group: GreetingGoodbye - Tests for greeting_goodbye function
 *===========================================================================*/

TEST_GROUP(GreetingGoodbye)
{
    const char* result;

    void setup()
    {
        result = nullptr;
    }

    void teardown()
    {
        result = nullptr;
    }
};

TEST(GreetingGoodbye, BasicName)
{
    result = say_goodbye("World");
    CHECK(result != nullptr);
    STRCMP_EQUAL("Goodbye, World!", result);
}

TEST(GreetingGoodbye, DifferentNames)
{
    result = say_goodbye("Alice");
    STRCMP_EQUAL("Goodbye, Alice!", result);

    result = say_goodbye("Bob");
    STRCMP_EQUAL("Goodbye, Bob!", result);
}

TEST(GreetingGoodbye, EmptyName)
{
    result = say_goodbye("");
    CHECK(result != nullptr);
    // SDK returns "stranger" for empty name
    STRCMP_EQUAL("Goodbye, stranger!", result);
}

TEST(GreetingGoodbye, NullName)
{
    result = say_goodbye(nullptr);
    // SDK returns "stranger" for null input
    CHECK(result != nullptr);
    STRCMP_EQUAL("Goodbye, stranger!", result);
}
