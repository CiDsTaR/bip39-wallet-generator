cmake_minimum_required(VERSION 3.15)
project(BIP39WalletGenerator VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4 /permissive-)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find required packages
find_package(OpenSSL REQUIRED)

# Platform-specific configurations
if(WIN32)
    # Windows-specific settings
    add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX)
    
    # Use vcpkg or find_package for secp256k1
    find_path(SECP256K1_INCLUDE_DIR secp256k1.h)
    find_library(SECP256K1_LIBRARY secp256k1)
    
    # On Windows, prefer minimal implementation due to linking issues
    if(NOT SECP256K1_INCLUDE_DIR OR NOT SECP256K1_LIBRARY)
        message(STATUS "secp256k1 not found. Using minimal implementation.")
        set(USE_MINIMAL_SECP256K1 ON)
    else()
        # Try to use full secp256k1, but fallback to minimal if linking fails
        message(STATUS "secp256k1 found, but using minimal implementation for Windows compatibility.")
        set(USE_MINIMAL_SECP256K1 ON)
    endif()
    
    # Always use Windows-compatible source file
    set(SOURCES wallet_generator_win.cpp)
else()
    # Unix-like systems
    find_path(SECP256K1_INCLUDE_DIR secp256k1.h)
    find_library(SECP256K1_LIBRARY secp256k1)
    
    if(NOT SECP256K1_INCLUDE_DIR OR NOT SECP256K1_LIBRARY)
        message(FATAL_ERROR "secp256k1 library not found")
    endif()
    
    set(SOURCES wallet_generator.cpp wallet_generator_impl.cpp)
endif()

# Create executable
add_executable(wallet_generator ${SOURCES})

# Include directories
target_include_directories(wallet_generator PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENSSL_INCLUDE_DIR}
)

if(WIN32)
    target_include_directories(wallet_generator PRIVATE windows)
endif()

if(SECP256K1_INCLUDE_DIR AND NOT USE_MINIMAL_SECP256K1)
    target_include_directories(wallet_generator PRIVATE ${SECP256K1_INCLUDE_DIR})
endif()

# Link libraries
target_link_libraries(wallet_generator 
    OpenSSL::SSL 
    OpenSSL::Crypto
)

if(SECP256K1_LIBRARY AND NOT USE_MINIMAL_SECP256K1)
    target_link_libraries(wallet_generator ${SECP256K1_LIBRARY})
endif()

# Windows-specific linking
if(WIN32)
    target_link_libraries(wallet_generator ws2_32 crypt32)
endif()

# Compiler definitions
if(USE_MINIMAL_SECP256K1)
    target_compile_definitions(wallet_generator PRIVATE USE_MINIMAL_SECP256K1)
endif()

# Installation
install(TARGETS wallet_generator
    RUNTIME DESTINATION bin
)

# Testing (optional)
option(BUILD_TESTS "Build test suite" OFF)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Package configuration
set(CPACK_PACKAGE_NAME "BIP39WalletGenerator")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BIP39 Multi-Network Wallet Generator")
set(CPACK_PACKAGE_VENDOR "BIP39 Wallet Generator Project")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "BIP39 Wallet Generator")
    set(CPACK_NSIS_PACKAGE_NAME "BIP39WalletGenerator")
else()
    set(CPACK_GENERATOR "TGZ;DEB")
endif()

include(CPack)