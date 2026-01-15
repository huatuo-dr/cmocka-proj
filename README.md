# C/C++ 单元测试框架学习项目

本项目用于学习和对比多种 C/C++ 单元测试框架，提供统一的 SDK 被测代码和多种测试框架实现。

## 📁 项目结构

```
cmocka-proj/
├── sdk/                      # 被测 SDK 库
│   ├── include/              # 头文件
│   │   ├── calc.h            # 计算模块
│   │   ├── greeting.h        # 问候模块
│   │   └── multi-calc.h      # 复合计算模块
│   └── src/                  # 源码实现
│
├── application/              # 示例应用程序
│
├── ut_cmocka/                # CMocka 单元测试
├── ut_unity_fff/             # Unity + fff 单元测试
├── ut_gtest_gmock/           # GoogleTest + GMock 单元测试
├── ut_gtest_mockcpp/         # GoogleTest + MockCpp 单元测试
├── ut_cpputest/              # CppUTest 单元测试
├── ut_check/                 # Check 单元测试
│
├── 3rdparty/                 # 第三方库源码
│   ├── cmocka-2.0.0/
│   ├── Unity-2.6.1/
│   ├── fff-1.1/
│   ├── googletest-1.17.0/
│   ├── mockcpp/
│   ├── cpputest/
│   └── check/
│
├── build/                    # 构建中间产物
├── output/                   # 编译输出
└── dist/                     # 最终可执行文件
```

## 🧪 支持的测试框架

| 框架 | Mock 方式 | 语言 | 文档 |
|------|----------|------|------|
| **CMocka** | 链接时 `--wrap` | C | [cmocka.md](cmocka.md) |
| **Unity + fff** | 函数指针替换 | C | [unity_fff.md](unity_fff.md) |
| **GoogleTest + GMock** | 链接时 `--wrap` | C++ | [gtest_gmock.md](gtest_gmock.md) |
| **GoogleTest + MockCpp** | 运行时 Hook | C++ | [gtest_mockcpp.md](gtest_mockcpp.md) |
| **CppUTest** | 链接时 `--wrap` + Mock API | C++ | [cpputest.md](cpputest.md) |
| **Check** | 链接时 `--wrap` | C | [check.md](check.md) |

## 🔧 SDK 模块说明

SDK 是被测代码，包含三个模块：

### calc 模块
基础计算函数：
```c
int calc_add(int a, int b);       // 加法
int calc_subtract(int a, int b);  // 减法
int calc_multiply(int a, int b);  // 乘法
int calc_divide(int a, int b);    // 除法
```

### greeting 模块
问候消息函数：
```c
const char* greeting_hello(const char* name);
const char* greeting_goodbye(const char* name);
```

### multi-calc 模块
复合计算函数（依赖 calc 模块，适合测试 mock）：
```c
// 计算表达式: (a + b) * (c - d)
int multi_calc_expression(int a, int b, int c, int d);

// 计算平均值: (a + b + c) / 3
int multi_calc_average(int a, int b, int c);
```

## 🚀 快速开始

### 构建 SDK

```shell
make sdk           # 构建 SDK 库
make sdk_install   # 安装 SDK 到 build/sdk
```

### 运行应用程序

```shell
make app           # 构建应用
make run           # 运行应用
```

### 运行测试

```shell
# 运行所有框架测试
make ut                    # 运行所有单元测试
make ut_cov                # 运行所有覆盖率测试

# CMocka 测试
make ut_cmocka             # 运行测试并生成报告
make ut_cmocka_cov         # 运行测试并生成覆盖率报告

# Unity + fff 测试
make ut_unity              # 运行测试并生成报告
make ut_unity_cov          # 运行测试并生成覆盖率报告

# GoogleTest + GMock 测试
make ut_gtest              # 运行测试并生成报告
make ut_gtest_cov          # 运行测试并生成覆盖率报告

# GoogleTest + MockCpp 测试
make ut_gtest_mockcpp      # 运行测试并生成报告
make ut_gtest_mockcpp_cov  # 运行测试并生成覆盖率报告

# CppUTest 测试
make ut_cpputest           # 运行测试并生成报告
make ut_cpputest_cov       # 运行测试并生成覆盖率报告

# Check 测试
make ut_check              # 运行测试并生成报告
make ut_check_cov          # 运行测试并生成覆盖率报告
```

### 清理

```shell
make clean         # 清理所有构建产物
```

## 📊 测试报告

### 报告位置

