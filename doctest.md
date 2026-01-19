# doctest 单元测试框架

仓库：https://github.com/doctest/doctest.git

## 📖 介绍

doctest 是一个轻量级的 C++ 单元测试框架，主要特点：

- **单头文件** - 只需 `#include <doctest/doctest.h>`，无需链接库
- **极快编译** - 引入头文件仅增加约 25ms 编译时间
- **SUBCASE 机制** - 类似 Catch2 的 SECTION，灵活的 setup/teardown
- **BDD 支持** - 内置 `SCENARIO`/`GIVEN`/`WHEN`/`THEN` 语法
- **Test Suites** - 测试用例分组功能
- **Decorators** - 装饰器支持（skip、timeout、description 等）
- **可嵌入生产代码** - 通过 `DOCTEST_CONFIG_DISABLE` 完全移除测试代码
- **线程安全** - 断言可在多线程中使用
- **无内置 Mock** - 需配合 `--wrap` 链接选项或其他 Mock 库

## 🔧 安装

### 全局安装

```shell
sudo apt update
sudo apt install libdoctest-dev
```

### 独立安装

执行命令：

```shell
git clone https://github.com/doctest/doctest.git
cd doctest
git checkout v2.4.12
cmake -B build -DCMAKE_INSTALL_PREFIX=build/doctest-install
cmake --build build --target install
```

编译完成后，在 `build/doctest-install` 目录中生成：

```
doctest-install/
├── include/doctest/    # 头文件
│   └── doctest.h       # 主头文件（单头文件）
└── lib/cmake/doctest/  # CMake 配置文件（无库文件）
```

## 🧪 基本用法

### 最简单的测试用例

```cpp
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

TEST_CASE("加法测试") {
    CHECK(1 + 1 == 2);
    REQUIRE(2 + 2 == 4);  // REQUIRE fails and stops, CHECK continues
}
```

### 常用断言宏

| 宏 | 用途 |
|----|------|
| `REQUIRE(expr)` | 断言，失败则终止当前测试 |
| `CHECK(expr)` | 断言，失败后继续执行 |
| `REQUIRE_FALSE(expr)` | 断言表达式为 false |
| `CHECK_EQ(a, b)` | 断言相等 |
| `CHECK_NE(a, b)` | 断言不相等 |
| `CHECK_GT(a, b)` | 断言大于 |
| `CHECK_LT(a, b)` | 断言小于 |
| `CHECK_THROWS(expr)` | 断言抛出异常 |
| `CHECK_THROWS_AS(expr, type)` | 断言抛出指定类型异常 |
| `CHECK_NOTHROW(expr)` | 断言不抛异常 |

## 📝 SUBCASE 语法

SUBCASE 是 doctest 的特色功能（与 Catch2 的 SECTION 相同），用于在测试用例中共享 setup 代码。每个 SUBCASE 独立运行，TEST_CASE 中 SUBCASE 之前的代码会在每个 SUBCASE 执行前重新运行。

### 基本 SUBCASE

```cpp
TEST_CASE("Calculator operations with shared setup") {
    // This setup runs BEFORE EACH subcase
    int operand_a = 10;
    int operand_b = 5;

    SUBCASE("Addition") {
        REQUIRE(calc_add(operand_a, operand_b) == 15);
    }

    SUBCASE("Subtraction") {
        REQUIRE(calc_subtract(operand_a, operand_b) == 5);
    }

    SUBCASE("Multiplication") {
        REQUIRE(calc_multiply(operand_a, operand_b) == 50);
    }
}
```

**执行流程**：
1. 初始化 `operand_a=10, operand_b=5` → 执行 "Addition" SUBCASE
2. 初始化 `operand_a=10, operand_b=5` → 执行 "Subtraction" SUBCASE
3. 初始化 `operand_a=10, operand_b=5` → 执行 "Multiplication" SUBCASE

### 嵌套 SUBCASE

