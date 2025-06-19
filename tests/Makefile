CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I..
LDFLAGS = -lssl -lcrypto -lsecp256k1

# Test targets
UNIT_TEST = test_main
INTEGRATION_TEST = integration_test
PERFORMANCE_TEST = performance_test
SECURITY_TEST = security_test

# Source files
UNIT_TEST_SRC = test_main.cpp ../wallet_generator_impl.cpp
INTEGRATION_TEST_SRC = integration_test.cpp
PERFORMANCE_TEST_SRC = performance_test.cpp ../wallet_generator_impl.cpp
SECURITY_TEST_SRC = security_test.cpp ../wallet_generator_impl.cpp

# Detect operating system
UNAME_S := $(shell uname -s)

# System-specific configuration
ifeq ($(UNAME_S),Linux)
    CXXFLAGS += -I/usr/include/secp256k1
endif

ifeq ($(UNAME_S),Darwin)
    # macOS with Homebrew
    CXXFLAGS += -I/usr/local/include -I/opt/homebrew/include
    LDFLAGS += -L/usr/local/lib -L/opt/homebrew/lib
endif

# Default target - run all tests
all: build-main test-unit test-integration test-performance test-security

# Build the main wallet generator (required for integration tests)
build-main:
	@echo "Building main wallet generator..."
	@cd .. && make

# Build individual test executables
$(UNIT_TEST): $(UNIT_TEST_SRC) test_framework.h
	$(CXX) $(CXXFLAGS) -o $(UNIT_TEST) $(UNIT_TEST_SRC) $(LDFLAGS)

$(INTEGRATION_TEST): $(INTEGRATION_TEST_SRC) test_framework.h
	$(CXX) $(CXXFLAGS) -o $(INTEGRATION_TEST) $(INTEGRATION_TEST_SRC) $(LDFLAGS)

$(PERFORMANCE_TEST): $(PERFORMANCE_TEST_SRC) test_framework.h
	$(CXX) $(CXXFLAGS) -o $(PERFORMANCE_TEST) $(PERFORMANCE_TEST_SRC) $(LDFLAGS)

$(SECURITY_TEST): $(SECURITY_TEST_SRC) test_framework.h
	$(CXX) $(CXXFLAGS) -o $(SECURITY_TEST) $(SECURITY_TEST_SRC) $(LDFLAGS)

# Individual test targets
test-unit: $(UNIT_TEST)
	@echo "\n=== Running Unit Tests ==="
	./$(UNIT_TEST)

test-integration: $(INTEGRATION_TEST) build-main
	@echo "\n=== Running Integration Tests ==="
	./$(INTEGRATION_TEST)

test-performance: $(PERFORMANCE_TEST)
	@echo "\n=== Running Performance Tests ==="
	./$(PERFORMANCE_TEST)

test-security: $(SECURITY_TEST)
	@echo "\n=== Running Security Tests ==="
	./$(SECURITY_TEST)

# Quick test - just unit tests
quick: $(UNIT_TEST)
	@echo "\n=== Running Quick Tests ==="
	./$(UNIT_TEST)

# Coverage testing
coverage: CXXFLAGS += --coverage
coverage: LDFLAGS += --coverage
coverage: clean $(UNIT_TEST) $(SECURITY_TEST)
	@echo "\n=== Running Tests with Coverage ==="
	./$(UNIT_TEST)
	./$(SECURITY_TEST)
	@echo "\n=== Generating Coverage Report ==="
	@if command -v lcov >/dev/null 2>&1; then \
		lcov --capture --directory . --output-file coverage.info; \
		lcov --remove coverage.info '/usr/*' --output-file coverage.info; \
		lcov --list coverage.info; \
	else \
		echo "lcov not installed. Install with: sudo apt-get install lcov"; \
	fi

# Memory testing with Valgrind
memtest: $(UNIT_TEST) $(SECURITY_TEST)
	@echo "\n=== Running Memory Tests ==="
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "Testing unit tests..."; \
		valgrind --leak-check=full --error-exitcode=1 ./$(UNIT_TEST); \
		echo "Testing security tests..."; \
		valgrind --leak-check=full --error-exitcode=1 ./$(SECURITY_TEST); \
	else \
		echo "Valgrind not installed. Install with: sudo apt-get install valgrind"; \
	fi

