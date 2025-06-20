# Windows Setup Guide

This guide explains how to build and run the BIP39 Wallet Generator on Windows systems.

## 🚨 Important Windows Security Notice

⚠️ **Windows Cryptographic Limitations**: The Windows implementation uses a minimal secp256k1 implementation and may have reduced security guarantees compared to Linux/macOS. This version is recommended for **development and testing only**. For production wallet generation, use Linux/macOS systems.

⚠️ **RIPEMD160 Fallback**: Windows builds commonly fall back to SHA256 for Bitcoin-style addresses. Always cross-validate addresses with established tools.

## Quick Start (Recommended)

### Option 1: Automated Installation

1. **Download and extract** the project
2. **Run as Administrator**: Right-click on PowerShell and select "Run as Administrator"
3. **Navigate to project folder**:
   ```powershell
   cd path\to\bip39-wallet-generator
   ```
4. **Install dependencies**:
   ```powershell
   PowerShell -ExecutionPolicy Bypass -File scripts\install_windows_deps.ps1
   ```
5. **Build the project**:
   ```batch
   scripts\build_windows.bat
   ```

### Option 2: Using vcpkg (Manual)

1. **Install Visual Studio 2019/2022** with C++ support
2. **Install Git** from https://git-scm.com/
3. **Install CMake** from https://cmake.org/
4. **Install vcpkg**:
   ```batch
   cd C:\
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   .\bootstrap-vcpkg.bat
   .\vcpkg.exe integrate install
   ```
5. **Install OpenSSL**:
   ```batch
   .\vcpkg.exe install openssl:x64-windows
   ```
6. **Build the project**:
   ```batch
   mkdir build && cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
   cmake --build . --config Release
   ```

## Requirements

### Essential Requirements
- **Windows 10/11** (64-bit recommended)
- **Visual Studio 2019 or 2022** (Community edition is sufficient)
  - Must include "Desktop development with C++" workload
  - Windows 10/11 SDK
- **CMake 3.15+**
- **Git for Windows**

### Dependencies
- **OpenSSL** (for cryptographic functions)
- **secp256k1** (optional, will use minimal implementation if not available)

## Installation Methods

### Method 1: vcpkg (Recommended)

vcpkg is Microsoft's C++ package manager that simplifies dependency management.

```batch
# Install vcpkg (run as Administrator)
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg.exe integrate install

# Install dependencies
.\vcpkg.exe install openssl:x64-windows
.\vcpkg.exe install secp256k1:x64-windows  # Optional - may not be available

# Build project
cd your\project\directory
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

### Method 2: Manual Installation

1. **Download OpenSSL** from https://slproweb.com/products/Win32OpenSSL.html
   - Choose "Win64 OpenSSL v3.x.x" (latest version)
   - Install to default location: `C:\Program Files\OpenSSL-Win64\`

2. **Set environment variables**:
   ```batch
   set OPENSSL_ROOT_DIR=C:\Program Files\OpenSSL-Win64
   set OPENSSL_INCLUDE_DIR=C:\Program Files\OpenSSL-Win64\include
   set OPENSSL_CRYPTO_LIBRARY=C:\Program Files\OpenSSL-Win64\lib\libcrypto.lib
   set OPENSSL_SSL_LIBRARY=C:\Program Files\OpenSSL-Win64\lib\libssl.lib
   ```

3. **Build project**:
   ```batch
   mkdir build && cd build
   cmake .. -DCMAKE_BUILD_TYPE=Release
   cmake --build . --config Release
   ```

### Method 3: Chocolatey

If you prefer using Chocolatey package manager:

```batch
# Install Chocolatey (run as Administrator)
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Install dependencies
choco install git cmake visualstudio2022buildtools visualstudio2022-workload-vctools -y

# Install vcpkg for OpenSSL
choco install vcpkg -y
```

## Building

### Using the Build Script (Recommended)

```batch
# Ensure you're in the project root directory
scripts\build_windows.bat
```

### Manual Build with CMake

```batch
# Create build directory
mkdir build
cd build

