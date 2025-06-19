# BIP39 Multi-Network Wallet Generator

A fast and secure C++ implementation for generating cryptocurrency wallets from BIP39 mnemonic phrases. Supports the top 10 blockchain networks with customizable derivation paths and multiple output formats.

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

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Disclaimer

This software is provided "as is" without warranty of any kind. Users are responsible for the security of their private keys and funds. The authors are not liable for any losses or damages resulting from the use of this software.

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
- **Bug Reports**: Use GitHub Issues
- **Feature Requests**: Use GitHub Issues with feature request template

---

**⚠️ Use at your own risk. Always verify generated addresses and test with small amounts first.**