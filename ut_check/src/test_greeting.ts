/**
 * @file test_greeting.ts
 * @brief Unit tests for greeting module using Check framework
 *
 * Demonstrates Check features:
 * - String assertions (ck_assert_str_eq, ck_assert_str_ne)
 * - Pointer assertions (ck_assert_ptr_nonnull, ck_assert_ptr_null)
 * - String comparison assertions
 *
 * This file uses checkmk syntax and will be converted to C code.
 */

#include <stdlib.h>
#include <string.h>
#include "greeting.h"

# suite GreetingSuite

/*============================================================================
 * TCase: HelloTests - Tests for say_hello function
 *===========================================================================*/

# tcase HelloTests

# test test_hello_basic
    const char *result = say_hello("World");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "Hello, World!");

# test test_hello_with_name
    const char *result = say_hello("Alice");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "Hello, Alice!");

# test test_hello_with_different_names
    const char *result1 = say_hello("Bob");
    ck_assert_str_eq(result1, "Hello, Bob!");

# test test_hello_empty_name
    const char *result = say_hello("");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "Hello, stranger!");

# test test_hello_returns_non_null
    ck_assert_ptr_nonnull(say_hello("Test"));
    ck_assert_ptr_nonnull(say_hello(""));
    ck_assert_ptr_nonnull(say_hello("A"));

/*============================================================================
 * TCase: GoodbyeTests - Tests for say_goodbye function
 *===========================================================================*/

# tcase GoodbyeTests

# test test_goodbye_basic
    const char *result = say_goodbye("World");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "Goodbye, World!");

# test test_goodbye_with_name
    const char *result = say_goodbye("Alice");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "Goodbye, Alice!");

# test test_goodbye_with_different_names
    const char *result1 = say_goodbye("Bob");
    ck_assert_str_eq(result1, "Goodbye, Bob!");

# test test_goodbye_empty_name
    const char *result = say_goodbye("");
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "Goodbye, stranger!");

/*============================================================================
 * TCase: StringComparisonTests - Demonstrates string comparison assertions
 *===========================================================================*/

# tcase StringComparisonTests

# test test_string_not_equal
    const char *hello = say_hello("Test");
    const char *goodbye = say_goodbye("Test");
    ck_assert_str_ne(hello, goodbye);

# test test_string_contains_name
    const char *result = say_hello("UniqueNameXYZ");
    ck_assert_ptr_nonnull(result);
    ck_assert_ptr_nonnull(strstr(result, "UniqueNameXYZ"));

# test test_string_starts_with
    const char *hello = say_hello("Test");
    const char *goodbye = say_goodbye("Test");
    ck_assert_int_eq(strncmp(hello, "Hello", 5), 0);
    ck_assert_int_eq(strncmp(goodbye, "Goodbye", 7), 0);

# test test_string_length
    const char *result = say_hello("A");
    size_t len = strlen(result);
    ck_assert_uint_eq(len, 9);

/*============================================================================
 * TCase: PointerTests - Demonstrates pointer assertions
 *===========================================================================*/

# tcase PointerTests

# test test_pointer_not_null
    const char *result = say_hello("Test");
    ck_assert_ptr_nonnull(result);

# test test_pointer_equality
    const char *result1 = say_hello("First");
    const char *ptr1 = result1;
    const char *result2 = say_hello("Second");
    const char *ptr2 = result2;
    ck_assert_ptr_eq(ptr1, ptr2);
    ck_assert_str_eq(result2, "Hello, Second!");
