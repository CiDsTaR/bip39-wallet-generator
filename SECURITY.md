# Security Policy

## Supported Versions

| Version | Supported          | Security Status |
| ------- | ------------------ | --------------- |
| 1.1.x   | :white_check_mark: | Active support  |
| 1.0.x   | :white_check_mark: | Security fixes only |
| < 1.0   | :x:                | Unsupported |

## 🚨 Critical Security Warnings

### RIPEMD160 Fallback Implementation
⚠️ **CRITICAL**: This implementation falls back to double SHA256 when RIPEMD160 is unavailable. This affects Bitcoin, Litecoin, and Dogecoin address generation and may produce addresses that differ from standard implementations.

**Verification Required**: Before using for Bitcoin-style networks, verify RIPEMD160 availability:
```bash
openssl list -digest-commands | grep rmd
```

### Platform Security Considerations
- **Linux/macOS**: Full cryptographic library support with comprehensive testing
- **Windows**: Uses minimal secp256k1 implementation with reduced security guarantees
- **Production Use**: Strongly recommended to use Linux/macOS for production wallet generation

## Reporting a Vulnerability

The security of the BIP39 Wallet Generator is of paramount importance. If you discover a security vulnerability, please follow these guidelines:

### How to Report

**DO NOT** create a public GitHub issue for security vulnerabilities.

Instead, please report security vulnerabilities by email to: **[dev@cidstar.es]**

### What to Include

Please include the following information in your report:

1. **Description** - A clear description of the vulnerability
2. **Impact** - The potential impact and attack scenarios
3. **Steps to Reproduce** - Detailed steps to reproduce the issue
4. **Environment** - Operating system, compiler version, OpenSSL version, etc.
5. **Proof of Concept** - If available, include a minimal proof of concept
6. **Platform Affected** - Specify if Windows, Linux, macOS, or all platforms
7. **Networks Affected** - Which cryptocurrency networks are impacted

### Severity Classification

| Severity | Description | Examples |
|----------|-------------|----------|
| **Critical** | Remote code execution, private key exposure, cryptographic breaks | Private key generation flaws, memory leaks exposing keys |
| **High** | Privilege escalation, cryptographic weaknesses, address generation errors | RIPEMD160 fallback issues, HD derivation flaws |
| **Medium** | Information disclosure, DoS attacks, input validation bypass | Memory disclosure, crash on invalid input |
| **Low** | Minor security improvements, hardening opportunities | Compiler warnings, code quality issues |

### Example Report Format

```
Subject: [SECURITY] Vulnerability in BIP39 Wallet Generator

Description:
[Clear description of the vulnerability]

Impact:
[Potential impact and risk assessment]

Steps to Reproduce:
1. [Step 1]
2. [Step 2]
3. [Step 3]

Environment:
- OS: Ubuntu 22.04 / Windows 11 / macOS 13.0
- Compiler: GCC 11.3 / MSVC 2022 / Clang 14.0
- OpenSSL Version: 3.0.2
- secp256k1 Version: 0.3.0 / Minimal implementation
- Project Version: 1.1.0

Platform Affected:
[ ] Linux  [ ] macOS  [ ] Windows  [ ] All

Networks Affected:
[ ] Bitcoin  [ ] Ethereum  [ ] All Networks  [ ] Specific: ___

Proof of Concept:
[Minimal code or steps demonstrating the issue]
```



## Current Security Features

### Cryptographic Implementation

| Component | Implementation | Security Level | Notes |
|-----------|---------------|----------------|-------|
| **Mnemonic to Seed** | PBKDF2-HMAC-SHA512 (BIP39) | ✅ High | Standard compliant |
| **HD Key Derivation** | Simplified HMAC-SHA512 | ⚠️ Medium | Not full BIP32 |
| **Private Key Generation** | secp256k1 / Minimal | ✅ High / ⚠️ Medium | Platform dependent |
| **Hash Functions** | SHA-256, RIPEMD-160/Fallback | ✅ High / ⚠️ Medium | RIPEMD160 availability dependent |
| **Address Generation** | Network-specific | ✅ High | Varies by network |

### Platform Security Matrix

