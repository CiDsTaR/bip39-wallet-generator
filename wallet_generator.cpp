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
}