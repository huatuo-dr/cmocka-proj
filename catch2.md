# Catch2 单元测试框架

仓库：https://github.com/catchorg/Catch2

## 📖 介绍

Catch2 是一个现代化的 C++ 单元测试框架，主要特点：

- **自然语法** - 使用普通 C++ 表达式作为断言，无需记忆大量宏
- **SECTION 机制** - 替代传统 fixture，更灵活的 setup/teardown
- **BDD 支持** - 内置 `SCENARIO`/`GIVEN`/`WHEN`/`THEN` 语法
- **自动注册** - 测试用例自动发现，无需手动注册
- **内置基准测试** - 支持 micro-benchmark
- **无内置 Mock** - 需配合 `--wrap` 链接选项或其他 Mock 库

## 🔧 安装

### 全局安装

```shell
sudo apt update
sudo apt install catch2
```

### 独立安装

执行命令：

```shell
git clone https://github.com/catchorg/Catch2.git
cd Catch2
git checkout v3.12.0
cmake -Bbuild -S. -DCMAKE_INSTALL_PREFIX=build/catch2-install
cmake --build build --target install
```

编译完成后，在 `build/catch2-install` 目录中生成：

```
catch2-install/
├── include/catch2/    # 头文件
└── lib/
    ├── libCatch2.a      # 核心库
    └── libCatch2Main.a  # 提供 main() 函数
```

## 🧪 基本用法

### 最简单的测试用例

```cpp
#include <catch2/catch_test_macros.hpp>

TEST_CASE("加法测试", "[calc]") {
    REQUIRE(1 + 1 == 2);
    CHECK(2 + 2 == 4);  // CHECK fails but continues, REQUIRE stops on failure
}
```

### 常用断言宏

| 宏 | 用途 |
|----|------|
| `REQUIRE(expr)` | 断言，失败则终止当前测试 |
| `CHECK(expr)` | 断言，失败后继续执行 |
| `REQUIRE_FALSE(expr)` | 断言表达式为 false |
| `REQUIRE_THROWS(expr)` | 断言抛出异常 |
| `REQUIRE_THROWS_AS(expr, type)` | 断言抛出指定类型异常 |
| `REQUIRE_NOTHROW(expr)` | 断言不抛异常 |

## 📝 SECTION 语法

SECTION 是 Catch2 的特色功能，用于在测试用例中共享 setup 代码。每个 SECTION 独立运行，TEST_CASE 中 SECTION 之前的代码会在每个 SECTION 执行前重新运行。

### 基本 SECTION

```cpp
TEST_CASE("Calculator operations with shared setup", "[calc]") {
    // This setup runs BEFORE EACH section
    int operand_a = 10;
    int operand_b = 5;

    SECTION("Addition") {
        REQUIRE(calc_add(operand_a, operand_b) == 15);
    }

    SECTION("Subtraction") {
        REQUIRE(calc_subtract(operand_a, operand_b) == 5);
    }

    SECTION("Multiplication") {
        REQUIRE(calc_multiply(operand_a, operand_b) == 50);
    }
}
```

**执行流程**：
1. 初始化 `operand_a=10, operand_b=5` → 执行 "Addition" SECTION
2. 初始化 `operand_a=10, operand_b=5` → 执行 "Subtraction" SECTION
3. 初始化 `operand_a=10, operand_b=5` → 执行 "Multiplication" SECTION

### 嵌套 SECTION

