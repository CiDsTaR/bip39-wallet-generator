#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class TestFramework {
private:
    static int totalTests;
    static int passedTests;
    static int failedTests;
    static std::vector<std::string> failures;
    static std::string currentGroup;
    static std::chrono::high_resolution_clock::time_point startTime;

public:
    static void setTestGroup(const std::string& groupName) {
        currentGroup = groupName;
        std::cout << "\n--- " << groupName << " ---" << std::endl;
        startTime = std::chrono::high_resolution_clock::now();
    }

    static void assertTrue(bool condition, const std::string& message, const char* file, int line) {
        totalTests++;
        if (condition) {
            passedTests++;
            std::cout << "✓ " << message << std::endl;
        } else {
            failedTests++;
            std::string failure = currentGroup + ": " + message + " (at " + file + ":" + std::to_string(line) + ")";
            failures.push_back(failure);
            std::cout << "✗ " << message << " [FAILED]" << std::endl;
        }
    }

    static std::string getSummary() {
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
        
        std::string summary = "=== Test Summary ===\n";
        summary += "Total Tests: " + std::to_string(totalTests) + "\n";
        summary += "Passed: " + std::to_string(passedTests) + "\n";
        summary += "Failed: " + std::to_string(failedTests) + "\n";
        
        if (totalTests > 0) {
            summary += "Success Rate: " + std::to_string((passedTests * 100) / totalTests) + "%\n";
        } else {
            summary += "Success Rate: 0%\n";
        }
        
        summary += "Execution Time: " + std::to_string(duration.count()) + "ms\n";
        
        if (failedTests > 0) {
            summary += "\nFailures:\n";
            for (const auto& failure : failures) {
                summary += "  - " + failure + "\n";
            }
        }
        
        return summary;
    }

    static bool hasFailures() {
        return failedTests > 0;
    }

    static void reset() {
        totalTests = 0;
        passedTests = 0;
        failedTests = 0;
        failures.clear();
        currentGroup = "";
        startTime = std::chrono::high_resolution_clock::now();
    }
    
    // Debug functions to help diagnose issues
    static int getTotalTests() { return totalTests; }
    static int getPassedTests() { return passedTests; }
    static int getFailedTests() { return failedTests; }
};

// Static member initialization
int TestFramework::totalTests = 0;
int TestFramework::passedTests = 0;
int TestFramework::failedTests = 0;
std::vector<std::string> TestFramework::failures;
std::string TestFramework::currentGroup = "";
std::chrono::high_resolution_clock::time_point TestFramework::startTime;

// Convenient macros
#define TEST_GROUP(name) TestFramework::setTestGroup(name)
#define TEST_ASSERT(condition, message) TestFramework::assertTrue(condition, message, __FILE__, __LINE__)
#define ASSERT_EQ(expected, actual) TEST_ASSERT((expected) == (actual), #expected " == " #actual)
#define ASSERT_NE(expected, actual) TEST_ASSERT((expected) != (actual), #expected " != " #actual)
#define ASSERT_TRUE(condition) TEST_ASSERT(condition, #condition " should be true")
#define ASSERT_FALSE(condition) TEST_ASSERT(!(condition), #condition " should be false")