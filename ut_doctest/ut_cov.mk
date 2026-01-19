# Coverage build rules for doctest unit tests
# Uses gcov + lcov + genhtml to generate HTML coverage reports

# Coverage directories
DOCTEST_COV_OUTPUT_DIR := $(OUTPUT_DIR)/coverage_doctest
DOCTEST_COV_SDK_OUTPUT_DIR := $(DOCTEST_COV_OUTPUT_DIR)/sdk
DOCTEST_COV_UT_OUTPUT_DIR := $(DOCTEST_COV_OUTPUT_DIR)/ut
DOCTEST_COV_REPORT_DIR := $(BUILD_DIR)/coverage-doctest-report

# Coverage flags
DOCTEST_COV_CFLAGS := $(CFLAGS) --coverage -fprofile-arcs -ftest-coverage
DOCTEST_COV_CXXFLAGS := $(CXXFLAGS) -std=c++11 --coverage -fprofile-arcs -ftest-coverage
DOCTEST_COV_LDFLAGS := --coverage

# Coverage SDK object files
DOCTEST_COV_SDK_SRCS := $(wildcard sdk/src/*.c)
DOCTEST_COV_SDK_OBJS := $(patsubst sdk/src/%.c, $(DOCTEST_COV_SDK_OUTPUT_DIR)/%.o, $(DOCTEST_COV_SDK_SRCS))

# Coverage UT object files
DOCTEST_COV_UT_SRCS := $(wildcard $(DOCTEST_SRC_DIR)/*.cpp)
DOCTEST_COV_UT_OBJS := $(patsubst $(DOCTEST_SRC_DIR)/%.cpp, $(DOCTEST_COV_UT_OUTPUT_DIR)/%.o, $(DOCTEST_COV_UT_SRCS))

# Coverage test executables
DOCTEST_COV_TEST_CALC := $(DOCTEST_COV_OUTPUT_DIR)/doctest_test_calc
DOCTEST_COV_TEST_GREETING := $(DOCTEST_COV_OUTPUT_DIR)/doctest_test_greeting
DOCTEST_COV_TEST_MULTI_CALC := $(DOCTEST_COV_OUTPUT_DIR)/doctest_test_multi_calc

# Coverage SDK library
DOCTEST_COV_SDK_LIB := $(DOCTEST_COV_OUTPUT_DIR)/libsdk_cov.a

# UT specific flags for coverage build
DOCTEST_COV_UT_CXXFLAGS := $(DOCTEST_COV_CXXFLAGS) -Isdk/include -I$(DOCTEST_INC_DIR)
DOCTEST_COV_UT_LDFLAGS := $(DOCTEST_COV_LDFLAGS) -L$(DOCTEST_COV_OUTPUT_DIR) -lsdk_cov

# Mock test specific LDFLAGS for coverage
DOCTEST_COV_MOCK_LDFLAGS := $(DOCTEST_COV_UT_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# Build and run coverage tests, then generate report
.PHONY: ut_doctest_cov
ut_doctest_cov: ut_doctest_cov_run ut_doctest_cov_report
	@echo ""
	@echo "========================================"
	@echo "doctest Coverage Report Generated!"
	@echo "  HTML: $(DOCTEST_COV_REPORT_DIR)/index.html"
	@echo "========================================"

# Run coverage tests
.PHONY: ut_doctest_cov_run
ut_doctest_cov_run: ut_doctest_cov_build
	@echo ""
	@echo "========================================"
	@echo "Running doctest Coverage Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running doctest_test_calc (coverage) ---"
	@$(DOCTEST_COV_TEST_CALC)
	@echo ""
	@echo "--- Running doctest_test_greeting (coverage) ---"
	@$(DOCTEST_COV_TEST_GREETING)
	@echo ""
	@echo "--- Running doctest_test_multi_calc (coverage) ---"
	@$(DOCTEST_COV_TEST_MULTI_CALC)

# Generate coverage report using lcov and genhtml
.PHONY: ut_doctest_cov_report
ut_doctest_cov_report: ut_doctest_cov_run
	@echo ""
	@echo "========================================"
	@echo "Generating doctest Coverage Report..."
	@echo "========================================"
	@$(MKDIR) $(DOCTEST_COV_REPORT_DIR)
	@echo "Collecting coverage data..."
	@lcov --capture --directory $(DOCTEST_COV_OUTPUT_DIR) --output-file $(DOCTEST_COV_REPORT_DIR)/coverage.info
	@echo "Filtering out system headers and doctest library..."
	@lcov --remove $(DOCTEST_COV_REPORT_DIR)/coverage.info '/usr/*' '*/doctest/*' --output-file $(DOCTEST_COV_REPORT_DIR)/coverage_filtered.info
	@echo "Generating HTML report..."
	@genhtml $(DOCTEST_COV_REPORT_DIR)/coverage_filtered.info --output-directory $(DOCTEST_COV_REPORT_DIR) --title "doctest SDK Coverage Report" --legend
	@echo ""
	@echo "Coverage report generated:"
	@echo "  $(DOCTEST_COV_REPORT_DIR)/index.html"

# Build coverage SDK library
$(DOCTEST_COV_SDK_LIB): $(DOCTEST_COV_SDK_OBJS)
	@echo "Building coverage SDK library: $@"
	$(AR) $(ARFLAGS) $@ $^

# Compile SDK source files with coverage (C files)
$(DOCTEST_COV_SDK_OUTPUT_DIR)/%.o: sdk/src/%.c
	@echo "Compiling (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(DOCTEST_COV_CFLAGS) -Isdk/include -c $< -o $@

# Build coverage test executables
.PHONY: ut_doctest_cov_build
ut_doctest_cov_build: $(DOCTEST_COV_SDK_LIB) $(DOCTEST_COV_TEST_CALC) $(DOCTEST_COV_TEST_GREETING) $(DOCTEST_COV_TEST_MULTI_CALC)
	@echo "doctest coverage test executables built successfully"

# Build coverage doctest_test_calc
$(DOCTEST_COV_TEST_CALC): $(DOCTEST_COV_UT_OUTPUT_DIR)/test_calc.o $(DOCTEST_COV_SDK_LIB)
	@echo "Building coverage test: $@"
	$(CXX) $< -o $@ $(DOCTEST_COV_UT_LDFLAGS)

# Build coverage doctest_test_greeting
$(DOCTEST_COV_TEST_GREETING): $(DOCTEST_COV_UT_OUTPUT_DIR)/test_greeting.o $(DOCTEST_COV_SDK_LIB)
	@echo "Building coverage test: $@"
	$(CXX) $< -o $@ $(DOCTEST_COV_UT_LDFLAGS)

# Build coverage doctest_test_multi_calc (with mock)
$(DOCTEST_COV_TEST_MULTI_CALC): $(DOCTEST_COV_UT_OUTPUT_DIR)/test_multi_calc.o $(DOCTEST_COV_SDK_LIB)
	@echo "Building coverage test (with mock): $@"
	$(CXX) $< -o $@ $(DOCTEST_COV_MOCK_LDFLAGS)

# Compile UT source files with coverage (C++ files)
$(DOCTEST_COV_UT_OUTPUT_DIR)/%.o: $(DOCTEST_SRC_DIR)/%.cpp
	@echo "Compiling test (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CXX) $(DOCTEST_COV_UT_CXXFLAGS) -c $< -o $@

# Clean doctest coverage artifacts
.PHONY: clean-doctest-cov
clean-doctest-cov:
	$(RM) $(DOCTEST_COV_OUTPUT_DIR) $(DOCTEST_COV_REPORT_DIR)
	@find . -name "*.gcno" -path "*/coverage_doctest/*" -delete 2>/dev/null || true
	@find . -name "*.gcda" -path "*/coverage_doctest/*" -delete 2>/dev/null || true
	@find . -name "*.gcov" -path "*/coverage_doctest/*" -delete 2>/dev/null || true