```cpp
TEST_CASE("Nested subcases for complex scenarios") {
    int base = 100;

    SUBCASE("With positive operand") {
        int operand = 10;

        SUBCASE("Addition increases value") {
            REQUIRE(calc_add(base, operand) == 110);
        }

        SUBCASE("Subtraction decreases value") {
            REQUIRE(calc_subtract(base, operand) == 90);
        }
    }

    SUBCASE("With negative operand") {
        int operand = -10;

        SUBCASE("Addition decreases value") {
            REQUIRE(calc_add(base, operand) == 90);
        }
    }
}
```

**执行流程**（深度优先）：
1. `base=100` → `operand=10` → "Addition increases value"
2. `base=100` → `operand=10` → "Subtraction decreases value"
3. `base=100` → `operand=-10` → "Addition decreases value"

## 🎯 BDD 语法

doctest 提供 BDD (Behavior-Driven Development) 风格的宏，使测试读起来像规格说明。

### BDD 宏对照表

| BDD 宏 | 等效于 | 前缀 |
|--------|--------|------|
| `SCENARIO(name)` | `TEST_CASE("Scenario: " + name)` | "Scenario: " |
| `GIVEN(desc)` | `SUBCASE("given: " + desc)` | "given: " |
| `WHEN(desc)` | `SUBCASE("when: " + desc)` | "when: " |
| `THEN(desc)` | `SUBCASE("then: " + desc)` | "then: " |
| `AND_WHEN(desc)` | `SUBCASE("and when: " + desc)` | "and when: " |
| `AND_THEN(desc)` | `SUBCASE("and then: " + desc)` | "and then: " |

### BDD 示例

```cpp
SCENARIO("User receives a hello greeting") {

    GIVEN("A valid user name") {
        const char* name = "Alice";

        WHEN("The hello greeting is requested") {
            const char* result = say_hello(name);

            THEN("The greeting contains 'Hello'") {
                REQUIRE(std::strstr(result, "Hello") != nullptr);
            }

            AND_THEN("The greeting contains the user's name") {
                REQUIRE(std::strstr(result, name) != nullptr);
            }
        }
    }
}
```

### 多个 WHEN 分支

```cpp
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
```

## 📦 TEST_SUITE 分组

TEST_SUITE 用于将相关的测试用例分组：

```cpp
TEST_SUITE("Multiplication Tests") {
    TEST_CASE("Basic multiplication") {
        CHECK(calc_multiply(2, 3) == 6);
    }

    TEST_CASE("Multiplication with negative numbers") {
        CHECK(calc_multiply(-2, 3) == -6);
    }
}

TEST_SUITE("Division Tests") {
    TEST_CASE("Basic division") {
        CHECK(calc_divide(10, 2) == 5);
    }
}
```

可通过命令行筛选特定 suite 的测试：
```shell
./test_runner --test-suite="Multiplication*"
```

## 🏷️ Decorators 装饰器

装饰器用于为测试用例添加额外属性：

```cpp
TEST_CASE("Test with description"
          * doctest::description("This is a description")) {
    CHECK(1 == 1);
}

TEST_CASE("Test with timeout"
          * doctest::timeout(1.0)) {
    // Must complete within 1 second
}

TEST_CASE("Test that may fail"
          * doctest::may_fail()) {
    // Won't fail the test run even if it fails
}

TEST_CASE("Skipped test"
          * doctest::skip(true)) {
    // This test is skipped
}
```

### 支持的装饰器

| 装饰器 | 用途 |
|--------|------|
| `skip(bool)` | 跳过测试（除非使用 `--no-skip`） |
| `may_fail(bool)` | 允许失败但仍报告 |
| `should_fail(bool)` | 期望失败，通过则报错 |
| `expected_failures(int)` | 期望的失败断言数量 |
| `timeout(double)` | 超时限制（秒） |
| `description("text")` | 测试描述 |

### TEST_SUITE 继承装饰器

```cpp
TEST_SUITE("Edge Cases" * doctest::description("Boundary tests")) {
    TEST_CASE("Zero operations") {
        // Inherits description from suite
    }
}
```