| Feature | Linux/macOS | Windows | Security Impact |
|---------|-------------|---------|-----------------|
| **secp256k1** | Full library | Minimal implementation | High |
| **RIPEMD160** | Native/Fallback | SHA256 fallback | Medium |
| **Memory Management** | Standard | Standard | Medium |
| **Test Coverage** | Comprehensive (100+ tests) | Manual testing | High |
| **Static Analysis** | Available | Limited | Medium |

### Current Protections

- **Secure Random Generation** - Uses system entropy sources
- **Memory Management** - Clears sensitive data when possible
- **Established Cryptography** - Uses OpenSSL and secp256k1 (where available)
- **No Network Communication** - Completely offline operation
- **Input Validation** - Validates user inputs and parameters
- **Cross-Platform Testing** - Comprehensive test suite on Linux/macOS

### Known Security Limitations

#### Critical Limitations

1. **RIPEMD160 Fallback** (Bitcoin, Litecoin, Dogecoin)
   - **Issue**: Falls back to double SHA256 when RIPEMD160 unavailable
   - **Impact**: May generate different addresses than standard implementations
   - **Mitigation**: Verify RIPEMD160 availability, cross-validate addresses

2. **Simplified HD Derivation** (All networks)
   - **Issue**: Not full BIP32 implementation
   - **Impact**: May not match hardware wallet derivation exactly
   - **Mitigation**: Cross-validate with established tools

3. **Windows Minimal secp256k1** (Windows only)
   - **Issue**: Uses simplified secp256k1 implementation
   - **Impact**: Reduced cryptographic validation
   - **Mitigation**: Use Linux/macOS for production

#### Medium Risk Limitations

4. **Memory Security** - Standard memory allocation may leave traces
5. **Side-Channel Attacks** - Not hardened against timing or power analysis
6. **Platform Dependencies** - Security varies by operating system implementation

### Mitigations

- **Platform Selection**: Use Linux/macOS for production wallets
- **Cross-Validation**: Verify generated addresses using multiple tools
- **Environment Hardening**: Disable swap files, use secure systems
- **Testing Protocol**: Run comprehensive security test suite
- **Address Verification**: Always validate addresses before use

## Security Testing and Validation

### Automated Security Testing

```bash
# Run comprehensive security test suite (Linux/macOS)
make test-security

# Static analysis
make static-analysis

# Memory leak detection
make memtest

# Code coverage analysis
make coverage
```

### Manual Security Validation

```bash
# Verify RIPEMD160 availability
openssl list -digest-commands | grep rmd

# Cross-validate Bitcoin addresses
./wallet_generator -n bitcoin "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
# Compare with: Electrum, Bitcoin Core, hardware wallets

# Test entropy and randomness
./wallet_generator -n bitcoin "test mnemonic 1"
./wallet_generator -n bitcoin "test mnemonic 2"
# Verify different mnemonics produce different addresses
```

### Security Test Coverage

- **Private Key Randomness**: Validates uniqueness and entropy
- **Seed Determinism**: Ensures consistent seed generation
- **Passphrase Isolation**: Verifies different passphrases create different keys
- **Network Isolation**: Confirms network-specific address generation
- **Derivation Path Isolation**: Tests path-specific key derivation
- **Memory Security**: Basic memory management validation
- **Input Validation**: Tests error handling and boundary conditions

## Cryptographic Standards

### Algorithms Used

| Component | Algorithm | Standard | Implementation | Security Level |
|-----------|-----------|----------|----------------|----------------|
| Mnemonic to Seed | PBKDF2-HMAC-SHA512 | BIP39 | OpenSSL | ✅ High |
| HD Key Derivation | HMAC-SHA512 (Simplified) | BIP32/BIP44 | OpenSSL | ⚠️ Medium |
| Private Key Generation | secp256k1 | SEC 2 | libsecp256k1/Minimal | ✅ High / ⚠️ Medium |
| Hash Functions | SHA-256 | NIST | OpenSSL | ✅ High |
| Hash Functions | RIPEMD-160 | ISO | OpenSSL/Fallback | ✅ High / ⚠️ Medium |
| Address Generation | Various | Network-specific | Custom | ✅ High |

### Key Security Properties

- **Deterministic** - Same mnemonic always produces same keys
- **Non-reversible** - Cannot derive mnemonic from generated keys
- **Cryptographically Secure** - Uses established cryptographic primitives
- **Standard Compliant** - Follows BIP39/BIP44 specifications (with limitations)

