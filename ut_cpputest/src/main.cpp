/**
 * @file main.cpp
 * @brief CppUTest main entry point
 *
 * This file provides the main function for running CppUTest tests.
 * CppUTest requires an explicit main function unlike some other frameworks.
 */

#include "CppUTest/CommandLineTestRunner.h"

int main(int argc, char** argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
