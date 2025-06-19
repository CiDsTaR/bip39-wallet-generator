#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/ripemd.h>
#include <openssl/opensslv.h>
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
#include <openssl/provider.h>
#endif
#include <secp256k1.h>

struct NetworkConfig {
    std::string name;
    std::string derivation_path;
    uint8_t address_version;
    uint8_t wif_version;
    std::string hrp; // For Bech32 (Bitcoin segwit, etc)
    bool use_keccak; // For Ethereum
};

class WalletGenerator {
private:
    secp256k1_context* ctx;
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
    OSSL_PROVIDER* legacy_provider;
    OSSL_PROVIDER* default_provider;
#endif

    std::string bytesToHex(const std::vector<uint8_t>& bytes);
    // Removed hexToBytes as it's unused - fixes unusedPrivateFunction warning
    std::vector<uint8_t> pbkdf2(const std::string& password, const std::string& salt, int iterations, int dkLen);
    std::vector<uint8_t> hmacSha512(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data);
    std::vector<uint8_t> sha256(const std::vector<uint8_t>& data);
    std::vector<uint8_t> ripemd160(const std::vector<uint8_t>& data);
    std::vector<uint8_t> keccak256(const std::vector<uint8_t>& data);
    std::string base58Encode(const std::vector<uint8_t>& data);
    std::vector<uint8_t> deriveKey(const std::vector<uint8_t>& seed, const std::string& path);

public:
    // Make networks accessible
    static const std::map<std::string, NetworkConfig> networks;

    WalletGenerator();
    ~WalletGenerator();

    std::vector<uint8_t> mnemonicToSeed(const std::string& mnemonic, const std::string& passphrase = "");

    struct WalletInfo {
        std::string privateKey;
        std::string publicKey;
        std::string address;
        std::string wif;
        std::string network;
        std::string derivationPath;
    };

    WalletInfo generateWallet(const std::vector<uint8_t>& seed, const std::string& networkName, const std::string& customPath = "");
    void printWallet(const WalletInfo& wallet, bool verbose = true);
};

void printUsage(const char* programName);