# Configure (with vcpkg)
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake -DCMAKE_BUILD_TYPE=Release

# Configure (without vcpkg, manual OpenSSL)
cmake .. -DCMAKE_BUILD_TYPE=Release -DOPENSSL_ROOT_DIR="C:\Program Files\OpenSSL-Win64"

# Build
cmake --build . --config Release
```

### Alternative: Using Visual Studio IDE

```batch
# Generate Visual Studio solution
cmake .. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake

# Open in Visual Studio
start BIP39WalletGenerator.sln
```

## Windows-Specific Implementation Details

### Architecture Differences

| Component | Windows Implementation | Notes |
|-----------|----------------------|-------|
| **Source File** | `wallet_generator_win.cpp` | Standalone Windows-compatible implementation |
| **Command Line Parsing** | `windows/getopt_win.h` | Custom POSIX getopt implementation |
| **secp256k1** | `windows/minimal_secp256k1.h` | Minimal implementation using OpenSSL EC |
| **RIPEMD160** | SHA256 fallback | Uses double SHA256 when RIPEMD160 unavailable |

### Cryptographic Considerations

```cpp
// Windows implementation uses:
#ifdef USE_MINIMAL_SECP256K1
    // Minimal secp256k1 implementation
    // Reduced cryptographic validation
    // OpenSSL EC functions as fallback
#endif

// RIPEMD160 handling:
const EVP_MD* ripemd160_md = EVP_ripemd160();
if (!ripemd160_md) {
    // Falls back to double SHA256
    // May produce different Bitcoin addresses
}
```

## Usage

After successful build, the executable will be located at:
- `build\Release\wallet_generator.exe` (Release build)
- `build\Debug\wallet_generator.exe` (Debug build)

### Basic Usage

```batch
# Show help
wallet_generator.exe -h

# Generate Bitcoin wallet
wallet_generator.exe "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Generate Ethereum wallet
wallet_generator.exe -n ethereum "your mnemonic phrase here"

# Generate for all networks
wallet_generator.exe -a "your mnemonic phrase here"
```

### Advanced Usage

```batch
# Multiple wallets
wallet_generator.exe -n ethereum -c 5 "your mnemonic phrase"

# Custom derivation path
wallet_generator.exe -n bitcoin -p "m/44'/0'/0'/0/10" "your mnemonic phrase"

# With passphrase
wallet_generator.exe -P "your_passphrase" "your mnemonic phrase"

# Verbose output
wallet_generator.exe -v "your mnemonic phrase"
```

## Testing on Windows

### Automated Testing (Limited)

```batch
# Copy test script to build directory
copy scripts\run_tests.bat build\Release\
cd build\Release

# Run basic tests
.\run_tests.bat --basic

# Run comprehensive tests
.\run_tests.bat --all
```

### Manual Testing

```batch
# Test basic functionality
wallet_generator.exe "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Test all networks
wallet_generator.exe -a "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Test error handling
wallet_generator.exe
wallet_generator.exe -n invalid_network "test"

