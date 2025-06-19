#!/bin/bash

# CI/CD Test Script for BIP39 Wallet Generator
# This script is designed to run in continuous integration environments

set -e  # Exit on any error
set -x  # Print commands as they're executed

# Environment variables
export DEBIAN_FRONTEND=noninteractive
export CI=true

# Colors for output (if supported)
if [ -t 1 ]; then
    RED='\033[0;31m'
    GREEN='\033[0;32m'
    YELLOW='\033[1;33m'
    BLUE='\033[0;34m'
    NC='\033[0m'
else
    RED=''
    GREEN=''
    YELLOW=''
    BLUE=''
    NC=''
fi

# Function to print colored output
print_color() {
    local color=$1
    shift
    echo -e "${color}$@${NC}"
}

# Function to print CI section headers
print_ci_header() {
    echo
    echo "::group::$1"
    print_color $BLUE "=================================================="
    print_color $BLUE "$1"
    print_color $BLUE "=================================================="
    echo
}

# Function to end CI group
end_ci_group() {
    echo "::endgroup::"
}

# Function to check system info
check_system_info() {
    print_ci_header "System Information"
    
    echo "Operating System:"
    uname -a
    
    echo "Compiler Information:"
    gcc --version || true
    g++ --version || true
    clang --version || true
    
    echo "Build Tools:"
    make --version || true
    
    echo "Available Memory:"
    free -h || true
    
    echo "CPU Information:"
    nproc || true
    
    end_ci_group
}

# Function to install dependencies
install_dependencies() {
    print_ci_header "Installing Dependencies"
    
    # Detect OS and install accordingly
    if [ -f /etc/debian_version ]; then
        echo "Detected Debian/Ubuntu system"
        apt-get update
        apt-get install -y libssl-dev libsecp256k1-dev build-essential
        apt-get install -y valgrind lcov cppcheck
    elif [ -f /etc/redhat-release ]; then
        echo "Detected RedHat/CentOS system"
        yum install -y openssl-devel gcc-c++ make
        # Note: secp256k1 might need to be built from source on RHEL/CentOS
    elif [ "$(uname)" == "Darwin" ]; then
        echo "Detected macOS system"
        # Assuming Homebrew is available in CI
        brew install openssl secp256k1
    else
        echo "Unknown operating system"
        exit 1
    fi
    
    end_ci_group
}

# Function to build the project
build_project() {
    print_ci_header "Building Project"
    
    # Clean any existing builds
    make clean || true
    
    # Build with maximum warnings
    make CXXFLAGS="-std=c++17 -Wall -Wextra -Werror -O2"
    
    # Verify binary was created
    if [ ! -f "./wallet_generator" ]; then
        print_color $RED "Build failed: wallet_generator binary not found"
        exit 1
    fi
    
    # Test basic functionality
    ./wallet_generator -h
    
    end_ci_group
}

# Function to run unit tests
run_unit_tests() {
    print_ci_header "Running Unit Tests"
    
    cd tests
    make test-unit
    cd ..
    
    end_ci_group
}

# Function to run integration tests
run_integration_tests() {
    print_ci_header "Running Integration Tests"
    
    cd tests
    make test-integration
    cd ..
    
    end_ci_group
}

# Function to run security tests
run_security_tests() {
    print_ci_header "Running Security Tests"
    
    cd tests
    make test-security
    cd ..
    
    end_ci_group
}

# Function to run performance tests
run_performance_tests() {
    print_ci_header "Running Performance Tests"
    
    cd tests
    make test-performance
    cd ..
    
    end_ci_group
}

# Function to run static analysis
run_static_analysis() {
    print_ci_header "Running Static Analysis"
    
    if command -v cppcheck &> /dev/null; then
        cd tests
        make static-analysis
        cd ..
    else
        print_color $YELLOW "cppcheck not available, skipping static analysis"
    fi
    
    end_ci_group
}

