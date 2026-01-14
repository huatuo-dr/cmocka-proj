# CppUTest unit test build rules

# Compiler for C++ tests
CXX := g++

# CppUTest directories
CPPUTEST_DIR := ut_cpputest/cpputest-install
CPPUTEST_INC_DIR := $(CPPUTEST_DIR)/include
CPPUTEST_LIB_DIR := $(CPPUTEST_DIR)/lib

# UT source files
CPPUTEST_SRC_DIR := ut_cpputest/src
CPPUTEST_SRCS := $(wildcard $(CPPUTEST_SRC_DIR)/*.cpp)
CPPUTEST_OUTPUT_DIR := $(OUTPUT_DIR)/ut_cpputest
CPPUTEST_OBJS := $(patsubst $(CPPUTEST_SRC_DIR)/%.cpp, $(CPPUTEST_OUTPUT_DIR)/%.o, $(CPPUTEST_SRCS))

# UT executables
CPPUTEST_TEST_BASIC := $(DIST_DIR)/cpputest_test_basic
CPPUTEST_TEST_MULTI_CALC := $(DIST_DIR)/cpputest_test_multi_calc

# UT report directory
CPPUTEST_REPORT_DIR := $(BUILD_DIR)/ut-cpputest-report

# UT specific flags (use installed SDK from build directory)
CPPUTEST_CXXFLAGS := $(CFLAGS) -std=c++11 -I$(SDK_INSTALL_INC_DIR) -I$(CPPUTEST_INC_DIR)
CPPUTEST_LDFLAGS := -L$(SDK_INSTALL_LIB_DIR) -L$(CPPUTEST_LIB_DIR) -lsdk -lCppUTest -lCppUTestExt

# Mock test specific LDFLAGS (--wrap options for mocking calc functions)
CPPUTEST_MOCK_LDFLAGS := $(CPPUTEST_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# Object files for basic tests (without mock)
CPPUTEST_BASIC_OBJS := $(CPPUTEST_OUTPUT_DIR)/main.o \
    $(CPPUTEST_OUTPUT_DIR)/test_calc.o \
    $(CPPUTEST_OUTPUT_DIR)/test_greeting.o \
    $(CPPUTEST_OUTPUT_DIR)/test_memory_leak.o

# Object files for mock tests
CPPUTEST_MOCK_OBJS := $(CPPUTEST_OUTPUT_DIR)/main.o \
    $(CPPUTEST_OUTPUT_DIR)/test_multi_calc.o

# Build and run all unit tests
.PHONY: ut_cpputest
ut_cpputest: ut_cpputest_run ut_cpputest_report
	@echo ""
	@echo "========================================"
	@echo "All CppUTest Unit Tests Completed!"
	@echo "  Reports: $(CPPUTEST_REPORT_DIR)/"
	@echo "========================================"

# Run unit tests (output to terminal)
.PHONY: ut_cpputest_run
ut_cpputest_run: ut_cpputest_build
	@echo ""
	@echo "========================================"
	@echo "Running CppUTest Unit Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running cpputest_test_basic ---"
	@$(CPPUTEST_TEST_BASIC) -v
	@echo ""
	@echo "--- Running cpputest_test_multi_calc (Mock Tests) ---"
	@$(CPPUTEST_TEST_MULTI_CALC) -v

# Generate XML reports and HTML report
.PHONY: ut_cpputest_report
ut_cpputest_report: ut_cpputest_build
	@echo ""
	@echo "========================================"
	@echo "Generating CppUTest Test Reports..."
	@echo "========================================"
	@$(MKDIR) $(CPPUTEST_REPORT_DIR)
	@$(CPPUTEST_TEST_BASIC) -ojunit > /dev/null 2>&1 || true
	@mv cpputest_*.xml $(CPPUTEST_REPORT_DIR)/ 2>/dev/null || true
	@$(CPPUTEST_TEST_MULTI_CALC) -ojunit > /dev/null 2>&1 || true
	@mv cpputest_*.xml $(CPPUTEST_REPORT_DIR)/ 2>/dev/null || true
	@echo "Generating HTML report..."
	@cd $(CPPUTEST_REPORT_DIR) && junit2html --merge merged.xml *.xml && junit2html merged.xml report.html
	@echo ""
	@echo "Reports generated:"
	@echo "  XML: $(CPPUTEST_REPORT_DIR)/*.xml"
	@echo "  HTML: $(CPPUTEST_REPORT_DIR)/report.html"

# Build unit tests only (without running)
.PHONY: ut_cpputest_build
ut_cpputest_build: sdk_install $(CPPUTEST_TEST_BASIC) $(CPPUTEST_TEST_MULTI_CALC)
	@echo "CppUTest test executables built successfully"
	@echo "  - $(CPPUTEST_TEST_BASIC)"
	@echo "  - $(CPPUTEST_TEST_MULTI_CALC) (with mock)"

# Build cpputest_test_basic executable (without mock)
$(CPPUTEST_TEST_BASIC): $(CPPUTEST_BASIC_OBJS)
	@echo "Building test executable: $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $^ -o $@ $(CPPUTEST_LDFLAGS)

# Build cpputest_test_multi_calc executable (with --wrap for mocking)
$(CPPUTEST_TEST_MULTI_CALC): $(CPPUTEST_MOCK_OBJS)
	@echo "Building test executable (with mock): $@"
	@$(MKDIR) $(dir $@)
	$(CXX) $^ -o $@ $(CPPUTEST_MOCK_LDFLAGS)

# Compile UT source files
$(CPPUTEST_OUTPUT_DIR)/%.o: $(CPPUTEST_SRC_DIR)/%.cpp
	@echo "Compiling: $<"
	@$(MKDIR) $(dir $@)
	$(CXX) $(CPPUTEST_CXXFLAGS) -c $< -o $@

# Clean CppUTest UT artifacts
.PHONY: clean-ut-cpputest
clean-ut-cpputest:
	$(RM) $(CPPUTEST_OUTPUT_DIR) $(CPPUTEST_REPORT_DIR) $(CPPUTEST_TEST_BASIC) $(CPPUTEST_TEST_MULTI_CALC)
