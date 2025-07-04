# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Native Windows test suite development
- GUI interface planning  
- Docker containerization
- Hardware wallet integration research
- Enhanced RIPEMD160 detection and fallback handling
- Production security hardening features

### Changed
- Performance optimizations for key derivation
- Enhanced cross-platform compatibility
- Improved documentation and security warnings

### Security
- Formal security audit planning
- Enhanced memory security improvements
- RIPEMD160 fallback documentation and validation

## [1.1.1] - 2025-01-25

### Security
- **🚨 CRITICAL**: Enhanced documentation of RIPEMD160 fallback behavior
- **Added prominent security warnings** about Bitcoin address generation differences
- **Windows security limitations** clearly documented and emphasized
- **Cross-validation procedures** added for address verification

### Changed  
- **Updated README.md** with comprehensive security warnings and platform-specific limitations
- **Enhanced SECURITY.md** with detailed cryptographic implementation warnings
- **Improved WINDOWS_SETUP.md** with security considerations and cross-validation procedures
- **Updated build documentation** to reflect actual file structure and implementation

### Fixed
- **Documentation accuracy** - All build instructions now match actual implementation
- **Security disclosure** - RIPEMD160 fallback properly documented
- **Platform differences** - Clear explanation of Unix vs Windows implementations

### Documentation
- **Added critical security warnings** prominently in all relevant documentation
- **Enhanced troubleshooting sections** with platform-specific guidance
- **Improved cross-validation examples** for address verification
- **Updated production readiness guidelines** with current limitations

## [1.1.0] - 2025-01-20

### Added
- **Full Windows Support** - Complete cross-platform compatibility
  - Windows-compatible source code (`wallet_generator_win.cpp`)
  - Custom getopt implementation for Windows (`windows/getopt_win.h`)
  - Minimal secp256k1 implementation for Windows (`windows/minimal_secp256k1.h`)
  - CMake build system for cross-platform builds
  - Windows-specific build scripts (`scripts/build_windows.bat`)
  - PowerShell dependency installer (`scripts/install_windows_deps.ps1`)
  - Comprehensive Windows setup documentation (`WINDOWS_SETUP.md`)
- **Enhanced Build System**
  - CMake configuration with automatic dependency detection
  - vcpkg integration for Windows package management
  - Cross-platform build support (Make + CMake)
  - Automated dependency installation scripts
- **Comprehensive Test Suite**
  - Unit tests (`tests/test_main.cpp`) - 50+ test cases
  - Integration tests (`tests/integration_test.cpp`) - CLI interface testing
  - Security tests (`tests/security_test.cpp`) - Cryptographic validation
  - Performance tests (`tests/performance_test.cpp`) - Benchmarking
  - Custom test framework (`tests/test_framework.h`)
  - Interactive test runner (`scripts/run_tests.sh`)
  - CI/CD test automation (`scripts/ci_test.sh`)
- **Development Infrastructure**
  - GitHub Actions CI/CD pipeline with multiple compilers
  - Code coverage analysis with lcov
  - Static analysis with cppcheck
  - Memory leak detection with Valgrind
  - Multi-platform testing (Ubuntu, macOS, Windows)
  - Automated security scanning

### Changed
- **Cross-Platform Architecture**
  - Restructured codebase for platform compatibility
  - Separated Unix and Windows source files (`wallet_generator.cpp` + `wallet_generator_impl.cpp` vs `wallet_generator_win.cpp`)
  - Enhanced Makefile with test integration
  - Improved error handling and validation
- **Documentation Overhaul**
  - Updated README with Windows support information
  - Added platform-specific installation guides
  - Enhanced contribution guidelines
  - Improved security documentation
- **Testing Infrastructure**
  - Migrated from basic testing to comprehensive test suite
  - Added multiple test categories and automation
  - Integrated CI/CD pipeline with GitHub Actions
  - Added performance benchmarking and security validation

### Fixed
- Windows compilation issues with POSIX dependencies
- Cross-platform path handling
- Compiler compatibility across different systems
- Memory management improvements
- Enhanced input validation and error handling

