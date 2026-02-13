// Framework data for the Remotion presentation (Chinese)

export interface Framework {
  id: string;
  name: string;
  language: 'C' | 'C++';
  mockType: string;
  description: string;
  installCmd: string;
  features: string[];
  pros: string[];
  cons: string[];
  bestFor: string;
  codeSnippet: string;
}

export const FRAMEWORKS: Framework[] = [
  {
    id: 'cmocka',
    name: 'CMocka',
    language: 'C',
    mockType: '链接时 --wrap',
    description: '"只做一件事, 但做得好" — 专为 C 语言设计的轻量级单元测试框架',
    installCmd: 'sudo apt install cmocka',
    features: [
      '仅依赖标准 C 库',
      '支持 Mock 对象 (--wrap)',
      '支持 Test Fixtures',
      '信号异常处理 (SIGSEGV)',
      '多种输出格式 (XML/TAP/Subunit)',
    ],
    pros: ['零依赖, 极致轻量', '适合嵌入式开发', '标准 JUnit XML 报告'],
    cons: ['Mock 需要链接器选项', '无自动参数匹配'],
    bestFor: '嵌入式 / 纯 C 项目',
    codeSnippet: `// Mock 函数: 使用 __wrap_ 前缀
int __wrap_calc_add(int a, int b) {
    (void)a; (void)b;
    return mock_type(int);  // 返回预设值
}

// 测试函数
static void test_with_mock(void **state) {
    will_return(__wrap_calc_add, 100);
    int result = multi_calc_expression(2, 3, 10, 4);
    assert_int_equal(result, expected);
}

// Makefile: -Wl,--wrap=calc_add`,
  },
  {
    id: 'unity',
    name: 'Unity + fff',
    language: 'C',
    mockType: '函数指针替换',
    description: '极致轻量的 C 语言测试框架, 配合 fff 实现函数级 Mock',
    installCmd: '// 单头文件, 直接复制到项目中\n#include "unity.h"\n#include "fff.h"',
    features: [
      '单头文件, 零构建依赖',
      'fff: 一行宏生成 Fake 函数',
      '丰富的 TEST_ASSERT 宏',
      '支持 setUp/tearDown',
    ],
    pros: ['极致可移植', '微控制器友好', '代码可读性极高'],
    cons: ['无自动参数匹配', '需手动管理 Fake 函数'],
    bestFor: '资源受限的嵌入式环境',
    codeSnippet: `DEFINE_FFF_GLOBALS;
// 一行宏生成 Fake 函数
FAKE_VALUE_FUNC(int, calc_add, int, int);

void test_add(void) {
    calc_add_fake.return_val = 10;

    int result = app_sum(5, 5);
    TEST_ASSERT_EQUAL(10, result);

    // 验证调用次数
    TEST_ASSERT_EQUAL(1, calc_add_fake.call_count);
}`,
  },
  {
    id: 'gtest',
    name: 'GoogleTest + GMock',
    language: 'C++',
    mockType: '--wrap / 虚函数',
    description: '业界标准的 C++ 测试框架, GMock 提供强大的 Mock 能力',
    installCmd: `# 从源码编译 (推荐)
git clone https://github.com/google/googletest
cd googletest && mkdir build && cd build
cmake .. && make && sudo make install`,
    features: [
      '丰富的断言 (EXPECT/ASSERT)',
      'GMock: 自动参数匹配 Matchers',
      '自动调用次数验证',
      '参数化测试 (TEST_P)',
      '测试发现 & 过滤',
    ],
    pros: ['功能最全面', '生态系统庞大', '文档详尽'],
    cons: ['学习曲线较陡', '编译速度较慢', '构建配置复杂'],
    bestFor: '大型 C++ 项目 / 企业级开发',
    codeSnippet: `class MockCalc : public CalcInterface {
public:
    MOCK_METHOD(int, Add, (int a, int b), (override));
};

TEST(CalcTest, MockAdd) {
    MockCalc mock;
    // 自动参数匹配 + 调用次数验证
    EXPECT_CALL(mock, Add(1, 2))
        .Times(1)
        .WillOnce(Return(3));

    ASSERT_EQ(3, mock.Add(1, 2));
}`,
  },
  {
    id: 'gtest_mockcpp',
    name: 'GoogleTest + MockCpp',
    language: 'C++',
    mockType: '运行时 Hook',
    description: '非侵入式 Mock: 运行时劫持任意函数, 无需修改被测代码',
    installCmd: `# 需要 Python3 + CMake 构建
cd 3rdparty/mockcpp
mkdir build && cd build
cmake .. && make`,
    features: [
      '运行时函数劫持',
      '可 Mock 非虚函数',
      '可直接 Mock C 函数',
      '无需链接器选项',
    ],
    pros: ['非侵入式', '可 Mock 任意函数', '不需要 --wrap'],
    cons: ['依赖平台特性', '项目维护不活跃'],
    bestFor: '无法修改源码的遗留系统',
    codeSnippet: `TEST(CalcTest, MockAdd) {
    // 运行时劫持 calc_add 函数
    MOCKER(calc_add)
        .stubs()
        .will(returnValue(10));

    // calc_add 被劫持, 返回 10
    ASSERT_EQ(10, calc_add(1, 1));

    // 清除 Mock
    GlobalMockObject::verify();
}`,
  },
  {
    id: 'cpputest',
    name: 'CppUTest',
    language: 'C++',
    mockType: '--wrap + Mock API',
    description: '内置内存泄漏检测的成熟 C/C++ 测试框架',
    installCmd: `sudo apt install cpputest
# 或从源码:
git clone https://github.com/cpputest/cpputest
cd cpputest && mkdir build && cd build
cmake .. && make && sudo make install`,
    features: [
      '内置内存泄漏检测 (自动)',
      '同时支持 C 和 C++',
      'MockSupport API',
      '自动参数/调用次数验证',
    ],
    pros: ['内存泄漏自动检测', '成熟稳定', 'C/C++ 混合项目友好'],
    cons: ['API 风格较老', '社区不如 GTest 活跃'],
    bestFor: '对内存安全要求高的项目',
    codeSnippet: `TEST_GROUP(MemoryGroup) {
    void teardown() {
        // 内存泄漏自动检测!
        // 如有未释放内存, 测试自动失败
    }
};

TEST(MemoryGroup, LeakDetection) {
    // 这个测试会自动失败:
    // malloc 了但没有 free
    char* str = (char*)malloc(10);
    // 忘记 free(str) → 测试失败!
}`,
  },
  {
    id: 'check',
    name: 'Check',
    language: 'C',
    mockType: '链接时 --wrap',
    description: '独特的 Fork 隔离机制, 每个测试在独立进程运行',
    installCmd: `sudo apt install check
# 或从源码编译:
git clone https://github.com/libcheck/check
cd check && mkdir build && cd build
autoreconf --install .. && ../configure
make && make install`,
    features: [
      'Fork 隔离 (测试崩溃不影响框架)',
      '信号测试 (SIGSEGV, SIGFPE)',
      '退出值测试 (exit code)',
      '循环测试 (内置参数化)',
      '超时控制',
      'checkmk 代码生成工具',
    ],
    pros: ['Segfault 防护', '信号/退出码验证', '简化语法生成'],
    cons: ['依赖 fork (不支持 Windows)', 'Mock 需要 --wrap'],
    bestFor: '需要测试崩溃/信号的系统级代码',
    codeSnippet: `// checkmk 简化语法 (.ts 文件)
#suite CalcSuite
#tcase AddTests

#test test_add_positive
    ck_assert_int_eq(calc_add(2, 3), 5);

// 信号测试: 期望触发 SIGSEGV
#test-signal(SIGSEGV) test_null_ptr
    int *p = NULL;
    *p = 42;  // 测试通过!

// 循环测试: _i 从 0 到 99
#test-loop(0, 100) test_array
    ck_assert_int_ge(_i, 0);`,
  },
  {
    id: 'catch2',
    name: 'Catch2',
    language: 'C++',
    mockType: '链接时 --wrap',
    description: '现代 C++ 测试框架, SECTION 机制和 BDD 语法让测试更优雅',
    installCmd: `# v2: 单头文件
#include "catch.hpp"
# v3: 需要编译
git clone https://github.com/catchorg/Catch2
cd Catch2 && cmake -B build && cmake --build build`,
    features: [
      'SECTION 嵌套测试 (状态自动重置)',
      'BDD 语法 (SCENARIO/GIVEN/WHEN/THEN)',
      '丰富的 Matcher 表达式',
      'Tags 标签分组',
    ],
    pros: ['语法极其优雅', '零配置即可使用', '嵌套 SECTION 隔离'],
    cons: ['v2 编译速度慢', '无内置 Mock'],
    bestFor: '追求开发体验的现代 C++ 项目',
    codeSnippet: `TEST_CASE("计算器测试", "[calc]") {
    int x = 0;

    SECTION("加法") {
        x = calc_add(1, 2);
        REQUIRE(x == 3);
    }
    // x 自动重置为 0!
    SECTION("减法") {
        x = calc_subtract(5, 3);
        REQUIRE(x == 2);
    }
}

// BDD 语法
SCENARIO("用户计算") {
    GIVEN("两个正整数") {
        WHEN("相加") {
            THEN("结果正确") {
                REQUIRE(calc_add(1, 2) == 3);
            }
        }
    }
}`,
  },
  {
    id: 'doctest',
    name: 'doctest',
    language: 'C++',
    mockType: '链接时 --wrap',
    description: '最快编译速度的 C++ 测试框架, 可嵌入生产代码',
    installCmd: `// 单头文件, 直接包含
#include "doctest.h"
// 编译速度比 Catch2 快 10x+`,
    features: [
      '编译速度最快 (比 Catch2 快 10x+)',
      '可嵌入生产代码 (DOCTEST_CONFIG_DISABLE)',
      'SUBCASE 机制 (类似 SECTION)',
      'BDD 语法支持',
      'Decorators 装饰器',
      '线程安全',
    ],
    pros: ['编译速度碾压级', '可禁用测试代码', '线程安全'],
    cons: ['生态不如 Catch2', '无内置 Mock'],
    bestFor: '对编译速度敏感 / 需要嵌入式测试的项目',
    codeSnippet: `TEST_CASE("快速加法测试") {
    CHECK(calc_add(1, 1) == 2);

    SUBCASE("子用例 - 负数") {
        CHECK(calc_add(-1, -1) == -2);
    }
    SUBCASE("子用例 - 零") {
        CHECK(calc_add(0, 0) == 0);
    }
}

// 可嵌入到生产代码中!
// 发布时定义 DOCTEST_CONFIG_DISABLE
// 所有测试代码自动消失, 零开销`,
  },
];

