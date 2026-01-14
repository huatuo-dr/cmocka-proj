# CppUTest 使用指南

官网：https://cpputest.github.io/

GitHub：https://github.com/cpputest/cpputest

## 📖 介绍

CppUTest 是一个面向 C/C++ 的单元测试和 Mock 框架，由 James Grenning 和 Bas Vodde 创建。它是《Test Driven Development for Embedded C》一书中推荐的测试框架。

### ✨ 主要特性

- 支持 C 和 C++ 代码测试
- **内置内存泄漏检测** 🔥
- 提供完整的 Mock 框架（CppUTestExt）
- 支持 Test Fixtures（setup 和 teardown）
- 丰富的断言宏
- 支持 JUnit XML 输出格式
- 跨平台支持

## 🔧 安装

### 全局安装

```shell
sudo apt-get update
sudo apt-get install cpputest
```

### 独立安装

```shell
git clone https://github.com/cpputest/cpputest.git
cd cpputest
```

选择下面其中一种方法进行编译

编译完成后，在 `cpputest-install` 目录中生成：
- `include/CppUTest/` - 核心头文件
- `include/CppUTestExt/` - 扩展库头文件（Mock 等）
- `lib/libCppUTest.a` - 核心静态库
- `lib/libCppUTestExt.a` - 扩展静态库

1. 方法一：Cmake

```shell
mkdir cpputest_build; cd cpputest_build
cmake .. -DCMAKE_INSTALL_PREFIX=./cpputest-install
cmake --build .
cmake --install .
```

2. 方法二：Autoreconf

```shell
mkdir cpputest_build; cd cpputest_build
autoreconf .. -i
../configure --prefix=$(pwd)/cpputest-install
make
make install
```

## 📁 项目结构

```text
.
├── Makefile                    # 主Makefile
├── cpputest.md                 # 本文档
├── 3rdparty/
│   └── cpputest/               # CppUTest 源码
├── sdk/                        # SDK库（被测代码）
│   ├── include/
│   │   ├── calc.h
│   │   ├── greeting.h
│   │   └── multi-calc.h
│   └── src/
│       ├── calc.c
│       ├── greeting.c
│       └── multi-calc.c
├── ut_cpputest/                # CppUTest 单元测试
│   ├── ut.mk                   # 测试编译规则
│   ├── ut_cov.mk               # 覆盖率编译规则
│   ├── cpputest-install/       # CppUTest 库（已编译）
│   │   ├── include/
│   │   └── lib/
│   └── src/                    # 测试源码
│       ├── main.cpp            # 测试入口
│       ├── test_calc.cpp       # calc 模块测试
│       ├── test_greeting.cpp   # greeting 模块测试
│       ├── test_multi_calc.cpp # multi-calc Mock 测试
│       └── test_memory_leak.cpp # 内存泄漏检测演示
└── build/                      # 构建产物
    ├── ut-cpputest-report/     # 测试报告
    └── coverage-cpputest-report/ # 覆盖率报告
```

## 🚀 构建命令

```shell
make help                    # 查看所有可用命令
make sdk                     # 编译SDK库
make sdk_install             # 安装SDK到build目录

make ut_cpputest             # 编译、运行测试并生成报告
make ut_cpputest_build       # 仅编译测试（不运行）
make ut_cpputest_run         # 运行测试（输出到终端）
make ut_cpputest_report      # 生成测试报告（XML + HTML）

make ut_cpputest_cov         # 运行测试并生成覆盖率报告
make ut_cpputest_cov_run     # 仅运行覆盖率测试
make ut_cpputest_cov_report  # 生成HTML覆盖率报告
make clean-cpputest-cov      # 清理覆盖率相关文件

make clean                   # 清理所有编译产物
```

## 📝 CppUTest 基础教程

### 1️⃣ 测试入口（main.cpp）

CppUTest 需要显式的 main 函数：

```cpp
#include "CppUTest/CommandLineTestRunner.h"

int main(int argc, char** argv)
{
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
```

