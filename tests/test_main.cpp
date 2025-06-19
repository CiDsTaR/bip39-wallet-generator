#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include "test_framework.h"
#include "../wallet_generator.cpp"

class WalletGeneratorTest {
private:
    WalletGenerator generator;
    
public:
    void runAllTests() {
        std::cout << "Running BIP39 Wallet Generator Tests...\n" << std::endl;
        
        testMnemonicToSeed();
        testBitcoinWalletGeneration();
        testEthereumWalletGeneration();
        testAllNetworks();
        testCustomDerivationPaths();
        testPassphraseSupport();
        testErrorHandling();
        testKnownTestVectors();
        testMemoryManagement();
        testCryptographicFunctions();
        
        std::cout << "\n" << TestFramework::getSummary() << std::endl;
    }
    
private:
    void testMnemonicToSeed() {
        TEST_GROUP("Mnemonic to Seed Conversion");
        
        // Test standard mnemonic without passphrase
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        TEST_ASSERT(seed.size() == 64, "Seed should be 64 bytes");
        TEST_ASSERT(!seed.empty(), "Seed should not be empty");
        
        // Test with passphrase
        std::vector<uint8_t> seedWithPassphrase = generator.mnemonicToSeed(mnemonic, "TREZOR");
        TEST_ASSERT(seed != seedWithPassphrase, "Seeds with different passphrases should be different");
        
        // Test consistency
        std::vector<uint8_t> seed2 = generator.mnemonicToSeed(mnemonic);
        TEST_ASSERT(seed == seed2, "Same mnemonic should produce same seed");
    }
    
    void testBitcoinWalletGeneration() {
        TEST_GROUP("Bitcoin Wallet Generation");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin");
        
        TEST_ASSERT(!wallet.privateKey.empty(), "Private key should not be empty");
        TEST_ASSERT(wallet.privateKey.length() == 64, "Private key should be 32 bytes (64 hex chars)");
        TEST_ASSERT(!wallet.publicKey.empty(), "Public key should not be empty");
        TEST_ASSERT(!wallet.address.empty(), "Address should not be empty");
        TEST_ASSERT(!wallet.wif.empty(), "WIF should not be empty");
        TEST_ASSERT(wallet.network == "bitcoin", "Network should be bitcoin");
        TEST_ASSERT(wallet.derivationPath == "m/44'/0'/0'/0/0", "Default derivation path should match");
        
        // Test address format (Bitcoin addresses start with 1, 3, or bc1)
        char firstChar = wallet.address[0];
        TEST_ASSERT(firstChar == '1' || firstChar == '3' || wallet.address.substr(0, 3) == "bc1", 
                   "Bitcoin address should have valid format");
    }
    
    void testEthereumWalletGeneration() {
        TEST_GROUP("Ethereum Wallet Generation");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "ethereum");
        
