# Check unit test build rules

# Check directories
CHECK_DIR := ut_check/check-install
CHECK_INC_DIR := $(CHECK_DIR)/include
CHECK_LIB_DIR := $(CHECK_DIR)/lib
CHECK_BIN_DIR := $(CHECK_DIR)/bin
CHECKMK := $(CHECK_BIN_DIR)/checkmk

# UT source files (.ts files that will be converted to .c)
CHECK_SRC_DIR := ut_check/src
CHECK_TS_FILES := $(wildcard $(CHECK_SRC_DIR)/*.ts)
CHECK_GEN_DIR := $(OUTPUT_DIR)/ut_check/generated
CHECK_OUTPUT_DIR := $(OUTPUT_DIR)/ut_check

# Generated C files from .ts
CHECK_GEN_SRCS := $(patsubst $(CHECK_SRC_DIR)/%.ts, $(CHECK_GEN_DIR)/%.c, $(CHECK_TS_FILES))

# Prevent Make from deleting generated .c files as intermediate files
.PRECIOUS: $(CHECK_GEN_DIR)/%.c

# Object files
CHECK_OBJS := $(patsubst $(CHECK_GEN_DIR)/%.c, $(CHECK_OUTPUT_DIR)/%.o, $(CHECK_GEN_SRCS))

# Mock helpers (hand-written C file)
CHECK_MOCK_HELPER := $(CHECK_SRC_DIR)/mock_helpers.c
CHECK_MOCK_HELPER_OBJ := $(CHECK_OUTPUT_DIR)/mock_helpers.o

# UT executables
CHECK_TEST_CALC := $(DIST_DIR)/check_test_calc
CHECK_TEST_GREETING := $(DIST_DIR)/check_test_greeting
CHECK_TEST_MULTI_CALC := $(DIST_DIR)/check_test_multi_calc
CHECK_TEST_FEATURES := $(DIST_DIR)/check_test_features

# UT report directory
CHECK_REPORT_DIR := $(BUILD_DIR)/ut-check-report

# UT specific flags
CHECK_CFLAGS := $(CFLAGS) -I$(SDK_INSTALL_INC_DIR) -I$(CHECK_INC_DIR)
CHECK_LDFLAGS := -L$(SDK_INSTALL_LIB_DIR) -L$(CHECK_LIB_DIR) -lsdk -lcheck -pthread -lm -lrt

# Mock test specific LDFLAGS (--wrap options for mocking calc functions)
CHECK_MOCK_LDFLAGS := $(CHECK_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# Build and run all unit tests
.PHONY: ut_check
ut_check: ut_check_run ut_check_report
	@echo ""
	@echo "========================================"
	@echo "All Check Unit Tests Completed!"
	@echo "  Reports: $(CHECK_REPORT_DIR)/"
	@echo "========================================"

# Run unit tests (output to terminal)
.PHONY: ut_check_run
ut_check_run: ut_check_build
	@echo ""
	@echo "========================================"
	@echo "Running Check Unit Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running check_test_calc ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_TEST_CALC)
	@echo ""
	@echo "--- Running check_test_greeting ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_TEST_GREETING)
	@echo ""
	@echo "--- Running check_test_multi_calc (Mock Tests) ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_TEST_MULTI_CALC)
	@echo ""
	@echo "--- Running check_test_features (Signal/Exit/Loop Tests) ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_TEST_FEATURES) || true

# Generate XML reports and HTML report
.PHONY: ut_check_report
ut_check_report: ut_check_build
	@echo ""
	@echo "========================================"
	@echo "Generating Check Test Reports..."
	@echo "========================================"
	@$(MKDIR) $(CHECK_REPORT_DIR)
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH \
		CK_XML_FILE=$(CHECK_REPORT_DIR)/test_calc.xml \
		$(CHECK_TEST_CALC) || true
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH \
		CK_XML_FILE=$(CHECK_REPORT_DIR)/test_greeting.xml \
		$(CHECK_TEST_GREETING) || true
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH \
		CK_XML_FILE=$(CHECK_REPORT_DIR)/test_multi_calc.xml \
		$(CHECK_TEST_MULTI_CALC) || true
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH \
		CK_XML_FILE=$(CHECK_REPORT_DIR)/test_features.xml \
		$(CHECK_TEST_FEATURES) || true
	@echo "Generating HTML report..."
	@cd $(CHECK_REPORT_DIR) && junit2html --merge merged.xml *.xml 2>/dev/null && junit2html merged.xml report.html 2>/dev/null || echo "Note: junit2html not available, XML reports generated only"
	@echo ""
	@echo "Reports generated:"
	@echo "  XML: $(CHECK_REPORT_DIR)/*.xml"
	@echo "  HTML: $(CHECK_REPORT_DIR)/report.html (if junit2html available)"

# Build unit tests only (without running)
.PHONY: ut_check_build
ut_check_build: sdk_install $(CHECK_TEST_CALC) $(CHECK_TEST_GREETING) $(CHECK_TEST_MULTI_CALC) $(CHECK_TEST_FEATURES)
	@echo "Check test executables built successfully"
	@echo "  - $(CHECK_TEST_CALC)"
	@echo "  - $(CHECK_TEST_GREETING)"
	@echo "  - $(CHECK_TEST_MULTI_CALC) (with mock)"
	@echo "  - $(CHECK_TEST_FEATURES) (signal/exit/loop tests)"

# Generate C files from .ts files using checkmk
$(CHECK_GEN_DIR)/%.c: $(CHECK_SRC_DIR)/%.ts
	@echo "Generating C from checkmk: $< -> $@"
	@$(MKDIR) $(dir $@)
	$(CHECKMK) $< > $@

# Compile generated C files
$(CHECK_OUTPUT_DIR)/%.o: $(CHECK_GEN_DIR)/%.c
	@echo "Compiling: $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CHECK_CFLAGS) -c $< -o $@

# Compile mock helpers
$(CHECK_MOCK_HELPER_OBJ): $(CHECK_MOCK_HELPER)
	@echo "Compiling mock helpers: $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CHECK_CFLAGS) -c $< -o $@

# Build check_test_calc executable
$(CHECK_TEST_CALC): $(CHECK_OUTPUT_DIR)/test_calc.o
	@echo "Building test executable: $@"
	@$(MKDIR) $(dir $@)
	$(CC) $< -o $@ $(CHECK_LDFLAGS)

# Build check_test_greeting executable
$(CHECK_TEST_GREETING): $(CHECK_OUTPUT_DIR)/test_greeting.o
	@echo "Building test executable: $@"
	@$(MKDIR) $(dir $@)
	$(CC) $< -o $@ $(CHECK_LDFLAGS)

# Build check_test_multi_calc executable (with --wrap for mocking)
$(CHECK_TEST_MULTI_CALC): $(CHECK_OUTPUT_DIR)/test_multi_calc.o $(CHECK_MOCK_HELPER_OBJ)
	@echo "Building test executable (with mock): $@"
	@$(MKDIR) $(dir $@)
	$(CC) $^ -o $@ $(CHECK_MOCK_LDFLAGS)

# Build check_test_features executable
$(CHECK_TEST_FEATURES): $(CHECK_OUTPUT_DIR)/test_check_features.o
	@echo "Building test executable: $@"
	@$(MKDIR) $(dir $@)
	$(CC) $< -o $@ $(CHECK_LDFLAGS)

# Clean Check UT artifacts
.PHONY: clean-ut-check
clean-ut-check:
	$(RM) $(CHECK_OUTPUT_DIR) $(CHECK_GEN_DIR) $(CHECK_REPORT_DIR) \
		$(CHECK_TEST_CALC) $(CHECK_TEST_GREETING) $(CHECK_TEST_MULTI_CALC) $(CHECK_TEST_FEATURES)