### 2️⃣ 最简单的测试用例

```cpp
#include "CppUTest/TestHarness.h"

// 定义测试组
TEST_GROUP(FirstTestGroup)
{
};

// 定义测试用例
TEST(FirstTestGroup, FirstTest)
{
    FAIL("This test fails!");
}

TEST(FirstTestGroup, PassingTest)
{
    CHECK_EQUAL(5, 2 + 3);
}
```

### 3️⃣ 测试 C 代码

使用 `extern "C"` 包裹 C 头文件：

```cpp
#include "CppUTest/TestHarness.h"

extern "C" {
    #include "calc.h"
}

TEST_GROUP(CalcAdd)
{
};

TEST(CalcAdd, PositiveNumbers)
{
    CHECK_EQUAL(5, calc_add(2, 3));
}
```

### 4️⃣ 常用断言宏

#### 布尔断言

```cpp
CHECK(expression);              // 表达式为真
CHECK_TRUE(expression);         // 表达式为真
CHECK_FALSE(expression);        // 表达式为假
```

#### 相等断言

```cpp
CHECK_EQUAL(expected, actual);  // 相等（使用 ==）
LONGS_EQUAL(expected, actual);  // long 类型相等
DOUBLES_EQUAL(expected, actual, tolerance);  // double 相等（带容差）
```

#### 字符串断言

```cpp
STRCMP_EQUAL(expected, actual);     // 字符串相等
STRCMP_CONTAINS(expected, actual);  // actual 包含 expected
```

#### 指针断言

```cpp
POINTERS_EQUAL(expected, actual);   // 指针相等
CHECK(ptr != nullptr);              // 指针非空
```

#### 失败断言

```cpp
FAIL("message");                    // 直接失败
```

### 5️⃣ Test Fixtures（测试夹具）

```cpp
TEST_GROUP(Calculator)
{
    int* buffer;

    // 每个测试前执行
    void setup()
    {
        buffer = new int[100];
    }

    // 每个测试后执行
    void teardown()
    {
        delete[] buffer;
    }
};

TEST(Calculator, TestWithBuffer)
{
    buffer[0] = 42;
    CHECK_EQUAL(42, buffer[0]);
}
```

## 🔧 Mock 测试

CppUTest 提供了强大的 Mock 框架，位于 `CppUTestExt` 库中。

### Mock 基本原理

```text
┌─────────────────────────────────────────────────────────────────┐
│  1. 使用 --wrap 链接选项替换函数                                  │
│     原函数 calc_add() → __wrap_calc_add()                        │
│                                                                  │
│  2. 在 __wrap 函数中使用 Mock API                                 │
│     mock().actualCall("calc_add").returnIntValue()               │
│                                                                  │
│  3. 在测试中设置期望                                              │
│     mock().expectOneCall("calc_add").andReturnValue(100)         │
└─────────────────────────────────────────────────────────────────┘
```

### Mock 函数实现

```cpp
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
    #include "calc.h"
    // 链接器提供的原始函数
    extern int __real_calc_add(int a, int b);
}

// Mock 控制标志
static bool mock_calc_add_enabled = true;

// __wrap 函数
extern "C" int __wrap_calc_add(int a, int b)
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
```

### 测试用例中设置期望

```cpp
TEST_GROUP(MultiCalcMock)
{
    void setup()
    {
        mock_calc_add_enabled = true;
    }

    void teardown()
    {
        mock().checkExpectations();  // 验证所有期望都已满足
        mock().clear();              // 清理 mock 状态
    }
};

TEST(MultiCalcMock, ExpressionWithMock)
{
    // 设置期望：calc_add(2, 3) 返回 5
    mock().expectOneCall("calc_add")
          .withParameter("a", 2)
          .withParameter("b", 3)
          .andReturnValue(5);

    // 调用被测函数
    int result = some_function_that_calls_calc_add(2, 3);

    CHECK_EQUAL(5, result);
}
```

### Mock API 汇总

