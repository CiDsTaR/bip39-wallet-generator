# Windows Setup Guide

This guide explains how to build and run the BIP39 Wallet Generator on Windows.

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
- **Visual Studio 2019 or 2022** (Community edition is fine)
  - Must include "Desktop development with C++" workload
- **CMake 3.15+**
- **Git**

### Dependencies
- **OpenSSL** (for cryptographic functions)
- **secp256k1** (optional, will use minimal implementation if not available)

## Installation Methods

### Method 1: vcpkg (Recommended)

vcpkg is Microsoft's C++ package manager that simplifies dependency management.

```batch
# Install vcpkg
git clone https://github.com/Microsoft/vcpkg.git C:\vcpkg
cd C:\vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg.exe integrate install

# Install dependencies
.\vcpkg.exe install openssl:x64-windows
.\vcpkg.exe install secp256k1:x64-windows  # Optional

# Build project
cd your\project\directory
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
```

### Method 2: Manual Installation

1. **Download OpenSSL** from https://slproweb.com/products/Win32OpenSSL.html
2. **Install to default location** (usually `C:\Program Files\OpenSSL-Win64\`)
3. **Set environment variables**:
   ```batch
   set OPENSSL_ROOT_DIR=C:\Program Files\OpenSSL-Win64
   set OPENSSL_INCLUDE_DIR=C:\Program Files\OpenSSL-Win64\include
   set OPENSSL_CRYPTO_LIBRARY=C:\Program Files\OpenSSL-Win64\lib\libcrypto.lib
   set OPENSSL_SSL_LIBRARY=C:\Program Files\OpenSSL-Win64\lib\libssl.lib
   ```
4. **Build project**:
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
choco install git cmake visualstudio2022buildtools -y
```

## Building

### Using the Build Script

```batch
scripts\build_windows.bat
```

### Manual Build

```batch
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

### With vcpkg

```batch
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build . --config Release
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

## Troubleshooting

### Common Issues

#### 1. "CMake not found"
- Install CMake from https://cmake.org/
- Add CMake to your PATH environment variable

#### 2. "Visual Studio compiler not found"
- Install Visual Studio 2019 or 2022 with C++ support
- Or use Visual Studio Build Tools
- Make sure to run from "Developer Command Prompt"

#### 3. "OpenSSL not found"
- Use vcpkg: `vcpkg install openssl:x64-windows`
- Or download from https://slproweb.com/products/Win32OpenSSL.html
- Set OPENSSL_ROOT_DIR environment variable

#### 4. "secp256k1 not found"
- The project will use a minimal implementation automatically
- Or install via vcpkg: `vcpkg install secp256k1:x64-windows`

#### 5. "Access denied" or permission errors
- Run Command Prompt or PowerShell as Administrator
- Check Windows Defender / antivirus settings

#### 6. "getopt.h not found"
- This is handled automatically by the Windows-compatible version
- Make sure you're using `wallet_generator_win.cpp` for Windows builds

### Build Errors

#### Error: "Could not find OpenSSL"
```batch
# Solution 1: Use vcpkg
vcpkg install openssl:x64-windows

# Solution 2: Set environment variables
set OPENSSL_ROOT_DIR=C:\Program Files\OpenSSL-Win64
```

#### Error: "LINK : fatal error LNK1181: cannot open input file 'libssl.lib'"
```batch
# Ensure OpenSSL is properly installed and linked
# Use vcpkg for easier management
```

#### Error: "MSB8020: The build tools for v142 cannot be found"
```batch
# Install Visual Studio 2019/2022 or Build Tools
choco install visualstudio2022buildtools -y
choco install visualstudio2022-workload-vctools -y
```

## Performance Notes

- **Release builds** are significantly faster than Debug builds
- **64-bit builds** are recommended for better performance
- **SSD storage** improves build and execution times

## Security Considerations

- **Run on secure, offline systems** for production wallet generation
- **Use Windows Defender** or trusted antivirus software
- **Disable swap files** when generating production wallets
- **Clear clipboard** after copying sensitive information

## Distribution

To distribute the compiled executable:

1. **Static linking** (if possible):
   ```batch
   cmake .. -DCMAKE_BUILD_TYPE=Release -DVCPKG_TARGET_TRIPLET=x64-windows-static
   ```

2. **Include required DLLs**:
   - OpenSSL DLLs (libssl-1_1-x64.dll, libcrypto-1_1-x64.dll)
   - Visual C++ Redistributable

3. **Create installer** using CPack:
   ```batch
   cpack -G NSIS
   ```

## Development Environment

### Recommended IDEs
- **Visual Studio 2019/2022** (best Windows integration)
- **Visual Studio Code** with C++ extensions
- **CLion** with CMake support

### Debugging
```batch
# Build debug version
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug

# Run with debugger
devenv build\Debug\wallet_generator.exe
```

## Testing on Windows

Currently, the test suite is designed for Unix-like systems. Windows testing requires:

1. **WSL (Windows Subsystem for Linux)** for full test suite
2. **Manual testing** using the executable
3. **Future enhancement**: Native Windows test runner

### Basic Testing

```batch
# Test basic functionality
wallet_generator.exe "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Test all networks
wallet_generator.exe -a "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"

# Test error handling
wallet_generator.exe
wallet_generator.exe -n invalid_network "test"
```

---

## Need Help?

If you encounter issues:

1. Check this troubleshooting guide
2. Verify all prerequisites are installed
3. Try the automated installation script
4. Open an issue on GitHub with:
   - Windows version
   - Visual Studio version
   - Error messages
   - Build logs