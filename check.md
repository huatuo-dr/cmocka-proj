# Check 单元测试框架

仓库：https://github.com/libcheck/check.git

官方文档：https://libcheck.github.io/check

## 📖 介绍

Check 是一个专为 C 语言设计的单元测试框架，具有以下核心特性：

### ✨ 主要特性

| 特性 | 说明 |
|------|------|
| **Fork 隔离** | 每个测试在独立进程运行，自动捕获 segfault/信号，测试崩溃不影响框架 |
| **信号测试** | 支持测试期望抛出特定信号（如 SIGSEGV、SIGFPE） |
| **退出值测试** | 支持测试期望以特定退出码退出 |
| **循环测试** | 内置循环测试支持，简化参数化测试 |
| **超时控制** | 可设置测试超时时间，防止死循环 |
| **双模式 Fixture** | Checked（每个测试前后）/ Unchecked（测试组前后） |
| **多输出格式** | 同时支持 XML、TAP、Log 输出 |
| **checkmk 工具** | 简化语法生成完整测试代码，减少样板代码 |

### 🔄 与其他框架对比

| 特性 | Check | CMocka | Unity | GTest |
|------|-------|--------|-------|-------|
| 语言 | C | C | C | C++ |
| Fork 隔离 | ✅ 默认开启 | ⚠️ 可选 | ❌ | ❌ |
| 信号测试 | ✅ | ❌ | ❌ | ❌ |
| 退出值测试 | ✅ | ❌ | ❌ | ❌ |
| 循环测试 | ✅ 内置 | ⚠️ prestate | ❌ | ✅ 参数化 |
| 超时控制 | ✅ | ❌ | ❌ | ❌ |
| 代码生成工具 | ✅ checkmk | ❌ | ❌ | ❌ |
| 内置 Mock | ❌ 需 --wrap | ✅ | ❌ 需 fff | ✅ GMock |

### 📋 Mock 实现

Check 不内置 Mock 功能，使用 GCC 链接器的 `--wrap` 选项实现（与 CMocka 相同）：

```makefile
LDFLAGS += -Wl,--wrap=calc_add -Wl,--wrap=calc_subtract
```

## 🔧 安装

### 全局安装

```shell
sudo apt-get update
sudo apt-get install check
```

### 独立安装

需要提前安装`texinfo`工具：`sudo apt install texinfo`

```shell
# commit: 455005dc29dc6727de7ee36fee4b49a13b39f73f
git clone https://github.com/libcheck/check.git
cd check
mkdir check_build; cd check_build
autoreconf --install ..
../configure --prefix="$(pwd)/check-install"
make
make install
```

编译完成后，在 `check-install` 目录中生成：

```
check-install/
├── bin/
│   └── checkmk              # 辅助工具，从简化语法生成 Check 测试代码
├── include/
│   ├── check.h              # 主头文件，编写测试时 #include <check.h>
│   └── check_stdint.h       # 标准整型定义
├── lib/
│   ├── libcheck.a           # 静态库
│   ├── libcheck.so          # 动态库符号链接
│   ├── libcheck.so.0        # 动态库符号链接
│   ├── libcheck.so.0.0.0    # 动态库实际文件
│   ├── libcheck.la          # libtool 归档文件
│   └── pkgconfig/           # pkg-config 配置文件
│       └── check.pc
└── share/
    ├── aclocal/
    │   └── check.m4         # autotools 宏文件
    ├── doc/check/           # 文档与示例代码
    │   ├── example/         # money 示例工程
    │   ├── README
    │   ├── NEWS
    │   └── ChangeLog
    └── man/                  # man 手册页
        ├── man1/checkmk.1
        ├── man3/suite_create.3
        └── man7/libcheck.7
```

## 🛠️ checkmk 代码生成工具

`checkmk` 是 Check 框架独有的测试代码生成工具，用于将简化语法的 `.ts` 文件转换为完整的 C 测试代码。

### 存在目的

使用 Check 框架需要编写大量样板代码：
- 创建 Suite、TCase、SRunner
- 建立 Suite/TCase/Test 之间的关系
- 编写 main 函数调用 `srunner_run_all`

