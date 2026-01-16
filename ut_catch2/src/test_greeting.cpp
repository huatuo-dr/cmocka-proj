/**
 * @file test_greeting.cpp
 * @brief Catch2 unit tests for greeting module using BDD style
 *
 * Demonstrates Catch2 BDD features:
 * - SCENARIO macro (alias for TEST_CASE with "Scenario: " prefix)
 * - GIVEN macro (alias for SECTION with "Given: " prefix)
 * - WHEN macro (alias for SECTION with "When: " prefix)
 * - THEN macro (alias for SECTION with "Then: " prefix)
 * - AND_GIVEN, AND_WHEN, AND_THEN for additional clauses
 *
 * BDD style makes tests read like specifications.
 */

#include <catch2/catch_test_macros.hpp>
#include <cstring>

// C header needs extern "C"
extern "C" {
#include "greeting.h"
}

/* ========== BDD Style Tests (SCENARIO/GIVEN/WHEN/THEN) ========== */

SCENARIO("User receives a hello greeting", "[greeting][bdd]") {

    GIVEN("A valid user name") {
        const char* name = "Alice";

        WHEN("The hello greeting is requested") {
            const char* result = say_hello(name);

            THEN("The greeting contains 'Hello'") {
                REQUIRE(result != nullptr);
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

SCENARIO("User receives a goodbye greeting", "[greeting][bdd]") {

    GIVEN("A valid user name") {
        const char* name = "Charlie";

        WHEN("The goodbye greeting is requested") {
            const char* result = say_goodbye(name);

            THEN("The greeting contains 'Goodbye'") {
                REQUIRE(result != nullptr);
                REQUIRE(std::strstr(result, "Goodbye") != nullptr);
            }

            THEN("The greeting contains the user's name") {
                REQUIRE(std::strstr(result, name) != nullptr);
            }
        }
    }
}

/* ========== BDD with AND clauses ========== */

SCENARIO("Greeting functions return consistent format", "[greeting][bdd][format]") {

    GIVEN("A user name") {
        const char* name = "David";

        AND_GIVEN("The greeting system is initialized") {
            // Additional setup if needed

            WHEN("Hello greeting is generated") {
                const char* hello_result = say_hello(name);

                THEN("The result is not null") {
                    REQUIRE(hello_result != nullptr);
                }

                AND_THEN("The result is not empty") {
                    REQUIRE(std::strlen(hello_result) > 0);
                }

                AND_THEN("The result contains the name") {
                    REQUIRE(std::strstr(hello_result, name) != nullptr);
                }
            }

            WHEN("Goodbye greeting is generated") {
                const char* goodbye_result = say_goodbye(name);

                THEN("The result is not null") {
                    REQUIRE(goodbye_result != nullptr);
                }

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

SCENARIO("Different greetings for the same user", "[greeting][bdd]") {

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

/* ========== Traditional TEST_CASE for comparison ========== */

TEST_CASE("Greeting hello returns expected format", "[greeting]") {
    const char* name = "TestUser";
    const char* result = say_hello(name);

    REQUIRE(result != nullptr);
    REQUIRE(std::strstr(result, "Hello") != nullptr);
    REQUIRE(std::strstr(result, name) != nullptr);
}

TEST_CASE("Greeting goodbye returns expected format", "[greeting]") {
    const char* name = "TestUser";
    const char* result = say_goodbye(name);

    REQUIRE(result != nullptr);
    REQUIRE(std::strstr(result, "Goodbye") != nullptr);
    REQUIRE(std::strstr(result, name) != nullptr);
}

/* ========== SECTION style for comparison ========== */

TEST_CASE("Greeting functions with SECTION style", "[greeting][section]") {
    const char* name = "Frank";

    SECTION("Hello greeting") {
        const char* result = say_hello(name);

        SECTION("Returns non-null") {
            REQUIRE(result != nullptr);
        }

        SECTION("Contains Hello keyword") {
            REQUIRE(std::strstr(result, "Hello") != nullptr);
        }

        SECTION("Contains user name") {
            REQUIRE(std::strstr(result, name) != nullptr);
        }
    }

    SECTION("Goodbye greeting") {
        const char* result = say_goodbye(name);

        SECTION("Returns non-null") {
            REQUIRE(result != nullptr);
        }

        SECTION("Contains Goodbye keyword") {
            REQUIRE(std::strstr(result, "Goodbye") != nullptr);
        }

        SECTION("Contains user name") {
            REQUIRE(std::strstr(result, name) != nullptr);
        }
    }
}
