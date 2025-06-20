# BIP39 Multi-Network Wallet Generator

A fast and secure C++ implementation for generating cryptocurrency wallets from BIP39 mnemonic phrases. Supports the top 10 blockchain networks with customizable derivation paths and multiple output formats.

## 🚨 Critical Security Warnings

⚠️ **RIPEMD160 FALLBACK WARNING**: This implementation falls back to double SHA256 when RIPEMD160 is unavailable (common on some systems). While this produces valid addresses, they may differ from standard Bitcoin address generation. For production Bitcoin wallets, ensure proper RIPEMD160 support or validate with established tools.

⚠️ **WINDOWS CRYPTOGRAPHIC LIMITATIONS**: Windows implementation uses minimal secp256k1 and may have reduced security guarantees. Use Linux/macOS for production wallet generation.

⚠️ **EDUCATIONAL PURPOSE**: This is an educational and development tool. See [Production Readiness](#production-readiness) section before using with real funds.

## Features

- 🔐 **BIP39 Compliant** - Standard mnemonic to seed conversion
- 🌐 **Multi-Network Support** - Bitcoin, Ethereum, BSC, Polygon, Avalanche, Solana, Cardano, Litecoin, Dogecoin, Tron
- 🔑 **Complete Key Information** - Private key, public key, address, and WIF format
- 🛣️ **Customizable Derivation** - Support for custom HD derivation paths
- 🔢 **Batch Generation** - Generate multiple wallets in one execution
- 🔒 **Passphrase Support** - Optional BIP39 passphrase protection
- ⚡ **High Performance** - Native C++ implementation with optimized cryptographic libraries
- 🖥️ **Cross-Platform** - Linux, macOS, and Windows support
- 🧪 **Comprehensive Testing** - 100+ test cases covering security, performance, and functionality

## Platform Support & Implementation

| Platform | Source Files | secp256k1 | RIPEMD160 | Test Suite | Production Ready |
|----------|-------------|-----------|-----------|------------|------------------|
| **Linux/macOS** | `wallet_generator.cpp` + `wallet_generator_impl.cpp` | Full library | Native/Fallback | Complete (100+ tests) | ⚠️ With audit |
| **Windows** | `wallet_generator_win.cpp` | Minimal implementation | SHA256 fallback | Manual testing | ❌ Development only |

### Platform-Specific Notes
- **Linux/macOS**: Full cryptographic library support with comprehensive testing
- **Windows**: Uses compatibility layers (`windows/getopt_win.h`, `windows/minimal_secp256k1.h`) 
- **All Platforms**: Simplified HD derivation (not full BIP32 implementation)

## Supported Networks

| Network | Symbol | Derivation Path | Address Format | Status |
|---------|--------|----------------|----------------|--------|
| Bitcoin | BTC | m/44'/0'/0'/0/0 | Base58 (P2PKH) | ⚠️ RIPEMD160 fallback |
| Ethereum | ETH | m/44'/60'/0'/0/0 | Hex (0x...) | ✅ Full support |
| Binance Smart Chain | BSC | m/44'/60'/0'/0/0 | Hex (0x...) | ✅ Full support |
| Polygon | MATIC | m/44'/60'/0'/0/0 | Hex (0x...) | ✅ Full support |
| Avalanche | AVAX | m/44'/60'/0'/0/0 | Hex (0x...) | ✅ Full support |
| Solana | SOL | m/44'/501'/0'/0/0 | Base58 | ✅ Full support |
| Cardano | ADA | m/44'/1815'/0'/0/0 | Base58 | ✅ Full support |
| Litecoin | LTC | m/44'/2'/0'/0/0 | Base58 (P2PKH) | ⚠️ RIPEMD160 fallback |
| Dogecoin | DOGE | m/44'/3'/0'/0/0 | Base58 (P2PKH) | ⚠️ RIPEMD160 fallback |
| Tron | TRX | m/44'/195'/0'/0/0 | Base58 | ✅ Full support |

## Quick Start

### Linux / macOS

```bash
# Install dependencies
# Ubuntu/Debian:
sudo apt-get install libssl-dev libsecp256k1-dev build-essential
# macOS:
brew install openssl secp256k1

# Clone and build
git clone https://github.com/CiDsTaR/bip39-wallet-generator.git
cd bip39-wallet-generator
make

# Generate a Bitcoin wallet
./wallet_generator "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
```

### Windows

Windows installation requires additional setup. Please see the comprehensive **[Windows Setup Guide](WINDOWS_SETUP.md)** for detailed instructions.

**Quick Windows Install:**
```batch
# Run as Administrator
PowerShell -ExecutionPolicy Bypass -File scripts\install_windows_deps.ps1
scripts\build_windows.bat

# Use the wallet generator
build\Release\wallet_generator.exe "your mnemonic phrase here"
```

## Installation

### Prerequisites

**All Platforms:**
- C++17 compatible compiler
- OpenSSL development libraries

**Linux/macOS Specific:**
- Make (for Makefile build)
- libsecp256k1 development libraries

**Windows Specific:**
- Visual Studio 2019/2022 or Build Tools
- CMake 3.15+
- See [Windows Setup Guide](WINDOWS_SETUP.md) for detailed requirements

### Linux/macOS Installation

1. **Install dependencies**
```bash
# Ubuntu/Debian
make install-deps-ubuntu

# macOS
make install-deps-macos
```

2. **Clone and build**
```bash
git clone https://github.com/CiDsTaR/bip39-wallet-generator.git
cd bip39-wallet-generator
make  # Compiles: wallet_generator.cpp + wallet_generator_impl.cpp
```

### Windows Installation

For Windows installation, please follow the **[Windows Setup Guide](WINDOWS_SETUP.md)** which covers:
- Automated dependency installation
- Manual setup procedures
- Visual Studio configuration
- vcpkg package management
- Troubleshooting common issues

## Usage

### Basic Usage

```bash
# Linux/macOS
./wallet_generator "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Windows
wallet_generator.exe "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
```

### Advanced Usage

```bash
# Generate Ethereum wallet
./wallet_generator -n ethereum "your twelve word mnemonic phrase goes here"

# Generate 5 wallets for Ethereum
./wallet_generator -n ethereum -c 5 "your mnemonic phrase"

# Generate wallets for all supported networks
./wallet_generator -a "your mnemonic phrase"

# Use custom derivation path
./wallet_generator -n bitcoin -p "m/44'/0'/0'/0/10" "your mnemonic phrase"

# Use BIP39 passphrase
./wallet_generator -P "your_passphrase" "your mnemonic phrase"

# Verbose output (shows public key and WIF)
./wallet_generator -v -n ethereum "your mnemonic phrase"
```

### Command Line Options

| Option | Long Form | Description |
|--------|-----------|-------------|
| `-n` | `--network` | Specify network (bitcoin, ethereum, binance, etc.) |
| `-c` | `--count` | Number of wallets to generate (default: 1) |
| `-p` | `--path` | Custom derivation path |
| `-P` | `--passphrase` | BIP39 passphrase (optional) |
| `-a` | `--all-networks` | Generate for all supported networks |
| `-v` | `--verbose` | Show complete information |
| `-h` | `--help` | Show help message |

## Example Output

```
=== ethereum Wallet ===
Derivation Path: m/44'/60'/0'/0/0
Private Key: c87509a1c067bbde78beb793e6fa76530b6382a4c0241e5e4a9ec0a0f44dc0d3
Public Key: 04ae1a62fe09c5f51b13905f07f06b99a2f7159b2225f374cd378d71302fa28414e83f72ad8278fc93b79df7ff1e2b40b2e9fe0e62c4e0d5eabac32c2ce8c6bb6b
WIF: KzJDe7iwDJ1K2ESMFAKCahvPrh1GBJSXJ1WnSSLx9K4i8M5Y3oB7
Address: 0x9858EfFD232B4033E47d90003D41EC34EcaEda94
```

## Development and Testing

### Linux/macOS - Comprehensive Test Suite (100+ Tests)

```bash
# Complete test suite
make test-all                    # All test categories
./scripts/run_tests.sh --all     # Interactive test runner
./scripts/run_tests.sh --quick   # Basic functionality only

# Individual test categories  
make test-unit                   # Core functionality (50+ tests)
make test-integration           # CLI interface testing
make test-security              # Cryptographic validation
make test-performance           # Performance benchmarking

# Advanced testing
make coverage                   # Code coverage analysis
make memtest                    # Memory leak detection (requires valgrind)
make static-analysis            # Static code analysis (requires cppcheck)

# Interactive test runner with options
./scripts/run_tests.sh --help
./scripts/run_tests.sh --security    # Security tests only
./scripts/run_tests.sh --coverage    # With coverage analysis
```

### Windows - Limited Testing

```batch
# Manual testing only (automated suite requires WSL)
build\Release\wallet_generator.exe "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Basic test script
copy scripts\run_tests.bat build\Release\
cd build\Release
.\run_tests.bat --basic
.\run_tests.bat --all
```

**Note**: Windows automated testing is planned for future releases. Current testing requires manual verification.

### Project Structure

```
bip39-wallet-generator/
├── .github/workflows/           # CI/CD pipeline
├── tests/                       # Comprehensive test suite (Linux/macOS)
│   ├── test_framework.h         # Custom test framework
│   ├── test_main.cpp           # Unit tests (50+ test cases)
│   ├── integration_test.cpp    # CLI interface testing
│   ├── security_test.cpp       # Cryptographic validation
│   ├── performance_test.cpp    # Performance benchmarking
│   └── Makefile               # Test build system
├── scripts/                     # Build and test automation
│   ├── run_tests.sh            # Interactive test runner (Unix)
│   ├── ci_test.sh              # CI/CD automation (Unix)
│   ├── build_windows.bat       # Windows build automation
│   └── run_tests.bat           # Basic Windows testing
├── windows/                     # Windows compatibility files
│   ├── getopt_win.h            # POSIX getopt implementation
│   └── minimal_secp256k1.h     # Basic secp256k1 fallback
├── wallet_generator.cpp         # Main source (Linux/macOS)
├── wallet_generator_impl.cpp    # Implementation (Linux/macOS)
├── wallet_generator_win.cpp     # Windows-compatible source
├── wallet_generator.h          # Header file
├── Makefile                    # Unix build system
├── CMakeLists.txt              # Cross-platform build system
├── WINDOWS_SETUP.md            # Windows setup guide
└── [documentation files]
```

## Security Considerations

⚠️ **CRITICAL SECURITY WARNINGS:**

### Immediate Security Requirements
- **Cross-Validate Addresses**: Always verify generated addresses with established tools (Electrum, Bitcoin Core, MetaMask)
- **Test Environment**: Use on secure, offline systems when generating production wallets
- **RIPEMD160 Verification**: Check RIPEMD160 availability: `openssl list -digest-commands | grep rmd`
- **Platform Selection**: Use Linux/macOS for production (full test coverage and cryptographic support)

### Known Cryptographic Limitations

1. **RIPEMD160 Fallback**: Uses double SHA256 when RIPEMD160 unavailable
   - **Impact**: Bitcoin-style addresses may differ from standard implementation
   - **Affected Networks**: Bitcoin, Litecoin, Dogecoin
   - **Mitigation**: Ensure OpenSSL with legacy provider support

2. **Simplified HD Derivation**: Not full BIP32 implementation
   - **Impact**: May not match hardware wallet derivation exactly
   - **Mitigation**: Cross-validate with multiple tools

3. **Windows Minimal secp256k1**: Limited cryptographic validation
   - **Impact**: Reduced security guarantees on Windows
   - **Mitigation**: Use Linux/macOS for production wallets

### Security Best Practices

- **Never share your mnemonic phrase** - Anyone with access can control your funds
- **Disable swap files** when generating production wallets: `sudo swapoff -a`
- **Clear bash history**: `history -c && history -w`
- **Test with small amounts** first
- **Keep secure backups** of mnemonic phrases
- **Run security test suite**: `make test-security`

### Cross-Validation Examples

```bash
# Validate Bitcoin addresses with Bitcoin Core
bitcoin-cli validateaddress "1BvBMSEYstWetqTFn5Au4m4GFg7xJaNVN2"

# Compare with Electrum
# Generate same mnemonic in Electrum and compare first address

# Validate Ethereum addresses with web3
# Use MetaMask or MyEtherWallet with same mnemonic
```

## Production Readiness

⚠️ **CRITICAL: This tool is designed for educational and development purposes. Before using in production environments with real funds, you MUST understand and address the following:**

### 🔒 Security Assessment Required

#### **Immediate Security Requirements**
- **Professional Code Audit**: Security review by qualified professionals
- **Cryptographic Validation**: Verify all cryptographic implementations
- **Cross-Platform Testing**: Validate outputs across multiple tools
- **Environment Hardening**: Air-gapped systems, disabled swap files
- **RIPEMD160 Verification**: Ensure proper RIPEMD160 for Bitcoin-style addresses

#### **Known Implementation Limitations**

1. **RIPEMD160 Fallback** (Bitcoin, Litecoin, Dogecoin)
   ```bash
   # Check RIPEMD160 availability
   openssl list -digest-commands | grep rmd
   ```

2. **Simplified HD Derivation** (All networks)
   ```cpp
   // Current implementation uses simplified derivation
   // For production, replace with full BIP32 implementation
   ```

3. **Platform-Specific Limitations**
   - **Windows**: Minimal secp256k1, manual testing only
   - **All**: No built-in mnemonic generation
   - **All**: No transaction building capabilities

### 🛠️ Required Production Modifications

#### **Critical Code Changes Needed**
```cpp
// 1. Implement full BIP32/BIP44 HD derivation
// 2. Add proper RIPEMD160 for Bitcoin-style addresses
// 3. Implement secure memory management
// 4. Add comprehensive input validation
// 5. Add address format validation
```

#### **Security Infrastructure Requirements**
- **Hardware Security Modules (HSMs)** for key storage
- **Multi-signature schemes** for high-value wallets
- **Cold storage protocols**
- **Disaster recovery procedures**
- **Regular security audits**

### 📋 Production Validation Checklist

- [ ] **Code Security Audit** by qualified professionals
- [ ] **Cryptographic Review** of all implementations
- [ ] **Cross-Validation** with established tools
- [ ] **RIPEMD160 Verification** for Bitcoin-style networks
- [ ] **Platform Testing** across all target systems
- [ ] **Performance Testing** under production loads
- [ ] **Security Test Suite** passes completely
- [ ] **Memory Analysis** confirms no private key leakage
- [ ] **Entropy Validation** of random number generation
- [ ] **Backup and Recovery** procedures tested

### 🚨 Production Alternatives (Recommended)

For production environments, consider these battle-tested alternatives:

#### **Hardware Wallets**
- **Ledger**: Hardware wallet with secure element
- **Trezor**: Open-source hardware wallet
- **ColdCard**: Bitcoin-specific hardware wallet

#### **Enterprise Solutions**
- **BitGo**: Multi-signature wallet service
- **Fireblocks**: Institutional digital asset platform
- **Anchorage**: Qualified custody solution

#### **Production Libraries**
- **libbitcoin**: Comprehensive Bitcoin library (C++)
- **bitcoinjs-lib**: JavaScript Bitcoin library
- **web3.py**: Python Ethereum library
- **ethers.js**: JavaScript Ethereum library

### 📖 Migration to Production

1. **Security Assessment Phase**
   - Conduct thorough security audit
   - Validate all cryptographic components
   - Document security assumptions

2. **Hardening Phase**
   - Implement secure memory management
   - Add comprehensive validation
   - Replace simplified components

3. **Testing Phase**
   - Extensive security testing
   - Cross-platform validation
   - Performance testing

4. **Deployment Phase**
   - Staged rollout with monitoring
   - Incident response procedures
   - Continuous security assessment

## Troubleshooting

### Common Issues

#### RIPEMD160 Not Available
```bash
# Ubuntu/Debian
sudo apt-get install openssl libssl-dev

# Check availability
openssl list -digest-commands | grep rmd

# If missing, addresses will use SHA256 fallback
```

#### Build Errors
```bash
# Missing dependencies
make install-deps-ubuntu  # or install-deps-macos

# Clean build
make clean && make

# CMake build (alternative)
mkdir build && cd build
cmake .. && make
```

#### Test Failures
```bash
# Run specific test categories
make test-unit
make test-security

# Check system compatibility
./scripts/run_tests.sh --quick
```

#### Windows-Specific Issues
See [Windows Setup Guide](WINDOWS_SETUP.md) for detailed troubleshooting.

## Contributing

We welcome contributions! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

### Development Setup

**Linux/macOS:**
```bash
git clone https://github.com/CiDsTaR/bip39-wallet-generator.git
cd bip39-wallet-generator
make install-deps-ubuntu  # or install-deps-macos
make
make test-all
```

**Windows:**
See [Windows Setup Guide](WINDOWS_SETUP.md) for development environment setup.

## Roadmap

### Completed ✅
- [x] Cross-platform Windows support
- [x] Comprehensive test suite (100+ tests)
- [x] CI/CD pipeline
- [x] Multi-network support (10 networks)
- [x] Complete documentation

### In Progress 🔄
- [ ] Native Windows test automation
- [ ] Production-ready secure memory management
- [ ] Full BIP32/BIP44 HD derivation implementation

### Planned 📋
- [ ] GUI interface
- [ ] Docker containerization
- [ ] Hardware wallet integration
- [ ] Additional networks (Cosmos, Polkadot, etc.)
- [ ] Mnemonic phrase validation and generation
- [ ] Bech32 address support for Bitcoin
- [ ] Hardware Security Module (HSM) integration

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Disclaimer

This software is provided "as is" without warranty of any kind. Users are responsible for the security of their private keys and funds. The authors are not liable for any losses or damages resulting from the use of this software.

**This tool is designed for educational and development purposes. For production use with real funds, conduct thorough security audits and implement additional security measures as outlined in the [Production Readiness](#production-readiness) section.**

## Acknowledgments

- [BIP39 Specification](https://github.com/bitcoin/bips/blob/master/bip-0039.mediawiki)
- [BIP44 Specification](https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki)
- [libsecp256k1](https://github.com/bitcoin-core/secp256k1)
- [OpenSSL](https://www.openssl.org/)

## Support

If you find this project helpful, consider:
- ⭐ Starring the repository
- 🐛 Reporting issues
- 🔧 Contributing improvements
- 📖 Improving documentation

### Getting Help

- **General Usage**: Check this README and run `./wallet_generator --help`
- **Windows Setup**: See [Windows Setup Guide](WINDOWS_SETUP.md)
- **Contributing**: See [CONTRIBUTING.md](CONTRIBUTING.md)
- **Security Issues**: See [SECURITY.md](SECURITY.md)
- **Production Deployment**: Review [Production Readiness](#production-readiness) section
- **Bug Reports**: Use GitHub Issues
- **Feature Requests**: Use GitHub Issues with feature request template

---

**⚠️ Use at your own risk. Always verify generated addresses and test with small amounts first. For production use, implement additional security measures and conduct thorough security audits.**