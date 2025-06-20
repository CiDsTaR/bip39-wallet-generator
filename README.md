# BIP39 Multi-Network Wallet Generator

A fast and secure C++ implementation for generating cryptocurrency wallets from BIP39 mnemonic phrases. Supports the top 10 blockchain networks with customizable derivation paths and multiple output formats.

⚠️ **IMPORTANT**: This is an educational and development tool. See [Production Readiness](#production-readiness) section before using with real funds.

## 💝 Support This Project

If you find this project helpful, consider supporting development:

**Bitcoin (BTC):** `bc1q6zq385fgej7dg6fy2p63lk6jrp3vrk6qln2vnd`

**EVM-compatible networks (ETH/BNB or any other):** `0xB3a1A708C98AE07BD024c2A60a81e07D189B5390`


## Features

- 🔐 **BIP39 Compliant** - Standard mnemonic to seed conversion
- 🌐 **Multi-Network Support** - Bitcoin, Ethereum, BSC, Polygon, Avalanche, Solana, Cardano, Litecoin, Dogecoin, Tron
- 🔑 **Complete Key Information** - Private key, public key, address, and WIF format
- 🛣️ **Customizable Derivation** - Support for custom HD derivation paths
- 🔢 **Batch Generation** - Generate multiple wallets in one execution
- 🔒 **Passphrase Support** - Optional BIP39 passphrase protection
- ⚡ **High Performance** - Native C++ implementation with optimized cryptographic libraries
- 🖥️ **Cross-Platform** - Linux, macOS, and Windows support

## Platform Support

| Platform | Status | Build System | Notes |
|----------|--------|--------------|-------|
| **Linux** | ✅ Full Support | Make / CMake | Complete test suite |
| **macOS** | ✅ Full Support | Make / CMake | Complete test suite |
| **Windows** | ✅ Full Support | CMake | See [Windows Setup Guide](WINDOWS_SETUP.md) |

## Supported Networks

| Network | Symbol | Derivation Path | Address Format |
|---------|--------|----------------|----------------|
| Bitcoin | BTC | m/44'/0'/0'/0/0 | Base58 (P2PKH) |
| Ethereum | ETH | m/44'/60'/0'/0/0 | Hex (0x...) |
| Binance Smart Chain | BSC | m/44'/60'/0'/0/0 | Hex (0x...) |
| Polygon | MATIC | m/44'/60'/0'/0/0 | Hex (0x...) |
| Avalanche | AVAX | m/44'/60'/0'/0/0 | Hex (0x...) |
| Solana | SOL | m/44'/501'/0'/0/0 | Base58 |
| Cardano | ADA | m/44'/1815'/0'/0/0 | Base58 |
| Litecoin | LTC | m/44'/2'/0'/0/0 | Base58 (P2PKH) |
| Dogecoin | DOGE | m/44'/3'/0'/0/0 | Base58 (P2PKH) |
| Tron | TRX | m/44'/195'/0'/0/0 | Base58 |

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
git https://github.com/CiDsTaR/bip39-wallet-generator.git
cd bip39-wallet-generator
make
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

## Production Readiness

⚠️ **CRITICAL: This tool is designed for educational and development purposes. Before using in production environments with real funds, you MUST understand and address the following:**

### 🔒 Security Considerations

#### **Immediate Security Requirements**
- **Air-gapped Systems**: Generate production wallets only on completely offline, air-gapped computers
- **Secure Boot Media**: Use a clean, live Linux distribution (like Tails) from read-only media
- **No Network**: Disconnect all network interfaces during key generation
- **Secure Disposal**: Physically destroy or securely wipe any storage used during generation

#### **Environment Hardening**
```bash
# Disable swap to prevent private keys being written to disk
sudo swapoff -a

# Clear bash history before and after use
history -c && history -w

# Use secure memory allocation if available
ulimit -c 0  # Disable core dumps
```

### 🛠️ Production Implementation Requirements

#### **Critical Cryptographic Considerations**

1. **RIPEMD160 Fallback Warning**
   ```cpp
   // Current implementation falls back to double SHA256 when RIPEMD160 unavailable
   // For Bitcoin production wallets, ensure proper RIPEMD160:
   
   // Install OpenSSL with legacy provider support:
   sudo apt-get install openssl libssl-dev
   
   // Or compile with specific RIPEMD160 support
   ```

2. **Simplified HD Derivation**
   ```cpp
   // Current implementation uses simplified derivation
   // For production, replace with full BIP32/BIP44 implementation:
   
   // Recommended libraries:
   // - libbitcoin (C++)
   // - bitcoinj (Java)
   // - python-bitcoinlib (Python)
   ```

3. **Enhanced Entropy**
   ```cpp
   // Add additional entropy sources for production:
   // - Hardware RNG
   // - Multiple entropy pools
   // - Entropy validation
   ```

#### **Code Modifications for Production**

**1. Secure Memory Management**
```cpp
#include <sodium.h>  // Add libsodium for secure memory

class SecureWalletGenerator {
private:
    // Use secure memory allocation
    void* secure_malloc(size_t size) {
        return sodium_malloc(size);
    }
    
    void secure_free(void* ptr) {
        sodium_free(ptr);
    }
    
    // Clear sensitive data
    void secure_zero(void* ptr, size_t size) {
        sodium_memzero(ptr, size);
    }
};
```

**2. Enhanced Validation**
```cpp
// Add comprehensive input validation
bool validateMnemonic(const std::string& mnemonic) {
    // Implement BIP39 wordlist validation
    // Check checksum
    // Validate entropy
    return true;
}

// Add address validation
bool validateAddress(const std::string& address, const std::string& network) {
    // Implement network-specific address validation
    return true;
}
```

**3. Audit Logging**
```cpp
// Add secure audit logging (without sensitive data)
void logOperation(const std::string& operation, bool success) {
    // Log to secure audit trail
    // Never log private keys or mnemonics
}
```

### 🔍 Testing and Validation

#### **Pre-Production Checklist**
- [ ] **Code Audit**: Professional security code review
- [ ] **Cryptographic Audit**: Validate all cryptographic implementations
- [ ] **Test Vectors**: Validate against official BIP39/BIP44 test vectors
- [ ] **Cross-Validation**: Compare outputs with established wallet implementations
- [ ] **Stress Testing**: Test with various input edge cases
- [ ] **Memory Analysis**: Verify no private keys remain in memory/swap

#### **Validation Commands**
```bash
# Run comprehensive security tests
./scripts/run_tests.sh --security

# Validate against known test vectors
./wallet_generator -v "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Cross-validate with other tools (recommended)
# Compare outputs with: electrum, bitcoin-cli, ethereumjs-wallet, etc.
```

### 📋 Production Deployment Guidelines

#### **Infrastructure Requirements**
1. **Hardware Security Modules (HSMs)** for key storage
2. **Secure enclaves** for key generation
3. **Multi-signature schemes** for high-value wallets
4. **Cold storage** protocols
5. **Disaster recovery** procedures

#### **Operational Security**
1. **Principle of Least Privilege**: Minimal system access
2. **Segregation of Duties**: Multiple people for critical operations
3. **Regular Security Updates**: Keep all dependencies current
4. **Incident Response Plan**: Procedures for security breaches
5. **Regular Backups**: Secure, tested backup procedures

#### **Compliance Considerations**
- **Regulatory Requirements**: Know your local cryptocurrency regulations
- **KYC/AML**: Implement required customer verification
- **Data Protection**: GDPR, CCPA compliance for user data
- **Financial Regulations**: Banking and financial service compliance
- **Audit Requirements**: Maintain audit trails and documentation

### 🚨 Known Limitations

#### **Current Implementation Limitations**
1. **Simplified HD Derivation**: Not full BIP32 implementation
2. **Limited Address Types**: Only supports legacy P2PKH for Bitcoin
3. **No Hardware Wallet Support**: Software-only implementation
4. **Basic Error Handling**: Limited error recovery mechanisms
5. **No Built-in Backup**: No automatic backup/recovery features

#### **Network-Specific Limitations**
- **Bitcoin**: Only P2PKH addresses (no SegWit/P2SH)
- **Ethereum**: Basic address generation (no ENS support)
- **Solana**: Simplified implementation
- **All Networks**: No transaction building capabilities

### 🛡️ Recommended Production Alternatives

For production environments, consider these battle-tested alternatives:

#### **Hardware Wallets**
- **Ledger**: Hardware wallet with secure element
- **Trezor**: Open-source hardware wallet
- **ColdCard**: Bitcoin-specific hardware wallet

#### **Enterprise Solutions**
- **BitGo**: Multi-signature wallet service
- **Fireblocks**: Institutional digital asset platform
- **Anchorage**: Qualified custody solution

#### **Libraries for Production Development**
- **libbitcoin**: Comprehensive Bitcoin library
- **bitcoinjs-lib**: JavaScript Bitcoin library
- **web3.py**: Python Ethereum library
- **ethers.js**: JavaScript Ethereum library

### 📖 Production Migration Path

#### **Phase 1: Security Assessment**
1. Conduct thorough security audit
2. Identify all cryptographic components
3. Validate against industry standards
4. Document all security assumptions

#### **Phase 2: Hardening**
1. Implement secure memory management
2. Add comprehensive input validation
3. Enhance error handling and logging
4. Implement proper HD derivation

#### **Phase 3: Testing**
1. Extensive security testing
2. Penetration testing
3. Code coverage analysis
4. Performance testing under load

#### **Phase 4: Deployment**
1. Staged rollout with monitoring
2. Incident response procedures
3. Regular security assessments
4. Continuous monitoring and updates

---

## Development and Testing

### Running Tests (Linux/macOS)

```bash
# Run all tests
make test-all

# Run specific test categories
make test-unit          # Unit tests
make test-integration   # Integration tests
make test-security      # Security tests
make test-performance   # Performance tests

# Advanced testing
make coverage          # Code coverage analysis
make memtest          # Memory leak detection
make static-analysis  # Static code analysis
```

### Using the Test Runner

```bash
# Interactive test runner
./scripts/run_tests.sh --help
./scripts/run_tests.sh --all
./scripts/run_tests.sh --quick
./scripts/run_tests.sh --security
```

### Windows Testing

Windows testing is currently limited to manual testing. The full automated test suite requires WSL (Windows Subsystem for Linux). See [Windows Setup Guide](WINDOWS_SETUP.md) for testing procedures.

### Project Structure

```
bip39-wallet-generator/
├── .github/workflows/           # CI/CD pipeline
├── tests/                       # Comprehensive test suite (Linux/macOS)
├── scripts/                     # Build and test automation
├── windows/                     # Windows compatibility files
├── wallet_generator.cpp         # Main source (Linux/macOS)
├── wallet_generator_win.cpp     # Windows-compatible source
├── Makefile                     # Unix build system
├── CMakeLists.txt              # Cross-platform build system
├── WINDOWS_SETUP.md            # Windows setup guide
└── [documentation files]
```

## Security Considerations

⚠️ **IMPORTANT SECURITY NOTES:**

- **Never share your mnemonic phrase** - Anyone with access to it can control your funds
- **Use on secure, offline systems** when generating production wallets
- **Verify addresses** before sending any funds
- **Test with small amounts** first
- **Keep backups** of your mnemonic phrase in secure locations
- **This tool is for educational/development purposes** - audit the code before production use

### Platform-Specific Security Notes

- **Linux/macOS**: Full security testing available via test suite
- **Windows**: Uses minimal secp256k1 implementation when full library unavailable
- **All platforms**: Disable swap files when generating production wallets

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

- [x] Cross-platform Windows support
- [x] Comprehensive test suite
- [x] CI/CD pipeline
- [ ] Add support for more networks (Cosmos, Polkadot, etc.)
- [ ] Implement Bech32 address generation for Bitcoin
- [ ] Add mnemonic phrase validation
- [ ] Support for custom word lists
- [ ] Hardware wallet integration
- [ ] GUI interface
- [ ] Docker containerization
- [ ] Native Windows test suite
- [ ] Production-ready secure memory management
- [ ] Full BIP32/BIP44 HD derivation implementation
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