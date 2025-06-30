#pragma once

#include <iostream>
#include <string>
#include <cstdlib>
#include <functional>
#include <string>
#include <vector>
#include <map>

namespace Vantor::Test {

    // Tracks how many VASSERT failures occurred
    inline int failedAssertions = 0;

    // Prints a summary after tests are run
    inline void PrintTestSummary() {
        if (failedAssertions == 0) {
            std::cout << "[VTEST] All assertions passed.\n";
        } else {
            std::cerr << "[VTEST] " << failedAssertions 
                      << " assertion(s) failed.\n";
        }
    }

    using VTestCaseFunc = std::function<void()>;

    struct VTestCase {
        std::string name;
        VTestCaseFunc func;
    };

    inline std::map<std::string, std::vector<VTestCase>>& GetTestRegistry() {
        static std::map<std::string, std::vector<VTestCase>> registry;
        return registry;
    }

    inline void RegisterTest(const std::string& module, const std::string& testName, VTestCaseFunc func) {
        GetTestRegistry()[module].push_back({testName, func});
    }

} // namespace Vantor::Test

// VASSERT (Non-Fatal Assertion)
#define VASSERT(condition, message)                                                \
    do {                                                                           \
        if (!(condition)) {                                                       \
            std::cerr << "[VASSERT FAILED] " << __FILE__ << ":" << __LINE__       \
                      << " → " << #condition << "\n"                               \
                      << "Message: " << message << "\n";                           \
            ++Vantor::Test::failedAssertions;                                      \
        }                                                                          \
    } while (0)

// VASSERT_FATAL (Fatal Assertion)
#define VASSERT_FATAL(condition, message)                                          \
    do {                                                                           \
        if (!(condition)) {                                                       \
            std::cerr << "[VASSERT_FATAL] " << __FILE__ << ":" << __LINE__        \
                      << " → " << #condition << "\n"                               \
                      << "Message: " << message << std::endl;                     \
            std::exit(EXIT_FAILURE);                                               \
        }                                                                          \
    } while (0)

// Helper to generate unique variable names
#define VTEST_CONCAT_INTERNAL(x, y) x##y
#define VTEST_CONCAT(x, y) VTEST_CONCAT_INTERNAL(x, y)

// Macro to register a test case in a module
#define VTEST_CASE(moduleName, testName)                                           \
    static void VTEST_CONCAT(testName, _func)();                                  \
    namespace {                                                                   \
        struct VTEST_CONCAT(testName, _registrar) {                              \
            VTEST_CONCAT(testName, _registrar)() {                               \
                Vantor::Test::RegisterTest(moduleName, #testName, &VTEST_CONCAT(testName, _func)); \
            }                                                                     \
        } VTEST_CONCAT(testName, _instance);                                     \
    }                                                                             \
    static void VTEST_CONCAT(testName, _func)()
