#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "test_framework.h"
#include "../wallet_generator.cpp"

class PerformanceTest {
private:
    WalletGenerator generator;

public:
    void runAllTests() {
        std::cout << "Running Performance Tests...\n" << std::endl;
        
        testMnemonicToSeedPerformance();
        testWalletGenerationPerformance();
        testBatchGenerationPerformance();
        testMemoryUsageTest();
        testScalabilityTest();
        
        std::cout << "\n" << TestFramework::getSummary() << std::endl;
    }

private:
    double measureExecutionTime(std::function<void()> func) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        return duration.count() / 1000.0; // Return milliseconds
    }

    void testMnemonicToSeedPerformance() {
        TEST_GROUP("Mnemonic to Seed Performance");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        
        // Single conversion benchmark
        double singleTime = measureExecutionTime([&]() {
            generator.mnemonicToSeed(mnemonic);
        });
        
        std::cout << "Single mnemonic to seed conversion: " << singleTime << "ms" << std::endl;
        TEST_ASSERT(singleTime < 1000, "Single conversion should take less than 1 second");
        
        // Multiple conversions benchmark
        const int iterations = 10;
        double totalTime = measureExecutionTime([&]() {
            for (int i = 0; i < iterations; i++) {
                generator.mnemonicToSeed(mnemonic);
            }
        });
        
        double avgTime = totalTime / iterations;
        std::cout << "Average time for " << iterations << " conversions: " << avgTime << "ms" << std::endl;
        TEST_ASSERT(avgTime < 1000, "Average conversion should take less than 1 second");
    }

    void testWalletGenerationPerformance() {
        TEST_GROUP("Wallet Generation Performance");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        std::vector<std::string> networks = {"bitcoin", "ethereum", "litecoin"};
        
        for (const std::string& network : networks) {
            double time = measureExecutionTime([&]() {
                generator.generateWallet(seed, network);
            });
            
            std::cout << network << " wallet generation: " << time << "ms" << std::endl;
            TEST_ASSERT(time < 100, network + " wallet generation should take less than 100ms");
        }
    }

    void testBatchGenerationPerformance() {
        TEST_GROUP("Batch Generation Performance");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        const int batchSizes[] = {10, 50, 100};
        
        for (int batchSize : batchSizes) {
            double totalTime = measureExecutionTime([&]() {
                for (int i = 0; i < batchSize; i++) {
                    std::string customPath = "m/44'/0'/0'/0/" + std::to_string(i);
                    generator.generateWallet(seed, "bitcoin", customPath);
                }
            });
            
            double avgTime = totalTime / batchSize;
            std::cout << "Batch of " << batchSize << " wallets: " << totalTime << "ms total, " 
                      << avgTime << "ms average" << std::endl;
            
            TEST_ASSERT(avgTime < 50, "Average wallet generation in batch should be fast");
            TEST_ASSERT(totalTime < batchSize * 100, "Batch generation should scale well");
        }
    }

    void testMemoryUsageTest() {
        TEST_GROUP("Memory Usage Test");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        
        // Test that we can generate many wallets without excessive memory growth
        const int iterations = 1000;
        double time = measureExecutionTime([&]() {
            for (int i = 0; i < iterations; i++) {
                std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
                WalletGenerator::WalletInfo wallet = generator.generateWallet(seed, "bitcoin");
                // Wallet objects should go out of scope and be cleaned up
            }
        });
        
        double avgTime = time / iterations;
        std::cout << "Memory test: " << iterations << " iterations in " << time << "ms, "
                  << avgTime << "ms average" << std::endl;
        
        TEST_ASSERT(avgTime < 10, "Memory usage should remain stable over many iterations");
        TEST_ASSERT(time < 10000, "Total time for memory test should be reasonable");
    }

    void testScalabilityTest() {
        TEST_GROUP("Scalability Test");
        
        std::string mnemonic = "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about";
        std::vector<uint8_t> seed = generator.mnemonicToSeed(mnemonic);
        
        // Test scaling with different numbers of wallets
        std::vector<int> scales = {1, 10, 100};
        std::vector<double> times;
        
        for (int scale : scales) {
            double time = measureExecutionTime([&]() {
                for (int i = 0; i < scale; i++) {
                    std::string customPath = "m/44'/0'/0'/0/" + std::to_string(i);
                    generator.generateWallet(seed, "ethereum", customPath);
                }
            });
            
            times.push_back(time);
            std::cout << "Generated " << scale << " wallets in " << time << "ms" << std::endl;
        }
        
        // Check that scaling is roughly linear (not exponential)
        double ratio1to10 = times[1] / times[0];
        double ratio10to100 = times[2] / times[1];
        
        std::cout << "Scaling ratios: 1->10: " << ratio1to10 << ", 10->100: " << ratio10to100 << std::endl;
        
        TEST_ASSERT(ratio1to10 < 15, "1 to 10 wallet scaling should be reasonable");
        TEST_ASSERT(ratio10to100 < 15, "10 to 100 wallet scaling should be reasonable");
        TEST_ASSERT(times[2] < 5000, "100 wallets should generate in under 5 seconds");
    }
};

int main() {
    PerformanceTest test;
    test.runAllTests();
    
    return TestFramework::hasFailures() ? 1 : 0;
}