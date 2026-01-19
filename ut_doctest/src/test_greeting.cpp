/**
 * @file test_greeting.cpp
 * @brief doctest unit tests for greeting module using BDD style
 *
 * Demonstrates doctest BDD features:
 * - SCENARIO macro (maps to TEST_CASE with "Scenario: " prefix)
 * - GIVEN macro (maps to SUBCASE with "given: " prefix)
 * - WHEN macro (maps to SUBCASE with "when: " prefix)
 * - THEN macro (maps to SUBCASE with "then: " prefix)
 * - AND_WHEN, AND_THEN for additional clauses
 *
 * BDD style makes tests read like specifications.
 */

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>
#include <cstring>

// C header needs extern "C"
extern "C" {
#include "greeting.h"
}

/* ========== BDD Style Tests (SCENARIO/GIVEN/WHEN/THEN) ========== */

SCENARIO("User receives a hello greeting") {

    GIVEN("A valid user name") {
        const char* name = "Alice";

        WHEN("The hello greeting is requested") {
            const char* result = say_hello(name);

            THEN("The greeting is not null") {
                REQUIRE(result != nullptr);
            }

            THEN("The greeting contains 'Hello'") {
                REQUIRE(std::strstr(result, "Hello") != nullptr);
            }

            THEN("The greeting contains the user's name") {
                REQUIRE(std::strstr(result, name) != nullptr);
            }
        }
    }

    GIVEN("Another user name") {
        const char* name = "Bob";

        WHEN("The hello greeting is requested") {
            const char* result = say_hello(name);

            THEN("The greeting is personalized for that user") {
                REQUIRE(result != nullptr);
                REQUIRE(std::strstr(result, "Bob") != nullptr);
            }
        }
    }
}

SCENARIO("User receives a goodbye greeting") {

    GIVEN("A valid user name") {
        const char* name = "Charlie";

        WHEN("The goodbye greeting is requested") {
            const char* result = say_goodbye(name);

            THEN("The greeting is not null") {
                REQUIRE(result != nullptr);
            }

            THEN("The greeting contains 'Goodbye'") {
                REQUIRE(std::strstr(result, "Goodbye") != nullptr);
            }

            THEN("The greeting contains the user's name") {
                REQUIRE(std::strstr(result, name) != nullptr);
            }
        }
    }
}

/* ========== BDD with AND clauses ========== */

SCENARIO("Greeting functions return consistent format") {

    GIVEN("A user name") {
        const char* name = "David";

        WHEN("Hello greeting is generated") {
            const char* hello_result = say_hello(name);

            THEN("The result is not null") {
                REQUIRE(hello_result != nullptr);

                AND_THEN("The result is not empty") {
                    REQUIRE(std::strlen(hello_result) > 0);
                }

                AND_THEN("The result contains the name") {
                    REQUIRE(std::strstr(hello_result, name) != nullptr);
                }
            }
        }

        AND_WHEN("Goodbye greeting is generated") {
            const char* goodbye_result = say_goodbye(name);

            THEN("The result is not null") {
                REQUIRE(goodbye_result != nullptr);

                AND_THEN("The result is not empty") {
                    REQUIRE(std::strlen(goodbye_result) > 0);
                }

                AND_THEN("The result contains the name") {
                    REQUIRE(std::strstr(goodbye_result, name) != nullptr);
                }
            }
        }
    }
}

/* ========== BDD with multiple WHEN clauses ========== */

SCENARIO("Different greetings for the same user") {

    GIVEN("A user named Eve") {
        const char* name = "Eve";

        WHEN("She receives a hello greeting") {
            const char* result = say_hello(name);

            THEN("It should be a welcoming message") {
                REQUIRE(std::strstr(result, "Hello") != nullptr);
            }
        }

        WHEN("She receives a goodbye greeting") {
            const char* result = say_goodbye(name);

            THEN("It should be a farewell message") {
                REQUIRE(std::strstr(result, "Goodbye") != nullptr);
            }
        }
    }
}

/* ========== SCENARIO_TEMPLATE for type parameterization ========== */

// Note: SCENARIO_TEMPLATE is available for parameterized BDD tests
// This example shows regular SCENARIO with different test data

SCENARIO("Greeting with various name lengths") {

    GIVEN("A short name") {
        const char* name = "Al";

        WHEN("Hello is called") {
            const char* result = say_hello(name);

            THEN("The greeting should include the short name") {
                REQUIRE(std::strstr(result, name) != nullptr);
            }
        }
    }

    GIVEN("A long name") {
        const char* name = "Christopher";

        WHEN("Hello is called") {
            const char* result = say_hello(name);

            THEN("The greeting should include the long name") {
                REQUIRE(std::strstr(result, name) != nullptr);
            }
        }
    }
}

/* ========== Traditional TEST_CASE for comparison ========== */

TEST_CASE("Greeting hello returns expected format") {
    const char* name = "TestUser";
    const char* result = say_hello(name);

    REQUIRE(result != nullptr);
    CHECK(std::strstr(result, "Hello") != nullptr);
    CHECK(std::strstr(result, name) != nullptr);
}

TEST_CASE("Greeting goodbye returns expected format") {
    const char* name = "TestUser";
    const char* result = say_goodbye(name);

    REQUIRE(result != nullptr);
    CHECK(std::strstr(result, "Goodbye") != nullptr);
    CHECK(std::strstr(result, name) != nullptr);
}

/* ========== SUBCASE style for comparison with BDD ========== */

TEST_CASE("Greeting functions with SUBCASE style") {
    const char* name = "Frank";

    SUBCASE("Hello greeting") {
        const char* result = say_hello(name);

        SUBCASE("Returns non-null") {
            REQUIRE(result != nullptr);
        }

        SUBCASE("Contains Hello keyword") {
            REQUIRE(std::strstr(result, "Hello") != nullptr);
        }

        SUBCASE("Contains user name") {
            REQUIRE(std::strstr(result, name) != nullptr);
        }
    }

    SUBCASE("Goodbye greeting") {
        const char* result = say_goodbye(name);

        SUBCASE("Returns non-null") {
            REQUIRE(result != nullptr);
        }

        SUBCASE("Contains Goodbye keyword") {
            REQUIRE(std::strstr(result, "Goodbye") != nullptr);
        }

        SUBCASE("Contains user name") {
            REQUIRE(std::strstr(result, name) != nullptr);
        }
    }
}