```cpp
// 设置期望
mock().expectOneCall("func");              // 期望调用一次
mock().expectNCalls(3, "func");            // 期望调用 N 次
mock().expectNoCall("func");               // 期望不被调用

// 参数匹配
.withParameter("name", value)              // 匹配参数
.withOutputParameterReturning("name", &value, sizeof(value))

// 返回值
.andReturnValue(value)                     // 设置返回值

// 在 wrap 函数中
mock().actualCall("func")                  // 记录实际调用
      .withParameter("a", a)
      .returnIntValueOrDefault(0);         // 获取返回值（带默认值）

// 验证和清理
mock().checkExpectations();                // 验证期望
mock().clear();                            // 清理状态
```

### 混合测试（Real + Mock）

```cpp
// 禁用所有 Mock，使用真实函数
static void disable_all_mocks()
{
    mock_calc_add_enabled = false;
    mock_calc_subtract_enabled = false;
}

TEST(MultiCalcHybrid, RealFunctions)
{
    disable_all_mocks();

    int result = multi_calc_expression(2, 3, 10, 4);
    CHECK_EQUAL(30, result);  // 使用真实函数计算
}

TEST(MultiCalcHybrid, PartialMock)
{
    // 只 mock 乘法，其他使用真实函数
    mock_calc_add_enabled = false;
    mock_calc_subtract_enabled = false;
    mock_calc_multiply_enabled = true;

    mock().expectOneCall("calc_multiply")
          .withParameter("a", 5)
          .withParameter("b", 6)
          .andReturnValue(999);

    int result = multi_calc_expression(2, 3, 10, 4);
    CHECK_EQUAL(999, result);
}
```

## 🔍 内存泄漏检测

CppUTest **内置内存泄漏检测**，这是它的一大特色功能。

### 自动检测

每个测试结束后，CppUTest 会自动检查是否有内存泄漏：

```cpp
TEST_GROUP(MemoryTest)
{
};

// 这个测试会失败，因为有内存泄漏
TEST(MemoryTest, LeakDetected)
{
    char* leak = new char[100];
    // 忘记 delete[] leak; → CppUTest 会报告泄漏
}

// 这个测试会通过
TEST(MemoryTest, NoLeak)
{
    char* buffer = new char[100];
    delete[] buffer;  // 正确释放
}
```

### 泄漏报告示例

```text
TEST(MemoryTest, LeakDetected)
Memory leak(s) found.
Alloc num (1) Leak size: 100 Allocated at: test_memory_leak.cpp:25

Total number of leaks: 1
```

### 期望内存泄漏

有时测试代码故意不释放内存，可以使用 `EXPECT_N_LEAKS`：

```cpp
TEST(MemoryLeakExpected, ExpectedLeak)
{
    EXPECT_N_LEAKS(1);  // 告诉 CppUTest 期望 1 个泄漏

    char* intentional_leak = new char[50];
    (void)intentional_leak;
    // 不释放 - 但测试会通过
}
```

### 启用增强的泄漏检测信息

在编译时添加内存泄漏检测宏可以获得更详细的信息：

```makefile
CXXFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorNewMacros.h
CFLAGS += -include $(CPPUTEST_HOME)/include/CppUTest/MemoryLeakDetectorMallocMacros.h
```

## 📊 测试输出示例

运行 `make ut_cpputest_run` 后的输出：

