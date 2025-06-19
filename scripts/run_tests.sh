#!/bin/bash

# BIP39 Wallet Generator Test Runner
# This script provides a comprehensive testing interface

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test results tracking
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Function to print colored output
print_color() {
    local color=$1
    shift
    echo -e "${color}$@${NC}"
}

# Function to print section headers
print_header() {
    echo
    print_color $BLUE "=================================================="
    print_color $BLUE "$1"
    print_color $BLUE "=================================================="
    echo
}

# Function to run a test and track results
run_test() {
    local test_name="$1"
    local test_command="$2"
    
    print_color $YELLOW "Running: $test_name"
    echo "Command: $test_command"
    echo
    
    # Run the command and capture exit code
    set +e  # Temporarily disable exit on error
    eval $test_command
    local exit_code=$?
    set -e  # Re-enable exit on error
    
    if [ $exit_code -eq 0 ]; then
        print_color $GREEN "✓ $test_name PASSED"
        ((PASSED_TESTS++))
    else
        print_color $RED "✗ $test_name FAILED (exit code: $exit_code)"
        ((FAILED_TESTS++))
    fi
    ((TOTAL_TESTS++))
    echo
}

# Function to check prerequisites
check_prerequisites() {
    print_header "Checking Prerequisites"
    
    # Check if wallet_generator exists
    if [ ! -f "./wallet_generator" ]; then
        print_color $YELLOW "wallet_generator not found. Building..."
        make clean && make
        if [ ! -f "./wallet_generator" ]; then
            print_color $RED "Failed to build wallet_generator"
            exit 1
        fi
    fi
    
    # Check required tools
    local tools=("g++" "make")
    for tool in "${tools[@]}"; do
        if ! command -v $tool &> /dev/null; then
            print_color $RED "$tool is required but not installed"
            exit 1
        fi
        print_color $GREEN "✓ $tool found"
    done
    
    # Check optional tools
    local optional_tools=("valgrind" "lcov" "cppcheck")
    for tool in "${optional_tools[@]}"; do
        if command -v $tool &> /dev/null; then
            print_color $GREEN "✓ $tool found (optional)"
        else
            print_color $YELLOW "○ $tool not found (optional)"
        fi
    done
}

# Function to show usage
show_usage() {
    echo "Usage: $0 [OPTIONS]"
    echo
    echo "Options:"
    echo "  -a, --all           Run all tests"
    echo "  -q, --quick         Run quick tests only"
    echo "  -u, --unit          Run unit tests"
    echo "  -i, --integration   Run integration tests"
    echo "  -p, --performance   Run performance tests"
    echo "  -s, --security      Run security tests"
    echo "  -c, --coverage      Run tests with coverage"
    echo "  -m, --memory        Run memory leak tests"
    echo "  -t, --static        Run static analysis"
    echo "  -b, --benchmark     Run performance benchmarks"
    echo "  -x, --stress        Run stress tests"
    echo "  -o, --optimizations Test different optimization levels"
    echo "  -r, --compilers     Test with different compilers"
    echo "  -h, --help          Show this help"
    echo
    echo "Examples:"
    echo "  $0 --all           # Run complete test suite"
    echo "  $0 --quick         # Run basic tests quickly"
    echo "  $0 -u -s           # Run unit and security tests"
    echo "  $0 --coverage      # Run with coverage analysis"
}

# Function to run comprehensive test suite
run_all_tests() {
    print_header "Comprehensive Test Suite"
    
    run_test "Unit Tests" "cd tests && make test-unit"
    run_test "Integration Tests" "cd tests && make test-integration"
    run_test "Performance Tests" "cd tests && make test-performance"
    run_test "Security Tests" "cd tests && make test-security"
    
    if command -v cppcheck &> /dev/null; then
        run_test "Static Analysis" "cd tests && make static-analysis"
    fi
    
    if command -v valgrind &> /dev/null; then
        run_test "Memory Leak Tests" "cd tests && make memtest"
    fi
}

# Function to run quick tests
run_quick_tests() {
    print_header "Quick Test Suite"
    
    run_test "Unit Tests" "cd tests && make test-unit"
    run_test "Basic Integration" "./wallet_generator \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\""
}

