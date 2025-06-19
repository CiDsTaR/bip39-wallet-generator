#include "wallet_generator.h"
#include <getopt.h>

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
} SECP256K1_EC_UNCOMPRESSED);
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
}