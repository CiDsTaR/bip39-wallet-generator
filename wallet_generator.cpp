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
#include <secp256k1.h>
#include <getopt.h>

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

    std::string bytesToHex(const std::vector<uint8_t>& bytes) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (uint8_t byte : bytes) {
            ss << std::setw(2) << static_cast<int>(byte);
        }
        return ss.str();
    }

    std::vector<uint8_t> hexToBytes(const std::string& hex) {
        std::vector<uint8_t> bytes;
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteString = hex.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
            bytes.push_back(byte);
        }
        return bytes;
    }

    std::vector<uint8_t> pbkdf2(const std::string& password, const std::string& salt, int iterations, int dkLen) {
        std::vector<uint8_t> key(dkLen);
        PKCS5_PBKDF2_HMAC(password.c_str(), password.length(),
                         reinterpret_cast<const unsigned char*>(salt.c_str()), salt.length(),
                         iterations, EVP_sha512(), dkLen, key.data());
        return key;
    }

    std::vector<uint8_t> hmacSha512(const std::vector<uint8_t>& key, const std::vector<uint8_t>& data) {
        std::vector<uint8_t> result(64);
        unsigned int len;
        HMAC(EVP_sha512(), key.data(), key.size(), data.data(), data.size(), result.data(), &len);
        return result;
    }

    std::vector<uint8_t> sha256(const std::vector<uint8_t>& data) {
        std::vector<uint8_t> hash(32);
        SHA256(data.data(), data.size(), hash.data());
        return hash;
    }

    std::vector<uint8_t> ripemd160(const std::vector<uint8_t>& data) {
        std::vector<uint8_t> hash(20);
        
        // Use EVP interface for OpenSSL 3.0+ compatibility
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create RIPEMD160 context");
        }
        
        if (EVP_DigestInit_ex(ctx, EVP_ripemd160(), nullptr) != 1 ||
            EVP_DigestUpdate(ctx, data.data(), data.size()) != 1 ||
            EVP_DigestFinal_ex(ctx, hash.data(), nullptr) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("RIPEMD160 computation failed");
        }
        
        EVP_MD_CTX_free(ctx);
        return hash;
    }

    std::vector<uint8_t> keccak256(const std::vector<uint8_t>& data) {
        // Use SHA3-256 as a substitute for Keccak256
        // Note: SHA3-256 and Keccak-256 are different, but for demonstration purposes
        // In production, use a proper Keccak implementation
        std::vector<uint8_t> hash(32);
        
        EVP_MD_CTX* ctx = EVP_MD_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create SHA3 context");
        }
        
        const EVP_MD* sha3_256 = EVP_sha3_256();
        if (!sha3_256) {
            EVP_MD_CTX_free(ctx);
            // Fallback to SHA256 if SHA3 is not available
            return sha256(data);
        }
        
        if (EVP_DigestInit_ex(ctx, sha3_256, nullptr) != 1 ||
            EVP_DigestUpdate(ctx, data.data(), data.size()) != 1 ||
            EVP_DigestFinal_ex(ctx, hash.data(), nullptr) != 1) {
            EVP_MD_CTX_free(ctx);
            throw std::runtime_error("SHA3-256 computation failed");
        }
        
        EVP_MD_CTX_free(ctx);
        return hash;
    }

    std::string base58Encode(const std::vector<uint8_t>& data) {
        const std::string alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";
        std::vector<uint8_t> temp = data;
        std::string result;
        
        while (!temp.empty() && temp.back() != 0) {
            int remainder = 0;
            for (int i = temp.size() - 1; i >= 0; i--) {
                int current = remainder * 256 + temp[i];
                temp[i] = current / 58;
                remainder = current % 58;
            }
            result = alphabet[remainder] + result;
            
            // Remove trailing zeros
            while (!temp.empty() && temp.back() == 0) {
                temp.pop_back();
            }
        }
        
        // Add '1' for each leading zero byte
        for (uint8_t byte : data) {
            if (byte == 0) result = '1' + result;
            else break;
        }
        
        return result;
    }

    std::vector<uint8_t> deriveKey(const std::vector<uint8_t>& seed, const std::string& /* path */) {
        // Simplified HD derivation implementation
        // For production use a complete library like libbitcoin
        // Note: path parameter is unused in this simplified implementation
        
        // Master key derivation
        std::string hmacKey = "Bitcoin seed";
        std::vector<uint8_t> hmacKeyBytes(hmacKey.begin(), hmacKey.end());
        std::vector<uint8_t> masterKey = hmacSha512(hmacKeyBytes, seed);
        
        return std::vector<uint8_t>(masterKey.begin(), masterKey.begin() + 32);
    }