```cpp
TEST_CASE("Nested sections for complex scenarios", "[calc]") {
    int base = 100;

    SECTION("With positive operand") {
        int operand = 10;

        SECTION("Addition increases value") {
            REQUIRE(calc_add(base, operand) == 110);
        }

        SECTION("Subtraction decreases value") {
            REQUIRE(calc_subtract(base, operand) == 90);
        }
    }

    SECTION("With negative operand") {
        int operand = -10;

        SECTION("Addition decreases value") {
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

Catch2 提供 BDD (Behavior-Driven Development) 风格的宏，使测试读起来像规格说明。

### BDD 宏对照表

| BDD 宏 | 等效于 | 前缀 |
|--------|--------|------|
| `SCENARIO(name, tags)` | `TEST_CASE("Scenario: " + name, tags)` | "Scenario: " |
| `GIVEN(desc)` | `SECTION("Given: " + desc)` | "Given: " |
| `WHEN(desc)` | `SECTION("When: " + desc)` | "When: " |
| `THEN(desc)` | `SECTION("Then: " + desc)` | "Then: " |
| `AND_GIVEN(desc)` | `SECTION("And given: " + desc)` | "And given: " |
| `AND_WHEN(desc)` | `SECTION("And when: " + desc)` | "And when: " |
| `AND_THEN(desc)` | `SECTION("And then: " + desc)` | "And then: " |

### BDD 示例

```cpp
SCENARIO("User receives a hello greeting", "[greeting][bdd]") {

    GIVEN("A valid user name") {
        const char* name = "Alice";

        WHEN("The hello greeting is requested") {
            const char* result = greeting_hello(name);

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
SCENARIO("Different greetings for the same user", "[greeting][bdd]") {

    GIVEN("A user named Eve") {
        const char* name = "Eve";

        WHEN("She receives a hello greeting") {
            const char* result = greeting_hello(name);

            THEN("It should be a welcoming message") {
                REQUIRE(std::strstr(result, "Hello") != nullptr);
            }
        }

        WHEN("She receives a goodbye greeting") {
            const char* result = greeting_goodbye(name);

            THEN("It should be a farewell message") {
                REQUIRE(std::strstr(result, "Goodbye") != nullptr);
            }
        }
    }
}
```

## 🔄 Mock 实现

Catch2 不内置 Mock 功能，本项目使用链接器 `--wrap` 选项实现 Mock：

### 原理

```
链接时: -Wl,--wrap=func_name
  - 调用 func_name → 实际调用 __wrap_func_name
  - 调用 __real_func_name → 实际调用原始 func_name
```

### Mock 实现示例

```cpp
// Mock control flags
static bool mock_calc_add = true;
static int mock_add_return = 0;

// Real function declaration
extern "C" {
    extern int __real_calc_add(int a, int b);
}

// Wrap function
extern "C" {
    int __wrap_calc_add(int a, int b) {
        if (mock_calc_add) {
            return mock_add_return;
        }
        return __real_calc_add(a, b);
    }
}

// Test
TEST_CASE("Expression with mocked calc_add", "[multi-calc][mock]") {
    mock_calc_add = true;
    mock_add_return = 100;

    int result = multi_calc_expression(1, 2, 3, 4);

    REQUIRE(result == /* expected based on mock */);
}
```

## 📊 报告输出

### 命令行选项

```shell
# 终端输出 (默认)
./test_runner

# JUnit XML 格式
./test_runner --reporter JUnit --out result.xml

# 同时输出多种格式
./test_runner --reporter console --reporter JUnit::out=result.xml

# 列出可用的 reporter
./test_runner --list-reporters
```

### 支持的 Reporter

| Reporter | 说明 |
|----------|------|
| `console` | 默认终端输出 |
| `JUnit` | JUnit XML 格式 |
| `xml` | Catch2 原生 XML |
| `compact` | 紧凑输出 |
| `SonarQube` | SonarQube 格式 |

## 🚀 编译和运行

### 编译命令

```shell
g++ -std=c++17 test.cpp \
    -Iut_catch2/catch2-install/include \
    -Lut_catch2/catch2-install/lib \
    -lCatch2Main -lCatch2 \
    -o test_runner
```

### Make 命令

```shell
# 运行测试并生成报告
make ut_catch2

# 仅构建
make ut_catch2_build

# 运行测试（终端输出）
make ut_catch2_run

# 生成 HTML 报告
make ut_catch2_report

# 运行覆盖率测试
make ut_catch2_cov
```

## 📁 目录结构

```
ut_catch2/
├── catch2-install/          # Catch2 安装目录
│   ├── include/catch2/      # 头文件
│   └── lib/                 # 库文件
│       ├── libCatch2.a
│       └── libCatch2Main.a
├── src/                     # 测试源码
│   ├── test_calc.cpp        # calc 模块测试 (展示 SECTION)
│   ├── test_greeting.cpp    # greeting 模块测试 (展示 BDD)
│   └── test_multi_calc.cpp  # multi-calc 模块测试 (展示 Mock)
├── ut.mk                    # 编译规则
└── ut_cov.mk                # 覆盖率规则
```

## 📈 测试报告位置

| 类型 | 路径 |
|------|------|
| 测试报告 | `build/ut-catch2-report/report.html` |
| 覆盖率报告 | `build/coverage-catch2-report/index.html` |

## 🆚 与其他框架对比

| 特性 | Catch2 | GTest | CMocka |
|------|--------|-------|--------|
| 语言 | C++ | C++ | C |
| 内置 Mock | ❌ | ✅ (GMock) | ✅ (--wrap) |
| SECTION 机制 | ✅ | ❌ | ❌ |
| BDD 语法 | ✅ 内置 | ❌ | ❌ |
| 断言语法 | 自然表达式 | EXPECT_*/ASSERT_* | assert_* |
| 学习曲线 | 低 | 中 | 低 |