| 框架 | 测试报告 | 覆盖率报告 |
|------|---------|-----------|
| CMocka | `build/ut-cmocka-report/` | `build/coverage-cmocka-report/` |
| Unity + fff | `build/ut-unity-report/` | `build/coverage-unity-report/` |
| GoogleTest + GMock | `build/ut-gtest-report/` | `build/coverage-gtest-report/` |
| GoogleTest + MockCpp | `build/ut-gtest-mockcpp-report/` | `build/coverage-gtest-mockcpp-report/` |
| CppUTest | `build/ut-cpputest-report/` | `build/coverage-cpputest-report/` |
| Check | `build/ut-check-report/` | `build/coverage-check-report/` |

### 报告生成机制对比

| 特性 | CMocka | Unity+fff | GTest+GMock | GTest+MockCpp | CppUTest | Check |
|------|--------|-----------|-------------|---------------|----------|-------|
| **原生输出格式** | XML (JUnit) | TXT | XML (JUnit) | XML (JUnit) | XML (JUnit) | XML |
| **HTML 转换工具** | junit2html | junit2html | junit2html | junit2html | junit2html | junit2html |
| **需要额外脚本** | ❌ | ✅ unity_to_junit.py | ❌ | ❌ | ❌ | ❌ |
| **终端输出可读性** | ⭐⭐⭐ 良好 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐ 良好 |
| **HTML 报告可读性** | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 | ⭐⭐⭐⭐ 优秀 |

### 报告生成流程

```
CMocka:
  测试程序 → XML (CMOCKA_MESSAGE_OUTPUT=XML) → junit2html → HTML

Unity + fff:
  测试程序 → TXT → unity_to_junit.py → XML → junit2html → HTML

GoogleTest:
  测试程序 → XML (--gtest_output=xml:) → junit2html → HTML

CppUTest:
  测试程序 → XML (-ojunit) → junit2html → HTML

Check:
  测试程序 → XML (CK_XML_FILE=) → junit2html → HTML
```

### 覆盖率报告

所有框架使用相同的覆盖率工具链：

```
源码编译 (--coverage) → 运行测试 → lcov (收集数据) → genhtml (生成 HTML)
```

### 第三方工具依赖

| 工具 | 用途 | 安装方式 |
|------|------|---------|
| `junit2html` | XML → HTML 转换 | `pip install junit2html` |
| `lcov` | 覆盖率数据收集 | `apt install lcov` |
| `genhtml` | 覆盖率 HTML 生成 | 包含在 lcov 中 |

## 📚 框架对比

| 特性 | CMocka | Unity+fff | GTest+GMock | GTest+MockCpp | CppUTest | Check |
|------|--------|-----------|-------------|---------------|----------|-------|
| 语言 | C | C | C++ | C++ | C++ | C |
| Mock 机制 | `--wrap` | 函数指针 | `--wrap` | 运行时 Hook | `--wrap` + Mock API | `--wrap` |
| 无需链接选项 | ❌ | ✅ | ❌ | ✅ | ❌ | ❌ |
| 调用真实函数 | `__real_` | 保存原指针 | `__real_` | 不 MOCKER 即可 | `__real_` | `__real_` |
| 参数匹配 | 手动 | 手动 | ✅ 自动 | ✅ 自动 | ✅ 自动 | 手动 |
| 调用次数验证 | 手动 | 手动 | ✅ 自动 | ✅ 自动 | ✅ 自动 | 手动 |
| 内存泄漏检测 | ❌ | ❌ | ❌ | ❌ | ✅ 内置 | ❌ |
| Fork 隔离 | ⚠️ 可选 | ❌ | ❌ | ❌ | ❌ | ✅ 默认 |
| 信号/退出测试 | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ |
| 代码生成工具 | ❌ | ❌ | ❌ | ❌ | ❌ | ✅ checkmk |
| 学习曲线 | 低 | 低 | 中 | 中 | 中 | 低 |

## 📖 详细文档

- [CMocka 使用指南](cmocka.md) - CMocka 框架详细说明
- [Unity + fff 使用指南](unity_fff.md) - Unity 和 fff 框架详细说明
- [GoogleTest + GMock 使用指南](gtest_gmock.md) - GoogleTest 和 GMock 详细说明
- [GoogleTest + MockCpp 使用指南](gtest_mockcpp.md) - GoogleTest 和 MockCpp 详细说明
- [CppUTest 使用指南](cpputest.md) - CppUTest 框架详细说明（含内存泄漏检测）
- [Check 使用指南](check.md) - Check 框架详细说明（含 checkmk 代码生成、信号/退出值测试）

## 🛠️ 环境要求

- GCC / G++
- Make
- Python 3（用于 mockcpp 构建）
- CMake（用于构建第三方库）

## 📝 Make 命令速查

```shell
make help          # 显示所有可用命令
```