`checkmk` 让你只需关注测试逻辑，自动生成所有样板代码。

### 简化语法关键字

| 指令 | 说明 | 示例 |
|------|------|------|
| `#suite Name` | 定义 Suite | `#suite CalcSuite` |
| `#tcase Name` | 定义 TCase | `#tcase AddTests` |
| `#test name` | 定义测试函数 | `#test test_add_positive` |
| `#test-signal(sig) name` | 期望抛出信号的测试 | `#test-signal(SIGSEGV) test_null_ptr` |
| `#test-exit(code) name` | 期望特定退出码的测试 | `#test-exit(1) test_exit_failure` |
| `#test-loop(s, e) name` | 循环测试（变量 `_i` 可用） | `#test-loop(0, 10) test_array` |
| `#main-pre` | main 函数开头插入代码 | 用于初始化 |
| `#main-post` | main 函数结尾插入代码 | 用于清理 |

### 使用示例

#### 简化语法文件（test_calc.ts）

```c
#include <stdlib.h>
#include "calc.h"

#suite CalcSuite

#tcase AddTests

#test test_add_positive
    ck_assert_int_eq(calc_add(2, 3), 5);
    ck_assert_int_eq(calc_add(100, 200), 300);

#test test_add_negative
    ck_assert_int_eq(calc_add(-2, -3), -5);

#tcase SubtractTests

#test test_subtract_basic
    ck_assert_int_eq(calc_subtract(5, 3), 2);

#test-loop(0, 5) test_loop_example
    // _i is the loop variable (0, 1, 2, 3, 4)
    ck_assert_int_ge(_i, 0);
    ck_assert_int_lt(_i, 5);
```

#### 生成 C 代码

```bash
checkmk test_calc.ts > test_calc.c
```

#### 编译运行

```bash
gcc -o test_calc test_calc.c -lcheck -pthread -lm -lrt
./test_calc
```

### 生成代码结构

`checkmk` 自动生成：
- `START_TEST` / `END_TEST` 包裹的测试函数
- Suite 创建函数 `{SuiteName}_suite()`
- 完整的 `main()` 函数（含 SRunner 配置）
- `#line` 指令（方便调试定位到原始 .ts 文件）

## 📋 断言宏速查

### 基本断言

| 宏 | 说明 |
|------|------|
| `ck_assert(expr)` | 表达式为真 |
| `ck_assert_msg(expr, ...)` | 表达式为真，失败时打印消息 |
| `ck_abort()` | 无条件失败 |
| `ck_abort_msg(...)` | 无条件失败并打印消息 |

### 整型断言

| 宏 | 说明 |
|------|------|
| `ck_assert_int_eq(X, Y)` | X == Y |
| `ck_assert_int_ne(X, Y)` | X != Y |
| `ck_assert_int_lt(X, Y)` | X < Y |
| `ck_assert_int_le(X, Y)` | X <= Y |
| `ck_assert_int_gt(X, Y)` | X > Y |
| `ck_assert_int_ge(X, Y)` | X >= Y |

### 无符号整型断言

| 宏 | 说明 |
|------|------|
| `ck_assert_uint_eq(X, Y)` | X == Y |
| `ck_assert_uint_ne(X, Y)` | X != Y |
| `ck_assert_uint_lt(X, Y)` | X < Y |
| `ck_assert_uint_le(X, Y)` | X <= Y |
| `ck_assert_uint_gt(X, Y)` | X > Y |
| `ck_assert_uint_ge(X, Y)` | X >= Y |

### 字符串断言

| 宏 | 说明 |
|------|------|
| `ck_assert_str_eq(X, Y)` | strcmp(X, Y) == 0 |
| `ck_assert_str_ne(X, Y)` | strcmp(X, Y) != 0 |
| `ck_assert_str_lt(X, Y)` | strcmp(X, Y) < 0 |
| `ck_assert_str_le(X, Y)` | strcmp(X, Y) <= 0 |
| `ck_assert_str_gt(X, Y)` | strcmp(X, Y) > 0 |
| `ck_assert_str_ge(X, Y)` | strcmp(X, Y) >= 0 |

