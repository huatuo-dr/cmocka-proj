# Check unit test coverage build rules

# Coverage directories
CHECK_COV_OUTPUT_DIR := $(OUTPUT_DIR)/ut_check_cov
CHECK_COV_GEN_DIR := $(OUTPUT_DIR)/ut_check_cov/generated
CHECK_COV_SDK_DIR := $(CHECK_COV_OUTPUT_DIR)/sdk
CHECK_COV_REPORT_DIR := $(BUILD_DIR)/coverage-check-report

# Coverage executables
CHECK_COV_TEST_CALC := $(DIST_DIR)/check_cov_test_calc
CHECK_COV_TEST_GREETING := $(DIST_DIR)/check_cov_test_greeting
CHECK_COV_TEST_MULTI_CALC := $(DIST_DIR)/check_cov_test_multi_calc
CHECK_COV_TEST_FEATURES := $(DIST_DIR)/check_cov_test_features

# Coverage flags
CHECK_COV_CFLAGS := $(CFLAGS) --coverage -I$(SDK_INC_DIR) -I$(CHECK_INC_DIR)
CHECK_COV_LDFLAGS := -L$(CHECK_LIB_DIR) -lcheck -pthread -lm -lrt --coverage

# Coverage mock LDFLAGS
CHECK_COV_MOCK_LDFLAGS := $(CHECK_COV_LDFLAGS) \
    -Wl,--wrap=calc_add \
    -Wl,--wrap=calc_subtract \
    -Wl,--wrap=calc_multiply \
    -Wl,--wrap=calc_divide

