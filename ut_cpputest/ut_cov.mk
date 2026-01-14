# Coverage build rules for CppUTest unit tests
# Uses gcov + lcov + genhtml to generate HTML coverage reports

# Coverage directories
CPPUTEST_COV_OUTPUT_DIR := $(OUTPUT_DIR)/coverage_cpputest
CPPUTEST_COV_SDK_OUTPUT_DIR := $(CPPUTEST_COV_OUTPUT_DIR)/sdk
CPPUTEST_COV_UT_OUTPUT_DIR := $(CPPUTEST_COV_OUTPUT_DIR)/ut
CPPUTEST_COV_REPORT_DIR := $(BUILD_DIR)/coverage-cpputest-report

# Coverage flags
CPPUTEST_COV_CFLAGS := $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage
CPPUTEST_COV_CXXFLAGS := $(CPPUTEST_COV_CFLAGS) -std=c++11
CPPUTEST_COV_LDFLAGS := --coverage

# Coverage SDK object files
CPPUTEST_COV_SDK_SRCS := $(wildcard sdk/src/*.c)
CPPUTEST_COV_SDK_OBJS := $(patsubst sdk/src/%.c, $(CPPUTEST_COV_SDK_OUTPUT_DIR)/%.o, $(CPPUTEST_COV_SDK_SRCS))

# Coverage UT object files
CPPUTEST_COV_UT_SRCS := $(wildcard $(CPPUTEST_SRC_DIR)/*.cpp)
CPPUTEST_COV_UT_OBJS := $(patsubst $(CPPUTEST_SRC_DIR)/%.cpp, $(CPPUTEST_COV_UT_OUTPUT_DIR)/%.o, $(CPPUTEST_COV_UT_SRCS))

# Coverage test executables
CPPUTEST_COV_TEST_BASIC := $(CPPUTEST_COV_OUTPUT_DIR)/cpputest_test_basic
CPPUTEST_COV_TEST_MULTI_CALC := $(CPPUTEST_COV_OUTPUT_DIR)/cpputest_test_multi_calc

# Coverage SDK library
CPPUTEST_COV_SDK_LIB := $(CPPUTEST_COV_OUTPUT_DIR)/libsdk_cov.a

# UT specific flags for coverage build
CPPUTEST_COV_UT_CXXFLAGS := $(CPPUTEST_COV_CXXFLAGS) -Isdk/include -I$(CPPUTEST_INC_DIR)
CPPUTEST_COV_UT_LDFLAGS := $(CPPUTEST_COV_LDFLAGS) -L$(CPPUTEST_COV_OUTPUT_DIR) -L$(CPPUTEST_LIB_DIR) -lsdk_cov -lCppUTest -lCppUTestExt

# Mock test specific LDFLAGS for coverage
CPPUTEST_COV_MOCK_LDFLAGS := $(CPPUTEST_COV_UT_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# Coverage object files for basic tests
CPPUTEST_COV_BASIC_OBJS := $(CPPUTEST_COV_UT_OUTPUT_DIR)/main.o \
    $(CPPUTEST_COV_UT_OUTPUT_DIR)/test_calc.o \
    $(CPPUTEST_COV_UT_OUTPUT_DIR)/test_greeting.o \
    $(CPPUTEST_COV_UT_OUTPUT_DIR)/test_memory_leak.o

# Coverage object files for mock tests
CPPUTEST_COV_MOCK_OBJS := $(CPPUTEST_COV_UT_OUTPUT_DIR)/main.o \
    $(CPPUTEST_COV_UT_OUTPUT_DIR)/test_multi_calc.o

# Build and run coverage tests, then generate report
.PHONY: ut_cpputest_cov
ut_cpputest_cov: ut_cpputest_cov_run ut_cpputest_cov_report
	@echo ""
	@echo "========================================"
	@echo "CppUTest Coverage Report Generated!"
	@echo "  HTML: $(CPPUTEST_COV_REPORT_DIR)/index.html"
	@echo "========================================"

# Run coverage tests
.PHONY: ut_cpputest_cov_run
ut_cpputest_cov_run: ut_cpputest_cov_build
	@echo ""
	@echo "========================================"
	@echo "Running CppUTest Coverage Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running cpputest_test_basic (coverage) ---"
	@$(CPPUTEST_COV_TEST_BASIC) -v
	@echo ""
	@echo "--- Running cpputest_test_multi_calc (coverage) ---"
	@$(CPPUTEST_COV_TEST_MULTI_CALC) -v

# Generate coverage report using lcov and genhtml
.PHONY: ut_cpputest_cov_report
ut_cpputest_cov_report: ut_cpputest_cov_run
	@echo ""
	@echo "========================================"
	@echo "Generating CppUTest Coverage Report..."
	@echo "========================================"
	@$(MKDIR) $(CPPUTEST_COV_REPORT_DIR)
	@echo "Collecting coverage data..."
	@lcov --capture --directory $(CPPUTEST_COV_OUTPUT_DIR) --output-file $(CPPUTEST_COV_REPORT_DIR)/coverage.info
	@echo "Filtering out system headers..."
	@lcov --remove $(CPPUTEST_COV_REPORT_DIR)/coverage.info '/usr/*' --output-file $(CPPUTEST_COV_REPORT_DIR)/coverage_filtered.info
	@echo "Generating HTML report..."
	@genhtml $(CPPUTEST_COV_REPORT_DIR)/coverage_filtered.info --output-directory $(CPPUTEST_COV_REPORT_DIR) --title "CppUTest SDK Coverage Report" --legend
	@echo ""
	@echo "Coverage report generated:"
	@echo "  $(CPPUTEST_COV_REPORT_DIR)/index.html"

# Build coverage SDK library
$(CPPUTEST_COV_SDK_LIB): $(CPPUTEST_COV_SDK_OBJS)
	@echo "Building coverage SDK library: $@"
	$(AR) $(ARFLAGS) $@ $^

# Compile SDK source files with coverage
$(CPPUTEST_COV_SDK_OUTPUT_DIR)/%.o: sdk/src/%.c
	@echo "Compiling (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CPPUTEST_COV_CFLAGS) -Isdk/include -c $< -o $@

# Build coverage test executables
.PHONY: ut_cpputest_cov_build
ut_cpputest_cov_build: $(CPPUTEST_COV_SDK_LIB) $(CPPUTEST_COV_TEST_BASIC) $(CPPUTEST_COV_TEST_MULTI_CALC)
	@echo "CppUTest coverage test executables built successfully"

# Build coverage cpputest_test_basic
$(CPPUTEST_COV_TEST_BASIC): $(CPPUTEST_COV_BASIC_OBJS) $(CPPUTEST_COV_SDK_LIB)
	@echo "Building coverage test: $@"
	$(CXX) $(CPPUTEST_COV_BASIC_OBJS) -o $@ $(CPPUTEST_COV_UT_LDFLAGS)

# Build coverage cpputest_test_multi_calc (with mock)
$(CPPUTEST_COV_TEST_MULTI_CALC): $(CPPUTEST_COV_MOCK_OBJS) $(CPPUTEST_COV_SDK_LIB)
	@echo "Building coverage test (with mock): $@"
	$(CXX) $(CPPUTEST_COV_MOCK_OBJS) -o $@ $(CPPUTEST_COV_MOCK_LDFLAGS)

# Compile UT source files with coverage
$(CPPUTEST_COV_UT_OUTPUT_DIR)/%.o: $(CPPUTEST_SRC_DIR)/%.cpp
	@echo "Compiling test (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CXX) $(CPPUTEST_COV_UT_CXXFLAGS) -c $< -o $@

# Clean CppUTest coverage artifacts
.PHONY: clean-cpputest-cov
clean-cpputest-cov:
	$(RM) $(CPPUTEST_COV_OUTPUT_DIR) $(CPPUTEST_COV_REPORT_DIR)
	@find . -name "*.gcno" -path "*/coverage_cpputest/*" -delete 2>/dev/null || true
	@find . -name "*.gcda" -path "*/coverage_cpputest/*" -delete 2>/dev/null || true
	@find . -name "*.gcov" -path "*/coverage_cpputest/*" -delete 2>/dev/null || true