### 指针断言

| 宏 | 说明 |
|------|------|
| `ck_assert_ptr_eq(X, Y)` | X == Y |
| `ck_assert_ptr_ne(X, Y)` | X != Y |
| `ck_assert_ptr_null(X)` | X == NULL |
| `ck_assert_ptr_nonnull(X)` | X != NULL |

### 浮点断言

| 宏 | 说明 |
|------|------|
| `ck_assert_float_eq_tol(X, Y, T)` | \|X - Y\| < T |
| `ck_assert_float_ne_tol(X, Y, T)` | \|X - Y\| >= T |
| `ck_assert_double_eq_tol(X, Y, T)` | \|X - Y\| < T |
| `ck_assert_double_nan(X)` | X 是 NaN |
| `ck_assert_double_infinite(X)` | X 是无穷大 |

### 内存断言

| 宏 | 说明 |
|------|------|
| `ck_assert_mem_eq(X, Y, L)` | memcmp(X, Y, L) == 0 |
| `ck_assert_mem_ne(X, Y, L)` | memcmp(X, Y, L) != 0 |

## 🎯 Check 特有功能

### 信号测试

测试期望抛出特定信号：

```c
#test-signal(SIGSEGV) test_null_pointer_access
    int *p = NULL;
    *p = 42;  // Will trigger SIGSEGV
```

### 退出值测试

测试期望以特定退出码退出：

```c
#test-exit(42) test_exit_with_code
    exit(42);  // Test passes if exit code is 42
```

### 循环测试

循环执行测试，变量 `_i` 可用：

```c
#test-loop(0, 100) test_array_bounds
    int arr[100] = {0};
    arr[_i] = _i;
    ck_assert_int_eq(arr[_i], _i);
```

### 超时控制

在手写 C 代码中设置超时：

```c
TCase *tc = tcase_create("TimeoutTests");
tcase_set_timeout(tc, 5);  // 5 seconds timeout
tcase_add_test(tc, test_long_running);
```

### Fixture（测试夹具）

#### Checked Fixture（每个测试前后执行）

```c
void setup(void) {
    // Runs before each test
}

void teardown(void) {
    // Runs after each test
}

tcase_add_checked_fixture(tc, setup, teardown);
```

#### Unchecked Fixture（测试组前后执行）

```c
void suite_setup(void) {
    // Runs once before the test case
}

void suite_teardown(void) {
    // Runs once after the test case
}

tcase_add_unchecked_fixture(tc, suite_setup, suite_teardown);
```

## 🌍 环境变量

| 变量 | 说明 | 可选值 |
|------|------|--------|
| `CK_FORK` | 控制 fork 模式 | `yes` / `no` |
| `CK_VERBOSITY` | 输出详细程度 | `silent` / `minimal` / `normal` / `verbose` |
| `CK_RUN_SUITE` | 只运行指定 Suite | Suite 名称 |
| `CK_RUN_CASE` | 只运行指定 TCase | TCase 名称 |
| `CK_INCLUDE_TAGS` | 只运行包含指定标签的测试 | 空格分隔的标签 |
| `CK_EXCLUDE_TAGS` | 排除包含指定标签的测试 | 空格分隔的标签 |
| `CK_DEFAULT_TIMEOUT` | 默认超时时间（秒） | 数字 |

## 📁 项目结构

```
ut_check/
├── check-install/              # Check 库（符号链接）
├── src/
│   ├── test_calc.ts            # calc 模块测试源文件
│   ├── test_greeting.ts        # greeting 模块测试源文件
│   ├── test_multi_calc.ts      # multi-calc 模块测试（Mock）
│   └── test_check_features.ts  # Check 特有功能演示
├── ut.mk                       # 测试编译规则
└── ut_cov.mk                   # 覆盖率编译规则
```

## 🚀 运行测试

```bash
# 构建并运行测试
make ut_check

# 仅构建
make ut_check_build

# 仅运行
make ut_check_run

# 生成报告
make ut_check_report

# 覆盖率测试
make ut_check_cov
```