# Cross-validation test (compare with known outputs)
wallet_generator.exe -v -n ethereum "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
```

### Cross-Platform Validation

Since Windows implementation may differ from Linux/macOS, always cross-validate:

```batch
# Generate test wallet
wallet_generator.exe -n bitcoin "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Expected Bitcoin address (from BIP44 test vectors): 1LqBGSKuX5yYUonjxT5qGfpUsXKYYWeabA
# Compare your output with this expected value
```

## Troubleshooting

### Common Issues

#### 1. "CMake not found"
```batch
# Download and install CMake from https://cmake.org/download/
# Add CMake to PATH environment variable
# Or use Chocolatey: choco install cmake
```

#### 2. "Visual Studio compiler not found"
```batch
# Install Visual Studio 2019 or 2022 with C++ support
# Or install Build Tools: choco install visualstudio2022buildtools
# Make sure to run from "Developer Command Prompt" or "x64 Native Tools Command Prompt"
```

#### 3. "OpenSSL not found"
**Solution A: Use vcpkg (Recommended)**
```batch
vcpkg install openssl:x64-windows
```

**Solution B: Manual installation**
```batch
# Download from https://slproweb.com/products/Win32OpenSSL.html
# Set environment variables:
set OPENSSL_ROOT_DIR=C:\Program Files\OpenSSL-Win64
```

#### 4. "secp256k1 not found"
This is expected and handled automatically:
```batch
# The project will use minimal implementation automatically
# Or try: vcpkg install secp256k1:x64-windows (may not be available)
```

#### 5. "Access denied" or permission errors
```batch
# Run Command Prompt or PowerShell as Administrator
# Check Windows Defender / antivirus settings
# Add project folder to exclusions if necessary
```

#### 6. "getopt.h not found"
This is handled automatically by the Windows-compatible version:
```cpp
// wallet_generator_win.cpp includes:
#include "windows/getopt_win.h"
```

### Build Errors

#### Error: "Could not find OpenSSL"
```batch
# Solution 1: Use vcpkg
cd C:\vcpkg
.\vcpkg.exe install openssl:x64-windows

# Solution 2: Set environment variables manually
set OPENSSL_ROOT_DIR=C:\Program Files\OpenSSL-Win64
```

#### Error: "LINK : fatal error LNK1181: cannot open input file 'libssl.lib'"
```batch
# Ensure OpenSSL is properly installed
# Check paths in CMakeCache.txt
# Clear cache and reconfigure:
del CMakeCache.txt
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
```

#### Error: "MSB8020: The build tools for v142 cannot be found"
```batch
# Install Visual Studio 2019/2022 or Build Tools
choco install visualstudio2022buildtools -y
choco install visualstudio2022-workload-vctools -y

# Or specify different generator:
cmake .. -G "Visual Studio 16 2019" # For VS 2019
```

#### Error: "RIPEMD160 not available"
This is expected on Windows and handled with fallback:
```batch
# Check OpenSSL legacy provider support:
openssl list -providers

# This will result in SHA256 fallback for Bitcoin addresses
# Cross-validate addresses with other tools
```

### Performance Issues

#### Slow Compilation
```batch
# Use Release build for better performance
cmake --build . --config Release

# Use parallel compilation
cmake --build . --config Release --parallel 4
```

#### Runtime Performance
```batch
# Release builds are significantly faster than Debug
# Use 64-bit build for better performance
# SSD storage improves build and execution times
```

## Security Considerations for Windows

### Platform-Specific Security Notes

- **Reduced Cryptographic Validation**: Minimal secp256k1 implementation
- **RIPEMD160 Fallback**: May produce different Bitcoin addresses
- **Limited Testing**: No automated security test suite
- **Development Use Only**: Not recommended for production wallets

### Security Best Practices

```batch
# Disable swap files when generating production wallets
# (This requires administrative privileges)
# Use Control Panel > System > Advanced > Performance > Settings > Advanced > Virtual Memory

# Clear command history
cls

# Use Windows Defender or trusted antivirus
# Keep Windows and all dependencies updated
```

### Cross-Validation Requirements

Always validate Windows-generated addresses:

```batch
# Generate address with Windows tool
wallet_generator.exe -n bitcoin "your test mnemonic"

# Compare with:
# - Electrum wallet (same mnemonic)
# - Bitcoin Core (if available)
# - Online tools (for testing only)
# - Linux/macOS version of this tool
```

## Distribution and Deployment

### Creating Distributable Builds

#### Static Linking (Recommended)
```batch
# Build with static libraries to reduce dependencies
cmake .. -DCMAKE_BUILD_TYPE=Release -DVCPKG_TARGET_TRIPLET=x64-windows-static
cmake --build . --config Release
```

#### Including Required DLLs
If using dynamic linking, include these DLLs with your executable:
- `libssl-1_1-x64.dll` or `libssl-3-x64.dll`
- `libcrypto-1_1-x64.dll` or `libcrypto-3-x64.dll`
- Visual C++ Redistributable (may be required)

#### Creating Installer
```batch
# Using CPack (if configured)
cpack -G NSIS