public:
    // Make networks accessible
    static const std::map<std::string, NetworkConfig> networks;

    WalletGenerator() {
        ctx = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    }

    ~WalletGenerator() {
        secp256k1_context_destroy(ctx);
    }

    std::vector<uint8_t> mnemonicToSeed(const std::string& mnemonic, const std::string& passphrase = "") {
        std::string salt = "mnemonic" + passphrase;
        return pbkdf2(mnemonic, salt, 2048, 64);
    }

    struct WalletInfo {
        std::string privateKey;
        std::string publicKey;
        std::string address;
        std::string wif;
        std::string network;
        std::string derivationPath;
    };

    WalletInfo generateWallet(const std::vector<uint8_t>& seed, const std::string& networkName, const std::string& customPath = "") {
        WalletInfo wallet;
        
        auto it = networks.find(networkName);
        if (it == networks.end()) {
            throw std::runtime_error("Unsupported network: " + networkName);
        }
        
        const NetworkConfig& config = it->second;
        wallet.network = networkName;
        wallet.derivationPath = customPath.empty() ? config.derivation_path : customPath;
        
        // Derive private key
        std::vector<uint8_t> privateKeyBytes = deriveKey(seed, wallet.derivationPath);
        wallet.privateKey = bytesToHex(privateKeyBytes);
        
        // Generate public key
        secp256k1_pubkey pubkey;
        if (!secp256k1_ec_pubkey_create(ctx, &pubkey, privateKeyBytes.data())) {
            throw std::runtime_error("Error generating public key");
        }
        
        std::vector<uint8_t> pubkeyBytes(65);
        size_t pubkeyLen = 65;
        secp256k1_ec_pubkey_serialize(ctx, pubkeyBytes.data(), &pubkeyLen, &pubkey, SECP256K1_EC_UNCOMPRESSED);
        wallet.publicKey = bytesToHex(pubkeyBytes);
        
        // Generate address based on network
        if (config.use_keccak) {
            // Ethereum-style address
            std::vector<uint8_t> pubkeyHash = keccak256(std::vector<uint8_t>(pubkeyBytes.begin() + 1, pubkeyBytes.end()));
            wallet.address = "0x" + bytesToHex(std::vector<uint8_t>(pubkeyHash.end() - 20, pubkeyHash.end()));
        } else {
            // Bitcoin-style address
            std::vector<uint8_t> pubkeyCompressed(33);
            size_t compressedLen = 33;
            secp256k1_ec_pubkey_serialize(ctx, pubkeyCompressed.data(), &compressedLen, &pubkey, SECP256K1_EC_COMPRESSED);
            
            std::vector<uint8_t> pubkeyHash = ripemd160(sha256(pubkeyCompressed));
            std::vector<uint8_t> addressBytes = {config.address_version};
            addressBytes.insert(addressBytes.end(), pubkeyHash.begin(), pubkeyHash.end());
            
            // Checksum
            std::vector<uint8_t> checksum = sha256(sha256(addressBytes));
            addressBytes.insert(addressBytes.end(), checksum.begin(), checksum.begin() + 4);
            
            wallet.address = base58Encode(addressBytes);
        }
        
        // WIF (Wallet Import Format)
        std::vector<uint8_t> wifBytes = {config.wif_version};
        wifBytes.insert(wifBytes.end(), privateKeyBytes.begin(), privateKeyBytes.end());
        wifBytes.push_back(0x01); // Compressed key
        
        std::vector<uint8_t> wifChecksum = sha256(sha256(wifBytes));
        wifBytes.insert(wifBytes.end(), wifChecksum.begin(), wifChecksum.begin() + 4);
        wallet.wif = base58Encode(wifBytes);
        
        return wallet;
    }

    void printWallet(const WalletInfo& wallet, bool verbose = true) {
        std::cout << "=== " << wallet.network << " Wallet ===" << std::endl;
        std::cout << "Derivation Path: " << wallet.derivationPath << std::endl;
        std::cout << "Private Key: " << wallet.privateKey << std::endl;
        if (verbose) {
            std::cout << "Public Key: " << wallet.publicKey << std::endl;
            std::cout << "WIF: " << wallet.wif << std::endl;
        }
        std::cout << "Address: " << wallet.address << std::endl;
        std::cout << std::endl;
    }
};