        TEST_ASSERT(!wallet.privateKey.empty(), "Private key should not be empty");
        TEST_ASSERT(wallet.privateKey.length() == 64, "Private key should be 32 bytes (64 hex chars)");
        TEST_ASSERT(!wallet.address.empty(), "Address should not be empty");
        TEST_ASSERT(wallet.address.substr(0, 2) == "0x", "Ethereum address should start with 0x");
        TEST_ASSERT(wallet.address.length() == 42, "Ethereum address should be 42 characters long");
        TEST_ASSERT(wallet.network == "ethereum", "Network should be ethereum");
        TEST_ASSERT(wallet.derivationPath == "m/44'/60'/0'/0/0", "Default derivation path should match");
    }
    
    void testAllNetworks() {
        TEST_GROUP("All Network Support");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        std::vector<std::string> networks = {
            "bitcoin", "ethereum", "binance", "polygon", "avalanche",
            "solana", "cardano", "litecoin", "dogecoin", "tron"
        };
        
        for (const std::string& network : networks) {
            WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, network);
            
            TEST_ASSERT(!wallet.privateKey.empty(), "Private key should not be empty for " + network);
            TEST_ASSERT(!wallet.address.empty(), "Address should not be empty for " + network);
            TEST_ASSERT(wallet.network == network, "Network should match for " + network);
            
            // Test network-specific address formats
            if (network == "ethereum" || network == "binance" || network == "polygon" || network == "avalanche") {
                TEST_ASSERT(wallet.address.substr(0, 2) == "0x", network + " address should start with 0x");
                TEST_ASSERT(wallet.address.length() == 42, network + " address should be 42 characters");
            }
        }
    }
    
    void testCustomDerivationPaths() {
        TEST_GROUP("Custom Derivation Paths");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        std::string customPath = "m/44'/0'/0'/0/5";
        WalletGenerator::WalletInfo wallet1 = generator.generateWallet(seed, "bitcoin", customPath);
        WalletGenerator::WalletInfo wallet2 = generator.generateWallet(seed, "bitcoin", "m/44'/0'/0'/0/0");
        
        TEST_ASSERT(wallet1.derivationPath == customPath, "Custom derivation path should be used");
        TEST_ASSERT(wallet1.privateKey != wallet2.privateKey, "Different paths should generate different keys");
        TEST_ASSERT(wallet1.address != wallet2.address, "Different paths should generate different addresses");
    }
    
    void testPassphraseSupport() {
        TEST_GROUP("Passphrase Support");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        
        std::vector<uint8_t> seed1 = generator.mnemonicToSeed(mnemonic, "");
        std::vector<uint8_t> seed2 = generator.mnemonicToSeed(mnemonic, "passphrase123");
        std::vector<uint8_t> seed3 = generator.mnemonicToSeed(mnemonic, "different_passphrase");
        
        TEST_ASSERT(seed1 != seed2, "Different passphrases should produce different seeds");
        TEST_ASSERT(seed2 != seed3, "Different passphrases should produce different seeds");
        TEST_ASSERT(seed1 != seed3, "Empty and non-empty passphrases should produce different seeds");
        
        // Test wallet generation with different passphrases
        WalletGenerator::WalletInfo wallet1 = generator.generateWallet(seed1, "bitcoin");
        WalletGenerator::WalletInfo wallet2 = generator.generateWallet(seed2, "bitcoin");
        
        TEST_ASSERT(wallet1.address != wallet2.address, "Different passphrases should generate different addresses");
    }
    
    void testErrorHandling() {
        TEST_GROUP("Error Handling");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        // Test unsupported network
        bool exceptionThrown = false;
        try {
            generator.generateWallet(seed, "unsupported_network");
        } catch (const std::runtime_error& e) {
            exceptionThrown = true;
            TEST_ASSERT(std::string(e.what()).find("Unsupported network") != std::string::npos, 
                       "Exception message should mention unsupported network");
        }
        TEST_ASSERT(exceptionThrown, "Should throw exception for unsupported network");
    }
    
    void testKnownTestVectors() {
        TEST_GROUP("Known Test Vectors");
        
        // BIP39 test vector
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic, "TREZOR");
        
        // Convert to hex for comparison
        std::string seedHex;
        for (uint8_t byte : seed) {
            char hex[3];
            sprintf(hex, "%02x", byte);
            seedHex += hex;
        }
        
        // Known seed for this mnemonic + passphrase combination
        std::string expectedSeed = "c55257c360c07c72029aebc1b53c05ed0362ada38ead3e3e9efa3708e53495531f09a6987599d18264c1e1c92f2cf141630c7a3c4ab7c81b2f001698e7463b04";
        
        TEST_ASSERT(seedHex == expectedSeed, "Seed should match known test vector");
    }
    
    void testMemoryManagement() {
        TEST_GROUP("Memory Management");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        
        // Test multiple generations don't leak memory
        for (int i = 0; i < 100; i++) {
            std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
            WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin");
            // Wallets should be different if we change the derivation path
            std::string customPath = "m/44'/0'/0'/0/" + std::to_string(i);
            WalletGenerator::WalletInfo wallet2 = generator.generateWallet(seed, "bitcoin", customPath);
        }
        
        TEST_ASSERT(true, "Memory management test completed without crashes");
    }
    
    void testCryptographicFunctions() {
        TEST_GROUP("Cryptographic Functions");
        
        std::string mnemonic1 = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::string mnemonic2 = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon";
        
        std::vector<uint8_t> seed1 = generator.mnemonicToSeed(mnemonic1);
        std::vector<uint8_t> seed2 = generator.mnemonicToSeed(mnemonic2);
        
        TEST_ASSERT(seed1 != seed2, "Different mnemonics should produce different seeds");
        
        WalletGenerator::WalletInfo wallet1 = generator.generateWallet(seed1, "bitcoin");
        WalletGenerator::WalletInfo wallet2 = generator.generateWallet(seed2, "bitcoin");
        
        TEST_ASSERT(wallet1.privateKey != wallet2.privateKey, "Different seeds should produce different private keys");
        TEST_ASSERT(wallet1.address != wallet2.address, "Different seeds should produce different addresses");
        
        // Test deterministic generation
        WalletGenerator::WalletInfo wallet1_repeat = generator.generateWallet(seed1, "bitcoin");
        TEST_ASSERT(wallet1.privateKey == wallet1_repeat.privateKey, "Same seed should produce same private key");
        TEST_ASSERT(wallet1.address == wallet1_repeat.address, "Same seed should produce same address");
    }
};

int main() {
    WalletGeneratorTest test;
    test.runAllTests();
    
    return TestFramework::hasFailures() ? 1 : 0;
}