## 🔄 Mock 实现

doctest 不内置 Mock 功能，本项目使用链接器 `--wrap` 选项实现 Mock：

### 原理

```
链接时: -Wl,--wrap=func_name
  - 调用 func_name → 实际调用 __wrap_func_name
  - 调用 __real_func_name → 实际调用原始 func_name
```

### Mock 实现示例

```cpp
// Mock control flags
static bool mock_calc_add_enabled = false;
static int mock_calc_add_return = 0;

// Real function declaration
extern "C" {
    extern int __real_calc_add(int a, int b);
}

// Wrap function
extern "C" {
    int __wrap_calc_add(int a, int b) {
        if (mock_calc_add_enabled) {
            return mock_calc_add_return;
        }
        return __real_calc_add(a, b);
    }
}

// Test
TEST_CASE("Expression with mocked calc_add") {
    mock_calc_add_enabled = true;
    mock_calc_add_return = 100;

    int result = multi_calc_expression(1, 2, 3, 4);

    CHECK(result == /* expected based on mock */);
}
```

## 📊 报告输出

### 命令行选项

```shell
# 终端输出 (默认)
./test_runner

# JUnit XML 格式
./test_runner --reporters=junit --out=result.xml

# 列出可用的 reporter
./test_runner --list-reporters

# 跳过标记为 skip 的测试
./test_runner --no-skip
```

### 支持的 Reporter

| Reporter | 说明 |
|----------|------|
| `console` | 默认终端输出 |
| `junit` | JUnit XML 格式 |
| `xml` | doctest 原生 XML |

## 🚀 编译和运行

### 编译命令

```shell
# doctest 是头文件库，无需链接
g++ -std=c++11 test.cpp \
    -Iut_doctest/doctest-install/include \
    -o test_runner
```

### Make 命令

```shell
# 运行测试并生成报告
make ut_doctest

# 仅构建
make ut_doctest_build

# 运行测试（终端输出）
make ut_doctest_run

# 生成 HTML 报告
make ut_doctest_report

# 运行覆盖率测试
make ut_doctest_cov
```

## 📁 目录结构

```
ut_doctest/
├── doctest-install/          # doctest 安装目录（可选，或使用 3rdparty）
│   └── include/doctest/      # 头文件
├── src/                      # 测试源码
│   ├── test_calc.cpp         # calc 模块测试 (展示 SUBCASE/TEST_SUITE/Decorators)
│   ├── test_greeting.cpp     # greeting 模块测试 (展示 BDD)
│   └── test_multi_calc.cpp   # multi-calc 模块测试 (展示 Mock)
├── ut.mk                     # 编译规则
└── ut_cov.mk                 # 覆盖率规则
```

## 📈 测试报告位置

| 类型 | 路径 |
|------|------|
| 测试报告 | `build/ut-doctest-report/report.html` |
| 覆盖率报告 | `build/coverage-doctest-report/index.html` |

## 🆚 与其他框架对比

| 特性 | doctest | Catch2 | GTest | CMocka |
|------|---------|--------|-------|--------|
| 语言 | C++ | C++ | C++ | C |
| 头文件库 | ✅ | ❌ (v3) | ❌ | ❌ |
| 编译速度 | ⭐⭐⭐⭐⭐ | ⭐⭐ | ⭐⭐⭐ | ⭐⭐⭐⭐ |
| 内置 Mock | ❌ | ❌ | ✅ (GMock) | ✅ (--wrap) |
| SUBCASE/SECTION | ✅ | ✅ | ❌ | ❌ |
| BDD 语法 | ✅ | ✅ | ❌ | ❌ |
| Test Suites | ✅ | ✅ (tags) | ✅ | ✅ |
| Decorators | ✅ | ✅ | ❌ | ❌ |
| 原生禁用宏支持 | ✅ (核心设计) | ❌ | ❌ | ❌ |
| 学习曲线 | 低 | 低 | 中 | 低 |