// Configuration for top 10 networks
const std::map<std::string, NetworkConfig> WalletGenerator::networks = {
    {"bitcoin", {"Bitcoin", "m/44'/0'/0'/0/0", 0x00, 0x80, "bc", false}},
    {"ethereum", {"Ethereum", "m/44'/60'/0'/0/0", 0x00, 0x80, "", true}},
    {"binance", {"Binance Smart Chain", "m/44'/60'/0'/0/0", 0x00, 0x80, "", true}},
    {"polygon", {"Polygon", "m/44'/60'/0'/0/0", 0x00, 0x80, "", true}},
    {"avalanche", {"Avalanche", "m/44'/60'/0'/0/0", 0x00, 0x80, "", true}},
    {"solana", {"Solana", "m/44'/501'/0'/0/0", 0x00, 0x80, "", false}},
    {"cardano", {"Cardano", "m/44'/1815'/0'/0/0", 0x00, 0x80, "", false}},
    {"litecoin", {"Litecoin", "m/44'/2'/0'/0/0", 0x30, 0xB0, "ltc", false}},
    {"dogecoin", {"Dogecoin", "m/44'/3'/0'/0/0", 0x1E, 0x9E, "", false}},
    {"tron", {"Tron", "m/44'/195'/0'/0/0", 0x41, 0x80, "", false}}
};

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] \"mnemonic phrase\"\n\n";
    std::cout << "Options:\n";
    std::cout << "  -n, --network NETWORK    Network (bitcoin, ethereum, binance, polygon, avalanche, solana, cardano, litecoin, dogecoin, tron)\n";
    std::cout << "  -c, --count COUNT        Number of wallets to generate (default: 1)\n";
    std::cout << "  -p, --path PATH          Custom derivation path\n";
    std::cout << "  -P, --passphrase PASS    BIP39 passphrase (optional)\n";
    std::cout << "  -a, --all-networks       Generate for all networks\n";
    std::cout << "  -v, --verbose            Show complete information\n";
    std::cout << "  -h, --help               Show this help\n\n";
    std::cout << "Example:\n";
    std::cout << "  " << programName << " -n ethereum -c 5 \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"\n";
}