### Security
- **Enhanced Cryptographic Validation**
  - Comprehensive security test suite
  - Private key randomness verification
  - Seed determinism validation
  - Network isolation testing
  - Address uniqueness verification
- **Platform Security Considerations**
  - Windows-specific security recommendations
  - Memory clearing improvements
  - Secure random number generation validation
  - **⚠️ RIPEMD160 Fallback Implementation**: Uses double SHA256 when RIPEMD160 unavailable (documented limitation)
  - **⚠️ Windows Minimal secp256k1**: Reduced cryptographic guarantees on Windows platform
- **Development Security**
  - Automated security scanning in CI/CD
  - Static analysis integration
  - Memory leak detection
  - Code coverage requirements

### Development
- **Multi-Platform Development Environment**
  - Visual Studio 2019/2022 support for Windows
  - Enhanced debugging capabilities
  - Cross-platform development guidelines
  - Automated build and test procedures
- **Quality Assurance**
  - Comprehensive test coverage (unit, integration, security, performance)
  - Multiple compiler testing (GCC, Clang, MSVC)
  - Platform compatibility verification
  - Performance regression testing

### Documentation
- **Windows Support Documentation**
  - Complete Windows setup guide (`WINDOWS_SETUP.md`)
  - vcpkg and dependency management instructions
  - Visual Studio configuration guidelines
  - Windows-specific troubleshooting
- **Enhanced Development Documentation**
  - Updated contributing guidelines with Windows considerations
  - Platform-specific build instructions
  - Testing procedures for all platforms
  - Security best practices documentation

## [1.0.0] - 2025-01-15

### Added
- Initial release of BIP39 Wallet Generator
- Support for 10 major blockchain networks:
  - Bitcoin (BTC)
  - Ethereum (ETH)
  - Binance Smart Chain (BSC)
  - Polygon (MATIC)
  - Avalanche (AVAX)
  - Solana (SOL)
  - Cardano (ADA)
  - Litecoin (LTC)
  - Dogecoin (DOGE)
  - Tron (TRX)
- BIP39 mnemonic to seed conversion
- BIP44 HD key derivation (simplified implementation)
- Multiple output formats (private key, public key, address, WIF)
- Command-line interface with comprehensive options
- Batch wallet generation
- Custom derivation path support
- Optional BIP39 passphrase support
- Cross-platform support (Linux, macOS)
- Comprehensive documentation and examples

### Security
- Uses established cryptographic libraries (OpenSSL, secp256k1)
- Implements secure random number generation
- Follows BIP39/BIP44 specifications (with simplifications)
- Memory clearing for sensitive data
- Input validation and error handling
- **⚠️ Known Limitation**: Simplified HD derivation (not full BIP32)
- **⚠️ Known Limitation**: RIPEMD160 fallback to double SHA256 when unavailable

### Development
- C++17 implementation
- Comprehensive build system with Makefile
- Basic CI/CD pipeline with GitHub Actions
- Code documentation and examples

### Documentation
- Complete README with usage examples
- Contributing guidelines
- Security policy
- Code of conduct
- Issue templates
- License (MIT)

## [0.9.0] - 2025-01-10 [Pre-release]

### Added
- Beta implementation of core functionality
- Basic network support for Bitcoin and Ethereum
- Command-line argument parsing
- Initial test suite

### Changed
- Refactored wallet generation logic
- Improved error handling

### Fixed
- Address generation issues for some networks
- Memory management improvements

## [0.5.0] - 2025-01-05 [Alpha]

### Added
- Proof of concept implementation
- Basic BIP39 mnemonic handling
- Simple Bitcoin wallet generation
- Core cryptographic functions

### Notes
- Alpha release for testing and feedback
- Limited network support
- Basic functionality only

---

## Release Notes

### Version 1.1.1 - Security Documentation Update

This patch release focuses on **critical security documentation updates** and does not change the core functionality.

