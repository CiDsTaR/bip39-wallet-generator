CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
LDFLAGS = -lssl -lcrypto -lsecp256k1

TARGET = wallet_generator
SOURCES = wallet_generator.cpp

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

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install -y libssl-dev libsecp256k1-dev build-essential

install-deps-macos:
	brew install openssl secp256k1

clean:
	rm -f $(TARGET)

test:
	./$(TARGET) -h
	./$(TARGET) -n ethereum "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Run comprehensive tests
test-all:
	@echo "Running comprehensive test suite..."
	@cd tests && make all

# Run quick tests
test-quick:
	@echo "Running quick tests..."
	@cd tests && make quick

# Run specific test categories
test-unit:
	@cd tests && make test-unit

test-integration:
	@cd tests && make test-integration

test-performance:
	@cd tests && make test-performance

test-security:
	@cd tests && make test-security

# Coverage and analysis
coverage:
	@cd tests && make coverage

memtest:
	@cd tests && make memtest

static-analysis:
	@cd tests && make static-analysis

# Clean everything including tests
clean-all: clean
	@cd tests && make clean

.PHONY: all clean install-deps-ubuntu install-deps-macos test test-all test-quick
.PHONY: test-unit test-integration test-performance test-security coverage memtest static-analysis clean-all