## Audit Information

### Security Reviews

- **Internal Review**: Ongoing code review by maintainers
- **Community Review**: Open source code available for public audit
- **Automated Analysis**: Continuous integration with security scanning
- **Platform Testing**: Multi-platform compatibility and security testing

### Completed Security Assessments

- ✅ **Static Code Analysis**: cppcheck, compiler warnings
- ✅ **Memory Analysis**: Valgrind leak detection
- ✅ **Cryptographic Testing**: Security test suite validation
- ✅ **Cross-Platform Testing**: Linux, macOS, Windows compatibility
- ✅ **Input Validation**: Boundary condition and error handling tests

### Planned Security Enhancements

- [ ] **Professional Security Audit** by third-party firm
- [ ] **Formal Cryptographic Verification** of key algorithms
- [ ] **Penetration Testing** against common attack vectors
- [ ] **Fuzzing Testing** for input validation robustness
- [ ] **Hardware Security Module (HSM)** integration
- [ ] **Secure Memory Allocation** for sensitive data
- [ ] **Enhanced Input Sanitization** and validation
- [ ] **Full BIP32 Implementation** to replace simplified derivation
- [ ] **Native Windows Security Testing** automation




## Platform-Specific Security Considerations

### Linux/macOS Security

- **Full Cryptographic Support** - Complete libsecp256k1 and OpenSSL
- **Comprehensive Testing** - 100+ automated security tests
- **Memory Protection** - Standard system memory protection
- **Static Analysis** - Automated code analysis in CI/CD

**Recommendations**:
- Use for production wallet generation
- Enable all available security features
- Run on hardened, offline systems
- Regularly update dependencies

### Windows Security

- **Limited Cryptographic Libraries** - Minimal secp256k1 implementation
- **Reduced Testing Coverage** - Manual testing only
- **Compatibility Layers** - Additional attack surface

**Recommendations**:
- Use only for development and testing
- Cross-validate all outputs with Linux/macOS
- Consider WSL for enhanced security
- Plan migration to Linux/macOS for production



## Legal and Compliance

### Disclaimer

This software is provided "as is" without warranty of any kind. Users assume all risks associated with:

- **Private Key Generation** and cryptocurrency transactions
- **Address Validation** and fund management
- **Platform-Specific Limitations** and security considerations
- **Cryptographic Implementation** variations and fallbacks

### Scope

This security policy covers:

- **BIP39 Wallet Generator source code** in this repository
- **Official distribution channels** and releases
- **Documented features and functionality** as described
- **Supported platforms** (Linux, macOS, Windows)

This policy does not cover:

- **Third-party forks** or modifications
- **User environments** or configurations
- **External dependencies** (covered by their own policies)
- **Derived works** or integrations

### Compliance Considerations

Users of this software should be aware of:

- **Local Cryptocurrency Regulations** - Compliance with applicable laws
- **Data Protection Requirements** - GDPR, CCPA, and similar regulations
- **Financial Service Regulations** - Banking and financial compliance
- **Export Control Laws** - Cryptographic software restrictions
- **Professional Standards** - Industry best practices and standards

---

## Security Best Practices for Users

### Environment Security

```bash
# Disable swap files when generating production wallets
sudo swapoff -a

# Clear command history
history -c && history -w

# Use secure, offline systems
# Disconnect network interfaces during key generation

# Verify system integrity
# Use checksums and signatures for all software
```

### Operational Security

1. **Air-Gapped Systems** - Use completely offline computers
2. **Live Operating Systems** - Boot from read-only media (Tails, etc.)
3. **Hardware Verification** - Use trusted, verified hardware
4. **Physical Security** - Secure physical access to systems
5. **Documentation Security** - Secure storage of procedures and keys

### Validation Procedures

```bash
# Always cross-validate addresses
./wallet_generator -n bitcoin "your mnemonic"
# Compare with: Electrum, Bitcoin Core, hardware wallets

# Verify RIPEMD160 support
openssl list -digest-commands | grep rmd

# Test with known vectors
./wallet_generator "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about"
# Compare output with published BIP39 test vectors
```

**Remember**: The security of your cryptocurrency depends on protecting your mnemonic phrase. Never share it with anyone, always verify generated addresses independently, and use appropriate security measures for your threat model.