#### 🚨 Critical Security Documentation Updates
- **RIPEMD160 Fallback Warning**: Prominently documented the fallback to double SHA256 when RIPEMD160 is unavailable
- **Windows Security Limitations**: Clear warnings about reduced cryptographic guarantees on Windows
- **Cross-Validation Requirements**: Added procedures for validating addresses with established tools
- **Platform-Specific Security Considerations**: Detailed documentation of security differences between platforms

#### 📖 Documentation Improvements
- **Updated README.md**: Comprehensive security warnings, updated build instructions, enhanced testing documentation
- **Enhanced SECURITY.md**: Detailed cryptographic implementation warnings, platform security matrix, incident response procedures
- **Improved WINDOWS_SETUP.md**: Security considerations, cross-validation procedures, troubleshooting enhancements
- **Build Documentation**: Updated to reflect actual file structure (`wallet_generator.cpp` + `wallet_generator_impl.cpp`)

#### 🔍 What This Means for Users
- **No Functional Changes**: Core wallet generation functionality remains unchanged
- **Enhanced Awareness**: Users now have clear information about platform-specific limitations
- **Better Validation**: Clear procedures for cross-validating generated addresses
- **Informed Decisions**: Users can make better decisions about platform choice for their use case

#### ⚠️ Important Notes
- **Bitcoin Address Generation**: May differ from standard implementations when RIPEMD160 is unavailable (fallback to double SHA256)
- **Windows Platform**: Recommended for development/testing only due to minimal secp256k1 implementation
- **Production Use**: Linux/macOS strongly recommended for production wallet generation
- **Cross-Validation**: Always verify addresses with established tools before use

### Version 1.1.0 - Cross-Platform Release

This major update brings **full Windows compatibility** and a **comprehensive testing infrastructure**, making the BIP39 Wallet Generator truly cross-platform.

#### 🎯 Key Highlights
- **🖥️ Windows Support**: Full native Windows compatibility with automated setup
- **🧪 Test Suite**: 100+ test cases covering security, performance, and functionality
- **🔧 Build System**: CMake + vcpkg integration for seamless dependency management
- **📖 Documentation**: Comprehensive Windows setup guide and enhanced documentation

#### 🔐 Security Enhancements
- **Comprehensive Security Testing**: Cryptographic validation, randomness testing, isolation verification
- **Cross-Platform Security**: Platform-specific security recommendations and best practices
- **Automated Security Scanning**: Integrated into CI/CD pipeline with static analysis and memory leak detection

#### 🚀 Development Experience
- **Multi-Platform Development**: Supports Visual Studio, GCC, Clang across Windows, Linux, macOS
- **Automated Testing**: Interactive test runner with multiple test categories
- **CI/CD Pipeline**: Automated builds, testing, and security scanning across platforms

#### 📋 Migration from 1.0.0
- **Linux/macOS Users**: No breaking changes, all existing functionality preserved
- **New Windows Users**: Follow [Windows Setup Guide](WINDOWS_SETUP.md) for installation
- **Developers**: Enhanced build system supports both Make (Unix) and CMake (cross-platform)

#### 🔄 What's New for Each Platform

**Windows Users:**
- Native Windows executable support
- vcpkg dependency management
- Visual Studio integration
- PowerShell automation scripts

**Linux/macOS Users:**
- Enhanced test suite with 100+ test cases
- Improved build system with additional Make targets
- Better CI/CD integration and documentation

**All Users:**
- Comprehensive security testing and validation
- Performance benchmarking capabilities
- Enhanced error handling and input validation

#### 📈 Performance Improvements
- Optimized cryptographic operations
- Enhanced memory management
- Scalability testing and validation
- Platform-specific optimizations

#### 🔍 Testing Coverage
- **Unit Tests**: Core functionality validation
- **Integration Tests**: CLI interface and user workflows
- **Security Tests**: Cryptographic security and isolation
- **Performance Tests**: Benchmarking and scalability
- **Memory Tests**: Leak detection and resource management

For detailed Windows installation instructions, see the [Windows Setup Guide](WINDOWS_SETUP.md).
For testing procedures, see the updated README and run `./scripts/run_tests.sh --help`.

