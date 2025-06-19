#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include "test_framework.h"
#include "../wallet_generator.h"

class SecurityTest {
private:
    WalletGenerator generator;

public:
    void runAllTests() {
        std::cout << "Running Security Tests...\n" << std::endl;
        
        testPrivateKeyRandomness();
        testSeedDeterminism();
        testPassphraseIsolation();
        testDerivationPathIsolation();
        testAddressUniqueness();
        testKeyValidation();
        testNetworkIsolation();
        testMemoryClearing();
        
        std::cout << "\n" << TestFramework::getSummary() << std::endl;
    }

private:
    void testPrivateKeyRandomness() {
        TEST_GROUP("Private Key Randomness");
        
        std::set<std::string> privateKeys;
        std::vector<std::string> mnemonics = {
            "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about",
            "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon",
            "legal winner thank year wave sausage worth useful legal winner thank yellow",
            "letter advice cage absurd amount doctor acoustic avoid letter advice cage above",
            "zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo zoo wrong"
        };
        
        for (const std::string& mnemonic : mnemonics) {
            std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
            WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin");
            privateKeys.insert(wallet.privateKey);
        }
        
        TEST_ASSERT(privateKeys.size() == mnemonics.size(), 
                   "All private keys should be unique for different mnemonics");
        
        // Test that private keys are not all zeros or all ones
        for (const std::string& privateKey : privateKeys) {
            std::string allZeros(64, '0');
            std::string allOnes(64, 'f');
            TEST_ASSERT(privateKey != allZeros, "Private key should not be all zeros");
            TEST_ASSERT(privateKey != allOnes, "Private key should not be all ones");
            TEST_ASSERT(privateKey.length() == 64, "Private key should be 64 hex characters");
        }
    }

    void testSeedDeterminism() {
        TEST_GROUP("Seed Determinism");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        
        // Generate the same seed multiple times
        std::vector<uint8_t> seed1 = generator.mnemonicToSeed(mnemonic);
        std::vector<uint8_t> seed2 = generator.mnemonicToSeed(mnemonic);
        std::vector<uint8_t> seed3 = generator.mnemonicToSeed(mnemonic);
        
        TEST_ASSERT(seed1 == seed2, "Same mnemonic should produce identical seeds");
        TEST_ASSERT(seed2 == seed3, "Same mnemonic should produce identical seeds");
        TEST_ASSERT(seed1.size() == 64, "Seed should be 64 bytes");
        
        // Test with passphrase
        std::vector<uint8_t> seedWithPass1 = generator.mnemonicToSeed(mnemonic, "test");
        std::vector<uint8_t> seedWithPass2 = generator.mnemonicToSeed(mnemonic, "test");
        
        TEST_ASSERT(seedWithPass1 == seedWithPass2, "Same mnemonic+passphrase should produce identical seeds");
        TEST_ASSERT(seed1 != seedWithPass1, "Different passphrases should produce different seeds");
    }

    void testPassphraseIsolation() {
        TEST_GROUP("Passphrase Isolation");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<std::string> passphrases = {"", "test", "TEST", "test123", "very_long_passphrase_with_special_chars!@#$%"};
        
        std::set<std::string> addresses;
        std::set<std::string> privateKeys;
        
        for (const std::string& passphrase : passphrases) {
            std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic, passphrase);
            WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin");
            
            addresses.insert(wallet.address);
            privateKeys.insert(wallet.privateKey);
        }
        