# Function to run memory tests
run_memory_tests() {
    print_ci_header "Running Memory Tests"
    
    if command -v valgrind &> /dev/null; then
        cd tests
        make memtest
        cd ..
    else
        print_color $YELLOW "valgrind not available, skipping memory tests"
    fi
    
    end_ci_group
}

# Function to run coverage analysis
run_coverage_analysis() {
    print_ci_header "Running Coverage Analysis"
    
    if command -v lcov &> /dev/null; then
        cd tests
        make coverage
        
        # Generate HTML report if possible
        if [ -f coverage.info ]; then
            genhtml coverage.info --output-directory coverage_html || true
            echo "Coverage report generated in tests/coverage_html/"
        fi
        cd ..
    else
        print_color $YELLOW "lcov not available, skipping coverage analysis"
    fi
    
    end_ci_group
}

# Function to test different networks
test_known_vectors() {
    print_ci_header "Testing Known Test Vectors"
    
    # Test with known BIP39 test vector
    echo "Testing Bitcoin wallet generation..."
    ./wallet_generator "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
    
    echo "Testing Ethereum wallet generation..."
    ./wallet_generator -n ethereum "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
    
    echo "Testing all networks..."
    ./wallet_generator -a "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
    
    echo "Testing with passphrase..."
    ./wallet_generator -P "TREZOR" "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
    
    echo "Testing custom derivation path..."
    ./wallet_generator -n bitcoin -p "m/44'/0'/0'/0/5" "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
    
    echo "Testing batch generation..."
    ./wallet_generator -n ethereum -c 3 "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
    
    end_ci_group
}

# Function to run compiler tests
test_compilers() {
    print_ci_header "Testing Different Compilers"
    
    # Test with different compilers if available
    if command -v gcc &> /dev/null; then
        echo "Testing with GCC..."
        make clean
        make CXX=gcc CXXFLAGS="-std=c++17 -Wall -Wextra -O2"
        ./wallet_generator -h
    fi
    
    if command -v clang++ &> /dev/null; then
        echo "Testing with Clang..."
        make clean
        make CXX=clang++ CXXFLAGS="-std=c++17 -Wall -Wextra -O2"
        ./wallet_generator -h
    fi
    
    # Restore default build
    make clean
    make
    
    end_ci_group
}

# Function to create test artifacts
create_artifacts() {
    print_ci_header "Creating Test Artifacts"
    
    # Create artifacts directory
    mkdir -p artifacts
    
    # Copy binary
    cp wallet_generator artifacts/
    
    # Copy test results if they exist
    if [ -d tests/coverage_html ]; then
        cp -r tests/coverage_html artifacts/
    fi
    
    # Create build info
    cat > artifacts/build_info.txt << EOF
Build Date: $(date)
Git Commit: ${GITHUB_SHA:-"unknown"}
Operating System: $(uname -a)
Compiler: $(g++ --version | head -n1)
Build Flags: -std=c++17 -Wall -Wextra -O2
EOF
    
    end_ci_group
}

# Function to run full CI pipeline
run_full_ci() {
    print_color $BLUE "Starting CI/CD Pipeline for BIP39 Wallet Generator"
    
    # Set up error handling
    trap 'print_color $RED "CI Pipeline failed at line $LINENO"' ERR
    
    check_system_info
    install_dependencies
    build_project
    
    # Core tests (always run)
    run_unit_tests
    run_integration_tests
    run_security_tests
    
    # Optional tests (run if tools available)
    run_static_analysis
    run_memory_tests
    run_coverage_analysis
    
    # Additional verification
    test_known_vectors
    test_compilers
    run_performance_tests
    
    # Create artifacts
    create_artifacts
    
    print_color $GREEN "✅ All CI tests passed successfully!"
}

# Main execution
main() {
    case "${1:-full}" in
        "quick")
            check_system_info
            build_project
            run_unit_tests
            test_known_vectors
            ;;
        "security")
            build_project
            run_security_tests
            run_static_analysis
            run_memory_tests
            ;;
        "coverage")
            build_project
            run_coverage_analysis
            ;;
        "full"|*)
            run_full_ci
            ;;
    esac
}

# Run main function
main "$@"