# Coverage build rules for Catch2 unit tests
# Uses gcov + lcov + genhtml to generate HTML coverage reports

# Coverage directories
CATCH2_COV_OUTPUT_DIR := $(OUTPUT_DIR)/coverage_catch2
CATCH2_COV_SDK_OUTPUT_DIR := $(CATCH2_COV_OUTPUT_DIR)/sdk
CATCH2_COV_UT_OUTPUT_DIR := $(CATCH2_COV_OUTPUT_DIR)/ut
CATCH2_COV_REPORT_DIR := $(BUILD_DIR)/coverage-catch2-report

# Coverage flags
CATCH2_COV_CFLAGS := $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage
CATCH2_COV_CXXFLAGS := $(CXXFLAGS) -std=c++17 --coverage -fprofile-arcs -ftest-coverage
CATCH2_COV_LDFLAGS := --coverage

# Coverage SDK object files
CATCH2_COV_SDK_SRCS := $(wildcard sdk/src/*.c)
CATCH2_COV_SDK_OBJS := $(patsubst sdk/src/%.c, $(CATCH2_COV_SDK_OUTPUT_DIR)/%.o, $(CATCH2_COV_SDK_SRCS))

# Coverage UT object files
CATCH2_COV_UT_SRCS := $(wildcard $(CATCH2_SRC_DIR)/*.cpp)
CATCH2_COV_UT_OBJS := $(patsubst $(CATCH2_SRC_DIR)/%.cpp, $(CATCH2_COV_UT_OUTPUT_DIR)/%.o, $(CATCH2_COV_UT_SRCS))

# Coverage test executables
CATCH2_COV_TEST_CALC := $(CATCH2_COV_OUTPUT_DIR)/catch2_test_calc
CATCH2_COV_TEST_GREETING := $(CATCH2_COV_OUTPUT_DIR)/catch2_test_greeting
CATCH2_COV_TEST_MULTI_CALC := $(CATCH2_COV_OUTPUT_DIR)/catch2_test_multi_calc

# Coverage SDK library
CATCH2_COV_SDK_LIB := $(CATCH2_COV_OUTPUT_DIR)/libsdk_cov.a

# UT specific flags for coverage build
CATCH2_COV_UT_CXXFLAGS := $(CATCH2_COV_CXXFLAGS) -Isdk/include -I$(CATCH2_INC_DIR)
CATCH2_COV_UT_LDFLAGS := $(CATCH2_COV_LDFLAGS) -L$(CATCH2_COV_OUTPUT_DIR) -L$(CATCH2_LIB_DIR) -lsdk_cov -lCatch2Main -lCatch2

# Mock test specific LDFLAGS for coverage
CATCH2_COV_MOCK_LDFLAGS := $(CATCH2_COV_UT_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# Build and run coverage tests, then generate report
.PHONY: ut_catch2_cov
ut_catch2_cov: ut_catch2_cov_run ut_catch2_cov_report
	@echo ""
	@echo "========================================"
	@echo "Catch2 Coverage Report Generated!"
	@echo "  HTML: $(CATCH2_COV_REPORT_DIR)/index.html"
	@echo "========================================"

# Run coverage tests
.PHONY: ut_catch2_cov_run
ut_catch2_cov_run: ut_catch2_cov_build
	@echo ""
	@echo "========================================"
	@echo "Running Catch2 Coverage Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running catch2_test_calc (coverage) ---"
	@$(CATCH2_COV_TEST_CALC)
	@echo ""
	@echo "--- Running catch2_test_greeting (coverage) ---"
	@$(CATCH2_COV_TEST_GREETING)
	@echo ""
	@echo "--- Running catch2_test_multi_calc (coverage) ---"
	@$(CATCH2_COV_TEST_MULTI_CALC)

# Generate coverage report using lcov and genhtml
.PHONY: ut_catch2_cov_report
ut_catch2_cov_report: ut_catch2_cov_run
	@echo ""
	@echo "========================================"
	@echo "Generating Catch2 Coverage Report..."
	@echo "========================================"
	@$(MKDIR) $(CATCH2_COV_REPORT_DIR)
	@echo "Collecting coverage data..."
	@lcov --capture --directory $(CATCH2_COV_OUTPUT_DIR) --output-file $(CATCH2_COV_REPORT_DIR)/coverage.info
	@echo "Filtering out system headers and catch2 library..."
	@lcov --remove $(CATCH2_COV_REPORT_DIR)/coverage.info '/usr/*' '*/catch2-install/*' --output-file $(CATCH2_COV_REPORT_DIR)/coverage_filtered.info
	@echo "Generating HTML report..."
	@genhtml $(CATCH2_COV_REPORT_DIR)/coverage_filtered.info --output-directory $(CATCH2_COV_REPORT_DIR) --title "Catch2 SDK Coverage Report" --legend
	@echo ""
	@echo "Coverage report generated:"
	@echo "  $(CATCH2_COV_REPORT_DIR)/index.html"

# Build coverage SDK library
$(CATCH2_COV_SDK_LIB): $(CATCH2_COV_SDK_OBJS)
	@echo "Building coverage SDK library: $@"
	$(AR) $(ARFLAGS) $@ $^

# Compile SDK source files with coverage (C files)
$(CATCH2_COV_SDK_OUTPUT_DIR)/%.o: sdk/src/%.c
	@echo "Compiling (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CATCH2_COV_CFLAGS) -Isdk/include -c $< -o $@

# Build coverage test executables
.PHONY: ut_catch2_cov_build
ut_catch2_cov_build: $(CATCH2_COV_SDK_LIB) $(CATCH2_COV_TEST_CALC) $(CATCH2_COV_TEST_GREETING) $(CATCH2_COV_TEST_MULTI_CALC)
	@echo "Catch2 coverage test executables built successfully"

# Build coverage catch2_test_calc
$(CATCH2_COV_TEST_CALC): $(CATCH2_COV_UT_OUTPUT_DIR)/test_calc.o $(CATCH2_COV_SDK_LIB)
	@echo "Building coverage test: $@"
	$(CXX) $< -o $@ $(CATCH2_COV_UT_LDFLAGS)

# Build coverage catch2_test_greeting
$(CATCH2_COV_TEST_GREETING): $(CATCH2_COV_UT_OUTPUT_DIR)/test_greeting.o $(CATCH2_COV_SDK_LIB)
	@echo "Building coverage test: $@"
	$(CXX) $< -o $@ $(CATCH2_COV_UT_LDFLAGS)

# Build coverage catch2_test_multi_calc (with mock)
$(CATCH2_COV_TEST_MULTI_CALC): $(CATCH2_COV_UT_OUTPUT_DIR)/test_multi_calc.o $(CATCH2_COV_SDK_LIB)
	@echo "Building coverage test (with mock): $@"
	$(CXX) $< -o $@ $(CATCH2_COV_MOCK_LDFLAGS)

# Compile UT source files with coverage (C++ files)
$(CATCH2_COV_UT_OUTPUT_DIR)/%.o: $(CATCH2_SRC_DIR)/%.cpp
	@echo "Compiling test (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CXX) $(CATCH2_COV_UT_CXXFLAGS) -c $< -o $@

# Clean Catch2 coverage artifacts
.PHONY: clean-catch2-cov
clean-catch2-cov:
	$(RM) $(CATCH2_COV_OUTPUT_DIR) $(CATCH2_COV_REPORT_DIR)
	@find . -name "*.gcno" -path "*/coverage_catch2/*" -delete 2>/dev/null || true
	@find . -name "*.gcda" -path "*/coverage_catch2/*" -delete 2>/dev/null || true
	@find . -name "*.gcov" -path "*/coverage_catch2/*" -delete 2>/dev/null || true