# Function to show test results summary
show_summary() {
    print_header "Test Results Summary"
    
    echo "Total Tests: $TOTAL_TESTS"
    if [ $PASSED_TESTS -gt 0 ]; then
        print_color $GREEN "Passed: $PASSED_TESTS"
    fi
    if [ $FAILED_TESTS -gt 0 ]; then
        print_color $RED "Failed: $FAILED_TESTS"
    fi
    
    if [ $TOTAL_TESTS -gt 0 ]; then
        local success_rate=$((PASSED_TESTS * 100 / TOTAL_TESTS))
        if [ $FAILED_TESTS -eq 0 ]; then
            print_color $GREEN "Success Rate: ${success_rate}% - ALL TESTS PASSED! 🎉"
        else
            print_color $RED "Success Rate: ${success_rate}% - Some tests failed ❌"
        fi
    else
        print_color $YELLOW "No tests were run"
    fi
    
    echo
}

# Main execution
main() {
    # Default to showing help if no arguments
    if [ $# -eq 0 ]; then
        show_usage
        exit 0
    fi
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -a|--all)
                check_prerequisites
                run_all_tests
                show_summary
                # Fixed: Return the actual failed test count, not just whether there are failures
                exit $FAILED_TESTS
                ;;
            -q|--quick)
                check_prerequisites
                run_quick_tests
                show_summary
                exit $FAILED_TESTS
                ;;
            -u|--unit)
                check_prerequisites
                run_test "Unit Tests" "cd tests && make test-unit"
                show_summary
                exit $FAILED_TESTS
                ;;
            -i|--integration)
                check_prerequisites
                run_test "Integration Tests" "cd tests && make test-integration"
                show_summary
                exit $FAILED_TESTS
                ;;
            -p|--performance)
                check_prerequisites
                run_test "Performance Tests" "cd tests && make test-performance"
                show_summary
                exit $FAILED_TESTS
                ;;
            -s|--security)
                check_prerequisites
                run_test "Security Tests" "cd tests && make test-security"
                show_summary
                exit $FAILED_TESTS
                ;;
            -c|--coverage)
                check_prerequisites
                if command -v lcov &> /dev/null; then
                    run_test "Coverage Analysis" "cd tests && make coverage"
                else
                    print_color $RED "lcov not found. Install with: sudo apt-get install lcov"
                    exit 1
                fi
                show_summary
                exit $FAILED_TESTS
                ;;
            -m|--memory)
                check_prerequisites
                if command -v valgrind &> /dev/null; then
                    run_test "Memory Leak Tests" "cd tests && make memtest"
                else
                    print_color $RED "valgrind not found. Install with: sudo apt-get install valgrind"
                    exit 1
                fi
                show_summary
                exit $FAILED_TESTS
                ;;
            -t|--static)
                check_prerequisites
                if command -v cppcheck &> /dev/null; then
                    run_test "Static Analysis" "cd tests && make static-analysis"
                else
                    print_color $RED "cppcheck not found. Install with: sudo apt-get install cppcheck"
                    exit 1
                fi
                show_summary
                exit $FAILED_TESTS
                ;;
            -b|--benchmark)
                check_prerequisites
                run_test "Performance Benchmarks" "cd tests && make benchmark"
                show_summary
                exit $FAILED_TESTS
                ;;
            -x|--stress)
                check_prerequisites
                run_test "Stress Tests" "cd tests && make stress"
                show_summary
                exit $FAILED_TESTS
                ;;
            -o|--optimizations)
                check_prerequisites
                run_test "Optimization Level Tests" "cd tests && make test-optimizations"
                show_summary
                exit $FAILED_TESTS
                ;;
            -r|--compilers)
                check_prerequisites
                run_test "Compiler Tests" "cd tests && make test-compilers"
                show_summary
                exit $FAILED_TESTS
                ;;
            -h|--help)
                show_usage
                exit 0
                ;;
            *)
                print_color $RED "Unknown option: $1"
                show_usage
                exit 1
                ;;
        esac
        shift
    done
}

# Run main function with all arguments
main "$@"