# Static analysis with suppressed warnings for API functions
static-analysis:
	@echo "\n=== Running Static Analysis ==="
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --error-exitcode=1 --suppress=missingIncludeSystem \
			--suppress=unusedFunction:../wallet_generator_impl.cpp:printWallet \
			--suppress=unusedFunction:../wallet_generator_impl.cpp:printUsage \
			--suppress=unusedFunction:test_framework.h:reset \
			--inline-suppr \
			$(UNIT_TEST_SRC) $(INTEGRATION_TEST_SRC) $(PERFORMANCE_TEST_SRC) $(SECURITY_TEST_SRC); \
	else \
		echo "cppcheck not installed. Install with: sudo apt-get install cppcheck"; \
	fi

# Stress testing
stress: $(PERFORMANCE_TEST)
	@echo "\n=== Running Stress Tests ==="
	./$(PERFORMANCE_TEST)
	@echo "Running extended stress test..."
	@for i in {1..5}; do \
		echo "Stress test iteration $$i"; \
		./$(PERFORMANCE_TEST) || exit 1; \
	done

# Test with different compilers
test-compilers: clean
	@echo "\n=== Testing with GCC ==="
	@$(MAKE) CXX=g++ $(UNIT_TEST)
	@./$(UNIT_TEST)
	@$(MAKE) clean
	@echo "\n=== Testing with Clang ==="
	@if command -v clang++ >/dev/null 2>&1; then \
		$(MAKE) CXX=clang++ $(UNIT_TEST); \
		./$(UNIT_TEST); \
	else \
		echo "Clang not available, skipping..."; \
	fi

# Benchmark specific functions
benchmark: $(PERFORMANCE_TEST)
	@echo "\n=== Running Benchmarks ==="
	@echo "Running performance test 10 times for averaging..."
	@for i in {1..10}; do \
		echo "Run $$i:"; \
		./$(PERFORMANCE_TEST) | grep "ms"; \
	done

# Test with different optimization levels
test-optimizations:
	@echo "\n=== Testing Different Optimization Levels ==="
	@for opt in "-O0" "-O1" "-O2" "-O3"; do \
		echo "Testing with $$opt"; \
		$(MAKE) clean; \
		$(MAKE) CXXFLAGS="$(CXXFLAGS) $$opt" $(UNIT_TEST); \
		./$(UNIT_TEST) || exit 1; \
	done

# Clean build artifacts
clean:
	rm -f $(UNIT_TEST) $(INTEGRATION_TEST) $(PERFORMANCE_TEST) $(SECURITY_TEST)
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -f *.o

# Install test dependencies
install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install -y libssl-dev libsecp256k1-dev build-essential
	sudo apt-get install -y valgrind lcov cppcheck

install-deps-macos:
	brew install openssl secp256k1

# Help target
help:
	@echo "Available targets:"
	@echo "  all              - Run all tests"
	@echo "  quick            - Run only unit tests"
	@echo "  test-unit        - Run unit tests"
	@echo "  test-integration - Run integration tests"
	@echo "  test-performance - Run performance tests"
	@echo "  test-security    - Run security tests"
	@echo "  coverage         - Run tests with coverage analysis"
	@echo "  memtest          - Run tests with memory leak detection"
	@echo "  static-analysis  - Run static code analysis"
	@echo "  stress           - Run stress tests"
	@echo "  benchmark        - Run performance benchmarks"
	@echo "  test-compilers   - Test with different compilers"
	@echo "  test-optimizations - Test with different optimization levels"
	@echo "  clean            - Clean build artifacts"
	@echo "  install-deps-*   - Install dependencies"
	@echo "  help             - Show this help"

.PHONY: all quick test-unit test-integration test-performance test-security
.PHONY: coverage memtest static-analysis stress benchmark test-compilers test-optimizations
.PHONY: clean install-deps-ubuntu install-deps-macos help build-main