```text
========================================
Running CppUTest Unit Tests...
========================================

--- Running cpputest_test_basic ---
TEST(MemoryLeakExpected, ExpectMultipleLeaks) - 0 ms
TEST(MemoryLeakExpected, ExpectOneLeak) - 0 ms
TEST(MemoryLeakDemo, ObjectAllocationNoLeak) - 0 ms
TEST(MemoryLeakDemo, MultipleAllocationsNoLeak) - 0 ms
TEST(MemoryLeakDemo, NoLeakWithMalloc) - 0 ms
TEST(MemoryLeakDemo, NoLeakWithNew) - 0 ms
TEST(GreetingGoodbye, NullName) - 0 ms
TEST(GreetingGoodbye, EmptyName) - 0 ms
TEST(GreetingGoodbye, DifferentNames) - 0 ms
TEST(GreetingGoodbye, BasicName) - 0 ms
TEST(GreetingHello, NullName) - 0 ms
TEST(GreetingHello, EmptyName) - 0 ms
TEST(GreetingHello, DifferentNames) - 0 ms
TEST(GreetingHello, BasicName) - 0 ms
TEST(CalcDivide, DivideByZero) - 0 ms
TEST(CalcDivide, IntegerDivision) - 0 ms
...

OK (30 tests, 30 ran, 45 checks, 0 ignored, 0 filtered out, 1 ms)

--- Running cpputest_test_multi_calc (Mock Tests) ---
TEST(MultiCalcHybrid, CompareMockVsReal) - 0 ms
TEST(MultiCalcHybrid, ExpressionWithPartialMock) - 0 ms
TEST(MultiCalcReal, AverageWithLargerNumbers) - 0 ms
TEST(MultiCalcReal, AverageWithRealFunctions) - 0 ms
TEST(MultiCalcReal, ExpressionWithRealFunctions) - 0 ms
TEST(MultiCalcMock, AverageWithFullMock) - 0 ms
TEST(MultiCalcMock, ExpressionWithMockedError) - 0 ms
TEST(MultiCalcMock, ExpressionWithFullMock) - 0 ms

OK (8 tests, 8 ran, 12 checks, 0 ignored, 0 filtered out, 0 ms)
```

## 📈 命令行选项

```shell
./test_program -v              # 详细输出（显示每个测试名）
./test_program -r2             # 重复运行 2 次
./test_program -g GroupName    # 只运行指定组
./test_program -n TestName     # 只运行指定测试
./test_program -ojunit         # 输出 JUnit XML 格式
./test_program -h              # 显示帮助
```

## 🔨 编译选项

### Makefile 示例

```makefile
# CppUTest 路径
CPPUTEST_INC := ut_cpputest/cpputest-install/include
CPPUTEST_LIB := ut_cpputest/cpputest-install/lib

# 编译选项
CXXFLAGS := -Wall -Wextra -g -std=c++11 -I$(CPPUTEST_INC) -I<你的头文件目录>
LDFLAGS := -L$(CPPUTEST_LIB) -lCppUTest -lCppUTestExt

# Mock 测试需要 --wrap 选项
MOCK_LDFLAGS := $(LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract

# 编译测试程序
test_basic: test_calc.cpp test_greeting.cpp main.cpp
    $(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

test_mock: test_multi_calc.cpp main.cpp
    $(CXX) $(CXXFLAGS) $^ -o $@ $(MOCK_LDFLAGS)
```

## 📚 本项目展示的 CppUTest 特性

| 特性 | 文件 | 说明 |
|------|------|------|
| 基本断言 | test_calc.cpp | CHECK_EQUAL, CHECK_TRUE 等 |
| 字符串断言 | test_greeting.cpp | STRCMP_EQUAL, CHECK |
| 指针断言 | test_greeting.cpp | POINTERS_EQUAL |
| Test Fixtures | test_greeting.cpp | setup(), teardown() |
| **Mock 测试** | test_multi_calc.cpp | mock().expectOneCall() |
| **参数验证** | test_multi_calc.cpp | withParameter() |
| **混合测试** | test_multi_calc.cpp | 真实函数 + Mock 切换 |
| **内存泄漏检测** | test_memory_leak.cpp | 自动检测 + EXPECT_N_LEAKS |
| **JUnit XML 报告** | ut.mk | -ojunit 选项 |

## 🔗 参考资料

- [CppUTest 官网](https://cpputest.github.io/)
- [CppUTest GitHub](https://github.com/cpputest/cpputest)
- [CppUTest Manual](https://cpputest.github.io/manual.html)
- 《Test Driven Development for Embedded C》 by James Grenning