// Full comparison matrix features (from README.md)
export interface ComparisonRow {
  feature: string;
  values: Record<string, string>;
}

export const COMPARISON_FEATURES: ComparisonRow[] = [
  {
    feature: '语言',
    values: { cmocka: 'C', unity: 'C', gtest: 'C++', gtest_mockcpp: 'C++', cpputest: 'C++', check: 'C', catch2: 'C++', doctest: 'C++' },
  },
  {
    feature: 'Mock 机制',
    values: { cmocka: '--wrap', unity: '函数指针', gtest: '--wrap', gtest_mockcpp: '运行时Hook', cpputest: '--wrap+API', check: '--wrap', catch2: '--wrap', doctest: '--wrap' },
  },
  {
    feature: '无需链接选项',
    values: { cmocka: '❌', unity: '✅', gtest: '❌', gtest_mockcpp: '✅', cpputest: '❌', check: '❌', catch2: '❌', doctest: '❌' },
  },
  {
    feature: '自动参数匹配',
    values: { cmocka: '手动', unity: '手动', gtest: '✅ 自动', gtest_mockcpp: '✅ 自动', cpputest: '✅ 自动', check: '手动', catch2: '手动', doctest: '手动' },
  },
  {
    feature: '调用次数验证',
    values: { cmocka: '手动', unity: '手动', gtest: '✅ 自动', gtest_mockcpp: '✅ 自动', cpputest: '✅ 自动', check: '手动', catch2: '手动', doctest: '手动' },
  },
  {
    feature: '内存泄漏检测',
    values: { cmocka: '❌', unity: '❌', gtest: '❌', gtest_mockcpp: '❌', cpputest: '✅ 内置', check: '❌', catch2: '❌', doctest: '❌' },
  },
  {
    feature: 'Fork 隔离',
    values: { cmocka: '⚠️ 可选', unity: '❌', gtest: '❌', gtest_mockcpp: '❌', cpputest: '❌', check: '✅ 默认', catch2: '❌', doctest: '❌' },
  },
  {
    feature: '信号/退出测试',
    values: { cmocka: '❌', unity: '❌', gtest: '❌', gtest_mockcpp: '❌', cpputest: '❌', check: '✅', catch2: '❌', doctest: '❌' },
  },
  {
    feature: 'SECTION/SUBCASE',
    values: { cmocka: '❌', unity: '❌', gtest: '❌', gtest_mockcpp: '❌', cpputest: '❌', check: '❌', catch2: '✅', doctest: '✅' },
  },
  {
    feature: 'BDD 语法',
    values: { cmocka: '❌', unity: '❌', gtest: '❌', gtest_mockcpp: '❌', cpputest: '❌', check: '❌', catch2: '✅ 内置', doctest: '✅ 内置' },
  },
  {
    feature: '代码生成工具',
    values: { cmocka: '❌', unity: '❌', gtest: '❌', gtest_mockcpp: '❌', cpputest: '❌', check: '✅ checkmk', catch2: '❌', doctest: '❌' },
  },
  {
    feature: '学习曲线',
    values: { cmocka: '低', unity: '低', gtest: '中', gtest_mockcpp: '中', cpputest: '中', check: '低', catch2: '低', doctest: '低' },
  },
];

// Slide timestamps (30fps)
// 20 slides: cover, contenders, matrix, 8 frameworks×2, outro
export const SLIDE_TIMESTAMPS = [
  0,    // 1.  封面           (3s)
  90,   // 2.  选手一览       (3s)
  180,  // 3.  对比矩阵       (8s)
  420,  // 4.  CMocka 概览    (4s)
  540,  // 5.  CMocka 代码    (4s)
  660,  // 6.  Unity 概览     (4s)
  780,  // 7.  Unity 代码     (4s)
  900,  // 8.  GTest 概览     (4s)
  1020, // 9.  GTest 代码     (4s)
  1140, // 10. MockCpp 概览   (4s)
  1260, // 11. MockCpp 代码   (4s)
  1380, // 12. CppUTest 概览  (4s)
  1500, // 13. CppUTest 代码  (4s)
  1620, // 14. Check 概览     (4s)
  1740, // 15. Check 代码     (4s)
  1860, // 16. Catch2 概览    (4s)
  1980, // 17. Catch2 代码    (4s)
  2100, // 18. doctest 概览   (4s)
  2220, // 19. doctest 代码   (4s)
  2340, // 20. 总结           (4s)
];
