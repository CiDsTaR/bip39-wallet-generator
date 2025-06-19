#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include "test_framework.h"

class IntegrationTest {
public:
    void runAllTests() {
        std::cout << "Running Integration Tests...\n" << std::endl;
        
        testCommandLineBasic();
        testCommandLineNetworks();
        testCommandLineOptions();
        testCommandLineErrorHandling();
        testBatchGeneration();
        testVerboseOutput();
        testAllNetworksFlag();
        
        std::cout << "\n" << TestFramework::getSummary() << std::endl;
    }

private:
    std::string runCommand(const std::string& command) {
        std::string result;
        FILE* pipe = popen(command.c_str(), "r");
        if (!pipe) return "ERROR";
        
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
        pclose(pipe);
        return result;
    }

    void testCommandLineBasic() {
        TEST_GROUP("Command Line Basic Functionality");
        
        // Test help command
        std::string helpOutput = runCommand("../wallet_generator -h");
        TEST_ASSERT(helpOutput.find("Usage:") != std::string::npos, "Help should show usage information");
        TEST_ASSERT(helpOutput.find("Options:") != std::string::npos, "Help should show options");
        
        // Test basic wallet generation
        std::string basicOutput = runCommand("../wallet_generator \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"");
        TEST_ASSERT(basicOutput.find("=== bitcoin Wallet ===") != std::string::npos, "Should generate Bitcoin wallet by default");
        TEST_ASSERT(basicOutput.find("Private Key:") != std::string::npos, "Should show private key");
        TEST_ASSERT(basicOutput.find("Address:") != std::string::npos, "Should show address");
    }

    void testCommandLineNetworks() {
        TEST_GROUP("Command Line Network Selection");
        
        std::vector<std::string> networks = {"bitcoin", "ethereum", "litecoin", "dogecoin"};
        
        for (const std::string& network : networks) {
            std::string command = "../wallet_generator -n " + network + " \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"";
            std::string output = runCommand(command);
            
            // Look for the exact format used in output: "=== network Wallet ==="
            std::string expectedPattern = "=== " + network + " Wallet ===";
            
            TEST_ASSERT(output.find(expectedPattern) != std::string::npos, 
                       "Should generate " + network + " wallet");
            TEST_ASSERT(output.find("Private Key:") != std::string::npos, 
                       "Should show private key for " + network);
        }
    }

    void testCommandLineOptions() {
        TEST_GROUP("Command Line Options");
        
        // Test custom derivation path
        std::string customPathOutput = runCommand("../wallet_generator -n bitcoin -p \"m/44'/0'/0'/0/5\" \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"");
        TEST_ASSERT(customPathOutput.find("m/44'/0'/0'/0/5") != std::string::npos, "Should use custom derivation path");
        
        // Test passphrase
        std::string passphraseOutput = runCommand("../wallet_generator -P \"test_passphrase\" \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"");
        TEST_ASSERT(passphraseOutput.find("Private Key:") != std::string::npos, "Should generate wallet with passphrase");
        
        // Test verbose output
        std::string verboseOutput = runCommand("../wallet_generator -v \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"");
        TEST_ASSERT(verboseOutput.find("Public Key:") != std::string::npos, "Verbose should show public key");
        TEST_ASSERT(verboseOutput.find("WIF:") != std::string::npos, "Verbose should show WIF");
    }

    void testCommandLineErrorHandling() {
        TEST_GROUP("Command Line Error Handling");
        
        // Test missing mnemonic
        std::string noMnemonicOutput = runCommand("../wallet_generator 2>&1");
        TEST_ASSERT(noMnemonicOutput.find("Error") != std::string::npos, "Should show error for missing mnemonic");
        
        // Test invalid network
        std::string invalidNetworkOutput = runCommand("../wallet_generator -n invalid_network \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\" 2>&1");
        TEST_ASSERT(invalidNetworkOutput.find("Error") != std::string::npos || 
                   invalidNetworkOutput.find("Unsupported") != std::string::npos, 
                   "Should show error for invalid network");
        
        // Test invalid option
        std::string invalidOptionOutput = runCommand("../wallet_generator --invalid-option \"test\" 2>&1");
        TEST_ASSERT(invalidOptionOutput.find("Usage:") != std::string::npos || 
                   invalidOptionOutput.find("Error") != std::string::npos,
                   "Should show usage or error for invalid option");
    }

    void testBatchGeneration() {
        TEST_GROUP("Batch Generation");
        
        // Test generating multiple wallets
        std::string batchOutput = runCommand("../wallet_generator -n ethereum -c 3 \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"");
        
        int walletCount = 0;
        size_t pos = 0;
        while ((pos = batchOutput.find("Wallet #", pos)) != std::string::npos) {
            walletCount++;
            pos++;
        }
        
        TEST_ASSERT(walletCount == 3, "Should generate exactly 3 wallets");
        TEST_ASSERT(batchOutput.find("Wallet #1") != std::string::npos, "Should show Wallet #1");
        TEST_ASSERT(batchOutput.find("Wallet #2") != std::string::npos, "Should show Wallet #2");
        TEST_ASSERT(batchOutput.find("Wallet #3") != std::string::npos, "Should show Wallet #3");
    }

    void testVerboseOutput() {
        TEST_GROUP("Verbose Output");
        
        std::string verboseOutput = runCommand("../wallet_generator -v -n ethereum \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"");
        
        TEST_ASSERT(verboseOutput.find("Private Key:") != std::string::npos, "Should show private key");
        TEST_ASSERT(verboseOutput.find("Public Key:") != std::string::npos, "Should show public key");
        TEST_ASSERT(verboseOutput.find("WIF:") != std::string::npos, "Should show WIF");
        TEST_ASSERT(verboseOutput.find("Address:") != std::string::npos, "Should show address");
        TEST_ASSERT(verboseOutput.find("Derivation Path:") != std::string::npos, "Should show derivation path");
    }

    void testAllNetworksFlag() {
        TEST_GROUP("All Networks Flag");
        
        std::string allNetworksOutput = runCommand("../wallet_generator -a \"abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about\"");
        
        // Check for each network
        std::vector<std::string> expectedNetworks = {
            "bitcoin", "ethereum", "binance", "polygon", "avalanche",
            "solana", "cardano", "litecoin", "dogecoin", "tron"
        };
        
        for (const std::string& network : expectedNetworks) {
            TEST_ASSERT(allNetworksOutput.find("NETWORK: " + network) != std::string::npos, 
                       "Should generate wallet for " + network);
        }
    }
};

int main() {
    // Check if the wallet_generator binary exists in parent directory
    std::ifstream binary("../wallet_generator");
    if (!binary.good()) {
        std::cout << "Error: wallet_generator binary not found in parent directory. Please run 'make' first." << std::endl;
        return 1;
    }
    
    IntegrationTest test;
    test.runAllTests();
    
    return TestFramework::hasFailures() ? 1 : 0;
}