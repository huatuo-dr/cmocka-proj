# Build rules for doctest unit tests
# doctest is a header-only library, no linking required

# Compiler
CXX := g++
CXXFLAGS := -Wall -Wextra -g

# doctest directories
DOCTEST_DIR := ut_doctest
DOCTEST_SRC_DIR := $(DOCTEST_DIR)/src
DOCTEST_INC_DIR := $(DOCTEST_DIR)/doctest-install/include

# Output directories
DOCTEST_OUTPUT_DIR := $(OUTPUT_DIR)/ut_doctest
DOCTEST_REPORT_DIR := $(BUILD_DIR)/ut-doctest-report

# Test source files
DOCTEST_SRCS := $(wildcard $(DOCTEST_SRC_DIR)/*.cpp)

# Test executables
DOCTEST_TEST_CALC := $(DOCTEST_OUTPUT_DIR)/doctest_test_calc
DOCTEST_TEST_GREETING := $(DOCTEST_OUTPUT_DIR)/doctest_test_greeting
DOCTEST_TEST_MULTI_CALC := $(DOCTEST_OUTPUT_DIR)/doctest_test_multi_calc

# UT specific flags (header-only, no library linking needed)
DOCTEST_UT_CXXFLAGS := $(CXXFLAGS) -std=c++11 -I$(SDK_INSTALL_INC_DIR) -I$(DOCTEST_INC_DIR)
DOCTEST_UT_LDFLAGS := -L$(SDK_INSTALL_LIB_DIR) -lsdk

# Mock test specific LDFLAGS (with --wrap options)
DOCTEST_MOCK_LDFLAGS := $(DOCTEST_UT_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# Build and run all doctest tests
.PHONY: ut_doctest
ut_doctest: ut_doctest_run ut_doctest_report
	@echo ""
	@echo "========================================"
	@echo "doctest Unit Tests Completed!"
	@echo "  Report: $(DOCTEST_REPORT_DIR)/report.html"
	@echo "========================================"

# Build all doctest test executables
.PHONY: ut_doctest_build
ut_doctest_build: sdk_install $(DOCTEST_TEST_CALC) $(DOCTEST_TEST_GREETING) $(DOCTEST_TEST_MULTI_CALC)
	@echo "doctest test executables built successfully"

# Run all doctest tests (terminal output)
.PHONY: ut_doctest_run
ut_doctest_run: ut_doctest_build
	@echo ""
	@echo "========================================"
	@echo "Running doctest Unit Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running doctest_test_calc ---"
	@$(DOCTEST_TEST_CALC)
	@echo ""
	@echo "--- Running doctest_test_greeting ---"
	@$(DOCTEST_TEST_GREETING)
	@echo ""
	@echo "--- Running doctest_test_multi_calc ---"
	@$(DOCTEST_TEST_MULTI_CALC)

# Generate test reports (JUnit XML -> HTML)
.PHONY: ut_doctest_report
ut_doctest_report: ut_doctest_build
	@echo ""
	@echo "========================================"
	@echo "Generating doctest Test Reports..."
	@echo "========================================"
	@$(MKDIR) $(DOCTEST_REPORT_DIR)
	@echo "Generating XML reports..."
	@$(DOCTEST_TEST_CALC) --reporters=junit --out=$(DOCTEST_REPORT_DIR)/test_calc.xml || true
	@$(DOCTEST_TEST_GREETING) --reporters=junit --out=$(DOCTEST_REPORT_DIR)/test_greeting.xml || true
	@$(DOCTEST_TEST_MULTI_CALC) --reporters=junit --out=$(DOCTEST_REPORT_DIR)/test_multi_calc.xml || true
	@echo "Merging XML reports..."
	@echo '<?xml version="1.0" encoding="UTF-8"?>' > $(DOCTEST_REPORT_DIR)/combined.xml
	@echo '<testsuites>' >> $(DOCTEST_REPORT_DIR)/combined.xml
	@for f in $(DOCTEST_REPORT_DIR)/test_*.xml; do \
		sed -n '/<testsuite /,/<\/testsuite>/p' $$f >> $(DOCTEST_REPORT_DIR)/combined.xml 2>/dev/null || true; \
	done
	@echo '</testsuites>' >> $(DOCTEST_REPORT_DIR)/combined.xml
	@echo "Converting to HTML..."
	@junit2html $(DOCTEST_REPORT_DIR)/combined.xml $(DOCTEST_REPORT_DIR)/report.html
	@echo ""
	@echo "Test report generated:"
	@echo "  $(DOCTEST_REPORT_DIR)/report.html"

# Build doctest_test_calc
$(DOCTEST_TEST_CALC): $(DOCTEST_SRC_DIR)/test_calc.cpp | sdk_install
	@echo "Building test: $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $(DOCTEST_UT_CXXFLAGS) $< -o $@ $(DOCTEST_UT_LDFLAGS)

# Build doctest_test_greeting
$(DOCTEST_TEST_GREETING): $(DOCTEST_SRC_DIR)/test_greeting.cpp | sdk_install
	@echo "Building test: $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $(DOCTEST_UT_CXXFLAGS) $< -o $@ $(DOCTEST_UT_LDFLAGS)

# Build doctest_test_multi_calc (with mock)
$(DOCTEST_TEST_MULTI_CALC): $(DOCTEST_SRC_DIR)/test_multi_calc.cpp | sdk_install
	@echo "Building test (with mock): $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $(DOCTEST_UT_CXXFLAGS) $< -o $@ $(DOCTEST_MOCK_LDFLAGS)

# Clean doctest artifacts
.PHONY: clean-doctest
clean-doctest:
	$(RM) $(DOCTEST_OUTPUT_DIR) $(DOCTEST_REPORT_DIR)