# Manual installer creation using NSIS, Inno Setup, or similar tools
```

### Deployment Checklist

- [ ] **Test on clean Windows system** without development tools
- [ ] **Include all required DLLs** or use static linking
- [ ] **Test with different Windows versions** (10, 11)
- [ ] **Verify antivirus compatibility** 
- [ ] **Document installation requirements**
- [ ] **Provide uninstall procedures**

## Development Environment

### Recommended IDEs
- **Visual Studio 2019/2022** (best Windows integration, IntelliSense, debugging)
- **Visual Studio Code** with C++ extensions (lightweight, cross-platform)
- **CLion** with CMake support (powerful, requires license)
- **Qt Creator** with CMake support (free, good CMake integration)

### Debugging

#### Visual Studio Debugging
```batch
# Build debug version
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug

# Open in Visual Studio
devenv build\Debug\wallet_generator.exe

# Set breakpoints and debug normally
```

#### Command Line Debugging
```batch
# Enable debug output
set CMAKE_VERBOSE_MAKEFILE=ON

# Run with debugging
gdb build\Debug\wallet_generator.exe  # If GDB available
# Or use Visual Studio debugger
```

### Development Workflow

1. **Setup Development Environment**
   ```batch
   # Install Visual Studio with C++ workload
   # Install CMake and Git
   # Setup vcpkg for dependencies
   ```

2. **Configure Project**
   ```batch
   git clone https://github.com/your-repo/bip39-wallet-generator.git
   cd bip39-wallet-generator
   mkdir build && cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
   ```

3. **Build and Test**
   ```batch
   cmake --build . --config Debug
   .\Debug\wallet_generator.exe --help
   ```

4. **Cross-Platform Testing**
   ```batch
   # Test same inputs on Linux/macOS and Windows
   # Compare outputs for consistency
   # Document any differences
   ```

## Known Limitations

### Current Windows Limitations

1. **No Automated Test Suite**: Manual testing required
2. **Minimal secp256k1**: Reduced cryptographic validation
3. **RIPEMD160 Fallback**: Different Bitcoin address generation
4. **Limited Static Analysis**: Fewer code quality tools available
5. **Performance**: May be slower than Linux/macOS versions

### Planned Improvements

- [ ] **Native Windows Test Suite**: Automated testing without WSL
- [ ] **Enhanced Security**: Better cryptographic library integration
- [ ] **GUI Interface**: Windows-native graphical interface
- [ ] **Installer Package**: MSI or NSIS installer
- [ ] **Code Signing**: Digital signatures for distribution

## Alternative: WSL (Windows Subsystem for Linux)

For enhanced functionality and security, consider using WSL:

### WSL Setup
```batch
# Install WSL2 (Windows 10 version 2004+ or Windows 11)
wsl --install

# Install Ubuntu
wsl --install -d Ubuntu

# Access Linux environment
wsl

# Follow Linux installation instructions within WSL
sudo apt-get update
sudo apt-get install libssl-dev libsecp256k1-dev build-essential
```

### Benefits of WSL
- **Full cryptographic library support**
- **Comprehensive test suite** (100+ tests)
- **Better security validation**
- **Compatible with Linux documentation**

---

## Need Help?

If you encounter issues:

1. **Check this troubleshooting guide**
2. **Verify all prerequisites are installed**
3. **Try the automated installation script**
4. **Consider using WSL for Linux compatibility**
5. **Open an issue on GitHub** with:
   - Windows version (Windows 10/11)
   - Visual Studio version
   - CMake version
   - OpenSSL version
   - Complete error messages
   - Build logs

### Getting Support

- **Windows-Specific Issues**: Use this guide
- **General Usage**: See main [README.md](README.md)
- **Security Concerns**: See [SECURITY.md](SECURITY.md)
- **Contributing**: See [CONTRIBUTING.md](CONTRIBUTING.md)
- **Bug Reports**: GitHub Issues with Windows label

Remember: For production use with real funds, prefer Linux/macOS systems with full cryptographic library support and comprehensive testing.