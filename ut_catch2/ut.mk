# Catch2 unit test build rules

# Catch2 directories
CATCH2_DIR := ut_catch2
CATCH2_INSTALL_DIR := $(CATCH2_DIR)/catch2-install
CATCH2_INC_DIR := $(CATCH2_INSTALL_DIR)/include
CATCH2_LIB_DIR := $(CATCH2_INSTALL_DIR)/lib

# UT source files
CATCH2_SRC_DIR := $(CATCH2_DIR)/src
CATCH2_OUTPUT_DIR := $(OUTPUT_DIR)/ut_catch2

# UT executables
CATCH2_TEST_CALC := $(DIST_DIR)/catch2_test_calc
CATCH2_TEST_GREETING := $(DIST_DIR)/catch2_test_greeting
CATCH2_TEST_MULTI_CALC := $(DIST_DIR)/catch2_test_multi_calc

# UT report directory
CATCH2_REPORT_DIR := $(BUILD_DIR)/ut-catch2-report

# C++ compiler flags
CATCH2_CXXFLAGS := $(CXXFLAGS) -std=c++17 -I$(SDK_INSTALL_INC_DIR) -I$(CATCH2_INC_DIR)
CATCH2_LDFLAGS := -L$(SDK_INSTALL_LIB_DIR) -L$(CATCH2_LIB_DIR) -lsdk -lCatch2Main -lCatch2

# Mock test specific LDFLAGS (--wrap options for mocking calc functions)
CATCH2_MOCK_LDFLAGS := $(CATCH2_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# Build and run all Catch2 tests
.PHONY: ut_catch2
ut_catch2: ut_catch2_run ut_catch2_report
	@echo ""
	@echo "========================================"
	@echo "All Catch2 Tests Completed!"
	@echo "  Reports: $(CATCH2_REPORT_DIR)/"
	@echo "========================================"

# Run Catch2 tests (output to terminal)
.PHONY: ut_catch2_run
ut_catch2_run: ut_catch2_build
	@echo ""
	@echo "========================================"
	@echo "Running Catch2 Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running catch2_test_calc ---"
	@$(CATCH2_TEST_CALC)
	@echo ""
	@echo "--- Running catch2_test_greeting ---"
	@$(CATCH2_TEST_GREETING)
	@echo ""
	@echo "--- Running catch2_test_multi_calc (Mock Tests) ---"
	@$(CATCH2_TEST_MULTI_CALC)

# Generate Catch2 reports (XML + HTML)
.PHONY: ut_catch2_report
ut_catch2_report: ut_catch2_build
	@echo ""
	@echo "========================================"
	@echo "Generating Catch2 Reports..."
	@echo "========================================"
	@$(MKDIR) $(CATCH2_REPORT_DIR)
	@echo "Running tests with JUnit XML output..."
	@$(CATCH2_TEST_CALC) --reporter JUnit --out $(CATCH2_REPORT_DIR)/test_calc.xml || true
	@$(CATCH2_TEST_GREETING) --reporter JUnit --out $(CATCH2_REPORT_DIR)/test_greeting.xml || true
	@$(CATCH2_TEST_MULTI_CALC) --reporter JUnit --out $(CATCH2_REPORT_DIR)/test_multi_calc.xml || true
	@echo "Merging XML reports..."
	@cd $(CATCH2_REPORT_DIR) && junit2html --merge merged.xml *.xml
	@echo "Generating HTML report..."
	@cd $(CATCH2_REPORT_DIR) && junit2html merged.xml report.html
	@echo ""
	@echo "Reports generated:"
	@echo "  XML: $(CATCH2_REPORT_DIR)/*.xml"
	@echo "  HTML: $(CATCH2_REPORT_DIR)/report.html"

# Build Catch2 tests only (without running)
.PHONY: ut_catch2_build
ut_catch2_build: sdk_install $(CATCH2_TEST_CALC) $(CATCH2_TEST_GREETING) $(CATCH2_TEST_MULTI_CALC)
	@echo "Catch2 executables built successfully"
	@echo "  - $(CATCH2_TEST_CALC)"
	@echo "  - $(CATCH2_TEST_GREETING)"
	@echo "  - $(CATCH2_TEST_MULTI_CALC) (with Mock)"

# Build catch2_test_calc executable
$(CATCH2_TEST_CALC): $(CATCH2_OUTPUT_DIR)/test_calc.o
	@echo "Building Catch2 executable: $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $< -o $@ $(CATCH2_LDFLAGS)

# Build catch2_test_greeting executable
$(CATCH2_TEST_GREETING): $(CATCH2_OUTPUT_DIR)/test_greeting.o
	@echo "Building Catch2 executable: $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $< -o $@ $(CATCH2_LDFLAGS)

# Build catch2_test_multi_calc executable (with --wrap for mocking)
$(CATCH2_TEST_MULTI_CALC): $(CATCH2_OUTPUT_DIR)/test_multi_calc.o
	@echo "Building Catch2 executable (with Mock): $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $< -o $@ $(CATCH2_MOCK_LDFLAGS)

# Compile Catch2 source files
$(CATCH2_OUTPUT_DIR)/%.o: $(CATCH2_SRC_DIR)/%.cpp
	@echo "Compiling Catch2: $<"
	@$(MKDIR) $(dir $@)
	$(CXX) $(CATCH2_CXXFLAGS) -c $< -o $@

# Clean Catch2 artifacts
.PHONY: clean-ut-catch2
clean-ut-catch2:
	$(RM) $(CATCH2_OUTPUT_DIR) $(CATCH2_REPORT_DIR) $(CATCH2_TEST_CALC) $(CATCH2_TEST_GREETING) $(CATCH2_TEST_MULTI_CALC)