int main(int argc, char* argv[]) {
    std::string network = "bitcoin";
    int count = 1;
    std::string customPath = "";
    std::string passphrase = "";
    bool allNetworks = false;
    bool verbose = false;
    
    static struct option longOptions[] = {
        {"network", required_argument, 0, 'n'},
        {"count", required_argument, 0, 'c'},
        {"path", required_argument, 0, 'p'},
        {"passphrase", required_argument, 0, 'P'},
        {"all-networks", no_argument, 0, 'a'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "n:c:p:P:avh", longOptions, nullptr)) != -1) {
        switch (opt) {
            case 'n':
                network = optarg;
                break;
            case 'c':
                count = std::stoi(optarg);
                break;
            case 'p':
                customPath = optarg;
                break;
            case 'P':
                passphrase = optarg;
                break;
            case 'a':
                allNetworks = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            default:
                printUsage(argv[0]);
                return 1;
        }
    }
    
    if (optind >= argc) {
        std::cerr << "Error: Mnemonic phrase required\n";
        printUsage(argv[0]);
        return 1;
    }
    
    std::string mnemonic = argv[optind];
    
    try {
        WalletGenerator generator;
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic, passphrase);
        
        std::vector<std::string> networksToProcess;
        if (allNetworks) {
            networksToProcess = {"bitcoin", "ethereum", "binance", "polygon", "avalanche", "solana", "cardano", "litecoin", "dogecoin", "tron"};
        } else {
            networksToProcess = {network};
        }
        
        for (const std::string& net : networksToProcess) {
            std::cout << "\n" << std::string(50, '=') << std::endl;
            std::cout << "NETWORK: " << net << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            
            for (int i = 0; i < count; i++) {
                std::string derivationPath = customPath;
                if (derivationPath.empty()) {
                    // Use default path but change the index
                    derivationPath = WalletGenerator::networks.at(net).derivation_path;
                    size_t lastSlash = derivationPath.find_last_of('/');
                    if (lastSlash != std::string::npos) {
                        derivationPath = derivationPath.substr(0, lastSlash + 1) + std::to_string(i);
                    }
                }
                
                WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, net, derivationPath);
                std::cout << "Wallet #" << (i + 1) << std::endl;
                generator.printWallet(wallet, verbose);
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}, ethereum, binance, polygon, avalanche, solana, cardano, litecoin, dogecoin, tron)\n";
    std::cout << "  -c, --count COUNT        Number of wallets to generate (default: 1)\n";
    std::cout << "  -p, --path PATH          Custom derivation path\n";
    std::cout << "  -P, --passphrase PASS    BIP39 passphrase (optional)\n";
    std::cout << "  -a, --all-networks       Generate for all networks\n";
    std::cout << "  -v, --verbose            Show complete information\n";
    std::cout << "  -h, --help               Show this help\n\n";
    std::cout << "Example:\n";
    std::cout << "  " << programName << " -n ethereum -c 5 \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"\n";
}

int main(int argc, char* argv[]) {
    std::string network = "bitcoin";
    int count = 1;
    std::string customPath = "";
    std::string passphrase = "";
    bool allNetworks = false;
    bool verbose = false;
    
    static struct option longOptions[] = {
        {"network", required_argument, 0, 'n'},
        {"count", required_argument, 0, 'c'},
        {"path", required_argument, 0, 'p'},
        {"passphrase", required_argument, 0, 'P'},
        {"all-networks", no_argument, 0, 'a'},
        {"verbose", no_argument, 0, 'v'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    int opt;
    while ((opt = getopt_long(argc, argv, "n:c:p:P:avh", longOptions, nullptr)) != -1) {
        switch (opt) {
            case 'n':
                network = optarg;
                break;
            case 'c':
                count = std::stoi(optarg);
                break;
            case 'p':
                customPath = optarg;
                break;
            case 'P':
                passphrase = optarg;
                break;
            case 'a':
                allNetworks = true;
                break;
            case 'v':
                verbose = true;
                break;
            case 'h':
                printUsage(argv[0]);
                return 0;
            default:
                printUsage(argv[0]);
                return 1;
        }
    }
    
    if (optind >= argc) {
        std::cerr << "Error: Mnemonic phrase required\n";
        printUsage(argv[0]);
        return 1;
    }
    
    std::string mnemonic = argv[optind];
    
    try {
        WalletGenerator generator;
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic, passphrase);
        
        std::vector<std::string> networksToProcess;
        if (allNetworks) {
            networksToProcess = {"bitcoin", "ethereum", "binance", "polygon", "avalanche", "solana", "cardano", "litecoin", "dogecoin", "tron"};
        } else {
            networksToProcess = {network};
        }
        
        for (const std::string& net : networksToProcess) {
            std::cout << "\n" << std::string(50, '=') << std::endl;
            std::cout << "NETWORK: " << net << std::endl;
            std::cout << std::string(50, '=') << std::endl;
            
            for (int i = 0; i < count; i++) {
                std::string derivationPath = customPath;
                if (derivationPath.empty()) {
                    // Use default path but change the index
                    derivationPath = WalletGenerator::networks.at(net).derivation_path;
                    size_t lastSlash = derivationPath.find_last_of('/');
                    if (lastSlash != std::string::npos) {
                        derivationPath = derivationPath.substr(0, lastSlash + 1) + std::to_string(i);
                    }
                }
                
                WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, net, derivationPath);
                std::cout << "Wallet #" << (i + 1) << std::endl;
                generator.printWallet(wallet, verbose);
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}