# SDK source files for coverage
CHECK_COV_SDK_SRCS := $(wildcard $(SDK_SRC_DIR)/*.c)
CHECK_COV_SDK_OBJS := $(patsubst $(SDK_SRC_DIR)/%.c, $(CHECK_COV_SDK_DIR)/%.o, $(CHECK_COV_SDK_SRCS))

# Generated C files for coverage
CHECK_COV_GEN_SRCS := $(patsubst $(CHECK_SRC_DIR)/%.ts, $(CHECK_COV_GEN_DIR)/%.c, $(CHECK_TS_FILES))

# Prevent Make from deleting generated .c files as intermediate files
.PRECIOUS: $(CHECK_COV_GEN_DIR)/%.c

# Build and run coverage tests
.PHONY: ut_check_cov
ut_check_cov: ut_check_cov_run ut_check_cov_report
	@echo ""
	@echo "========================================"
	@echo "Check Coverage Tests Completed!"
	@echo "  Report: $(CHECK_COV_REPORT_DIR)/index.html"
	@echo "========================================"

# Run coverage tests
.PHONY: ut_check_cov_run
ut_check_cov_run: ut_check_cov_build
	@echo ""
	@echo "========================================"
	@echo "Running Check Coverage Tests..."
	@echo "========================================"
	@echo ""
	@echo "--- Running check_cov_test_calc ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_COV_TEST_CALC)
	@echo ""
	@echo "--- Running check_cov_test_greeting ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_COV_TEST_GREETING)
	@echo ""
	@echo "--- Running check_cov_test_multi_calc ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_COV_TEST_MULTI_CALC)
	@echo ""
	@echo "--- Running check_cov_test_features ---"
	@LD_LIBRARY_PATH=$(CHECK_LIB_DIR):$$LD_LIBRARY_PATH $(CHECK_COV_TEST_FEATURES) || true

# Generate coverage report
.PHONY: ut_check_cov_report
ut_check_cov_report: ut_check_cov_run
	@echo ""
	@echo "========================================"
	@echo "Generating Check Coverage Report..."
	@echo "========================================"
	@$(MKDIR) $(CHECK_COV_REPORT_DIR)
	lcov --capture --directory $(CHECK_COV_SDK_DIR) --output-file $(CHECK_COV_REPORT_DIR)/coverage.info --rc lcov_branch_coverage=1
	lcov --remove $(CHECK_COV_REPORT_DIR)/coverage.info '/usr/*' --output-file $(CHECK_COV_REPORT_DIR)/coverage.info --rc lcov_branch_coverage=1
	genhtml $(CHECK_COV_REPORT_DIR)/coverage.info --output-directory $(CHECK_COV_REPORT_DIR) --branch-coverage
	@echo ""
	@echo "Coverage report generated: $(CHECK_COV_REPORT_DIR)/index.html"

# Build coverage tests
.PHONY: ut_check_cov_build
ut_check_cov_build: $(CHECK_COV_TEST_CALC) $(CHECK_COV_TEST_GREETING) $(CHECK_COV_TEST_MULTI_CALC) $(CHECK_COV_TEST_FEATURES)
	@echo "Check coverage test executables built successfully"

# Generate C files from .ts for coverage
$(CHECK_COV_GEN_DIR)/%.c: $(CHECK_SRC_DIR)/%.ts
	@echo "Generating C from checkmk (cov): $< -> $@"
	@$(MKDIR) $(dir $@)
	$(CHECKMK) $< > $@

# Compile SDK sources with coverage
$(CHECK_COV_SDK_DIR)/%.o: $(SDK_SRC_DIR)/%.c
	@echo "Compiling SDK (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CHECK_COV_CFLAGS) -c $< -o $@

# Compile generated test files with coverage
$(CHECK_COV_OUTPUT_DIR)/%.o: $(CHECK_COV_GEN_DIR)/%.c
	@echo "Compiling (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CHECK_COV_CFLAGS) -c $< -o $@

# Compile mock helpers with coverage
$(CHECK_COV_OUTPUT_DIR)/mock_helpers.o: $(CHECK_MOCK_HELPER)
	@echo "Compiling mock helpers (coverage): $<"
	@$(MKDIR) $(dir $@)
	$(CC) $(CHECK_COV_CFLAGS) -c $< -o $@

# Build coverage test executables
$(CHECK_COV_TEST_CALC): $(CHECK_COV_OUTPUT_DIR)/test_calc.o $(CHECK_COV_SDK_OBJS)
	@echo "Building coverage test: $@"
	@$(MKDIR) $(dir $@)
	$(CC) $^ -o $@ $(CHECK_COV_LDFLAGS)

$(CHECK_COV_TEST_GREETING): $(CHECK_COV_OUTPUT_DIR)/test_greeting.o $(CHECK_COV_SDK_OBJS)
	@echo "Building coverage test: $@"
	@$(MKDIR) $(dir $@)
	$(CC) $^ -o $@ $(CHECK_COV_LDFLAGS)

$(CHECK_COV_TEST_MULTI_CALC): $(CHECK_COV_OUTPUT_DIR)/test_multi_calc.o $(CHECK_COV_OUTPUT_DIR)/mock_helpers.o $(CHECK_COV_SDK_OBJS)
	@echo "Building coverage test (mock): $@"
	@$(MKDIR) $(dir $@)
	$(CC) $^ -o $@ $(CHECK_COV_MOCK_LDFLAGS)

$(CHECK_COV_TEST_FEATURES): $(CHECK_COV_OUTPUT_DIR)/test_check_features.o $(CHECK_COV_SDK_OBJS)
	@echo "Building coverage test: $@"
	@$(MKDIR) $(dir $@)
	$(CC) $^ -o $@ $(CHECK_COV_LDFLAGS)

# Clean coverage artifacts
.PHONY: clean-check-cov
clean-check-cov:
	$(RM) $(CHECK_COV_OUTPUT_DIR) $(CHECK_COV_GEN_DIR) $(CHECK_COV_REPORT_DIR) \
		$(CHECK_COV_TEST_CALC) $(CHECK_COV_TEST_GREETING) $(CHECK_COV_TEST_MULTI_CALC) $(CHECK_COV_TEST_FEATURES)