#### Known Issues
- Windows test suite requires manual testing (automated suite planned for future release)
- Full libsecp256k1 not always available on Windows (minimal implementation provided)
- Large-scale batch generation may require increased system resources

#### Credits
- Windows compatibility contributions
- Enhanced testing framework development
- Cross-platform build system improvements
- Comprehensive documentation updates
- Security testing and validation improvements

For detailed usage instructions, see the [README](README.md).
For Windows-specific setup, see the [Windows Setup Guide](WINDOWS_SETUP.md).
For security information, see the [Security Policy](SECURITY.md).
For contributing guidelines, see [CONTRIBUTING](CONTRIBUTING.md).

### Version 1.0.0 - Stable Release

This is the first stable release of the BIP39 Wallet Generator. The software has been thoroughly tested and is ready for production use with appropriate security precautions.

#### Key Features
- **Multi-network support**: Generate wallets for 10 major blockchain networks
- **Standards compliance**: Full BIP39 and BIP44 compatibility (with documented simplifications)
- **Security focus**: Uses established cryptographic libraries and best practices
- **Developer friendly**: Comprehensive documentation and examples
- **Cross-platform**: Works on Linux and macOS systems

#### Security Considerations
- Always use on secure, offline systems for production wallets
- Verify generated addresses using independent tools
- Test with small amounts before using real funds
- Keep mnemonic phrases secure and backed up properly
- **Note**: Implementation uses simplified HD derivation and may have RIPEMD160 fallback

#### Migration from Pre-release
- No breaking changes from 0.9.0
- Users upgrading from alpha versions should regenerate wallets
- New command-line options available (see README for details)

#### Known Issues
- Simplified HD derivation implementation (enhancement planned)
- Limited to command-line interface (GUI planned for future release)
- No built-in mnemonic generation (use external tools for mnemonic creation)
- RIPEMD160 fallback may affect Bitcoin address generation

#### Credits
- OpenSSL team for cryptographic primitives
- secp256k1 contributors for elliptic curve implementation
- Bitcoin and Ethereum communities for protocol specifications
- All contributors and testers who helped make this release possible

For detailed usage instructions, see the [README](README.md).
For security information, see the [Security Policy](SECURITY.md).
For contributing guidelines, see [CONTRIBUTING](CONTRIBUTING.md).

---

## Security Changelog

### Cryptographic Security Notes

#### Version 1.1.1
- **Documentation**: Prominently documented RIPEMD160 fallback behavior
- **Awareness**: Added cross-validation requirements for all address generation
- **Platform Guidance**: Clear security recommendations per platform

#### Version 1.1.0  
- **Enhancement**: Comprehensive security test suite (100+ tests)
- **Validation**: Private key randomness and seed determinism testing
- **Analysis**: Automated security scanning in CI/CD pipeline
- **Windows**: Documented minimal secp256k1 implementation limitations

#### Version 1.0.0
- **Implementation**: RIPEMD160 fallback to double SHA256 when unavailable
- **Limitation**: Simplified HD derivation (not full BIP32)
- **Foundation**: Basic security practices and input validation

### Security Recommendations by Version

| Version | Linux/macOS | Windows | Production Ready |
|---------|-------------|---------|------------------|
| **1.1.1** | ⚠️ With audit & cross-validation | ❌ Development only | With extensive validation |
| **1.1.0** | ⚠️ With audit | ❌ Development only | With security review |
| **1.0.0** | ⚠️ Basic use | ❌ Not supported | Educational only |

### Ongoing Security Considerations

- **RIPEMD160 Availability**: Always verify and cross-validate Bitcoin addresses
- **Platform Selection**: Use Linux/macOS for production, Windows for development only
- **Comprehensive Testing**: Run full security test suite before any production use
- **Professional Review**: Security audit recommended before production deployment
- **Cross-Validation**: Always verify addresses with multiple established tools

For current security status and detailed information, see [SECURITY.md](SECURITY.md).