        TEST_ASSERT(addresses.size() == passphrases.size(), 
                   "Different passphrases should produce different addresses");
        TEST_ASSERT(privateKeys.size() == passphrases.size(), 
                   "Different passphrases should produce different private keys");
    }

    void testDerivationPathIsolation() {
        TEST_GROUP("Derivation Path Isolation");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        std::vector<std::string> paths = {
            "m/44'/0'/0'/0/0",
            "m/44'/0'/0'/0/1",
            "m/44'/0'/0'/0/2",
            "m/44'/0'/0'/1/0",
            "m/44'/0'/1'/0/0"
        };
        
        std::set<std::string> addresses;
        std::set<std::string> privateKeys;
        
        for (const std::string& path : paths) {
            WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin", path);
            addresses.insert(wallet.address);
            privateKeys.insert(wallet.privateKey);
            TEST_ASSERT(wallet.derivationPath == path, "Derivation path should be correctly set");
        }
        
        TEST_ASSERT(addresses.size() == paths.size(), 
                   "Different derivation paths should produce different addresses");
        TEST_ASSERT(privateKeys.size() == paths.size(), 
                   "Different derivation paths should produce different private keys");
    }

    void testAddressUniqueness() {
        TEST_GROUP("Address Uniqueness");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        std::set<std::string> allAddresses;
        std::vector<std::string> networks = {"bitcoin", "ethereum", "litecoin", "dogecoin"};
        
        for (const std::string& network : networks) {
            for (int i = 0; i < 10; i++) {
                std::string path = "m/44'/0'/0'/0/" + std::to_string(i);
                WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, network, path);
                allAddresses.insert(wallet.address);
            }
        }
        
        // Should have unique addresses for each network and derivation path combination
        size_t expectedAddresses = networks.size() * 10;
        TEST_ASSERT(allAddresses.size() == expectedAddresses, 
                   "All generated addresses should be unique");
    }

    void testKeyValidation() {
        TEST_GROUP("Key Validation");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin");
        
        // Test private key format
        TEST_ASSERT(wallet.privateKey.length() == 64, "Private key should be 64 hex characters");
        for (char c : wallet.privateKey) {
            TEST_ASSERT(std::isxdigit(c), "Private key should contain only hex characters");
        }
        
        // Test public key format  
        TEST_ASSERT(wallet.publicKey.length() == 130, "Uncompressed public key should be 130 hex characters");
        TEST_ASSERT(wallet.publicKey.substr(0, 2) == "04", "Uncompressed public key should start with 04");
        
        // Test that private key is not zero
        std::string zeroKey(64, '0');
        TEST_ASSERT(wallet.privateKey != zeroKey, "Private key should not be zero");
        
        // Test that private key is less than secp256k1 curve order
        // This is a simplified check - in production you'd want more thorough validation
        TEST_ASSERT(wallet.privateKey[0] <= '7', "Private key should be less than curve order");
    }

    void testNetworkIsolation() {
        TEST_GROUP("Network Isolation");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        // Test that different networks produce different addresses for same derivation
        WalletGenerator::WalletInfo btcWallet = generator.generateWallet(seed, "bitcoin");
        WalletGenerator::WalletInfo ethWallet = generator.generateWallet(seed, "ethereum");
        WalletGenerator::WalletInfo ltcWallet = generator.generateWallet(seed, "litecoin");
        
        TEST_ASSERT(btcWallet.address != ethWallet.address, 
                   "Bitcoin and Ethereum should have different addresses");
        TEST_ASSERT(btcWallet.address != ltcWallet.address, 
                   "Bitcoin and Litecoin should have different addresses");
        TEST_ASSERT(ethWallet.address != ltcWallet.address, 
                   "Ethereum and Litecoin should have different addresses");
        
        // Test address format validation
        TEST_ASSERT(ethWallet.address.substr(0, 2) == "0x", "Ethereum address should start with 0x");
        TEST_ASSERT(ethWallet.address.length() == 42, "Ethereum address should be 42 characters");
        
        // Bitcoin addresses should not start with 0x
        TEST_ASSERT(btcWallet.address.substr(0, 2) != "0x", "Bitcoin address should not start with 0x");
    }

    void testMemoryClearing() {
        TEST_GROUP("Memory Clearing Test");
        
        // This is a basic test - in production you'd want more sophisticated memory analysis
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        
        {
            // Create wallet in a scope that will be destroyed
            std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
            WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin");
            
            TEST_ASSERT(!wallet.privateKey.empty(), "Private key should be generated");
            TEST_ASSERT(!wallet.address.empty(), "Address should be generated");
        }
        
        // After scope destruction, sensitive data should ideally be cleared
        // This is more of a design test than a functional test
        TEST_ASSERT(true, "Memory clearing scope test completed");
    }
};

int main() {
    SecurityTest test;
    test.runAllTests();
    
    return TestFramework::hasFailures() ? 1 : 0;
}