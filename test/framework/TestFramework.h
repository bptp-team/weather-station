#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

// Minimal test runner. Tests register themselves with the TEST macro and the
// runner reports every failure, so the suite needs no external dependency and
// CI can build it anywhere.

#include <sstream>
#include <string>
#include <vector>

namespace testing {

using TestBody = void (*)();

void registerTest(const char *name, TestBody body);

// Runs every registered test and returns the process exit code.
int runAllTests();

// Aborts the running test with a failure message.
[[noreturn]] void fail(const char *file, int line, const std::string &message);

// Registers a test while the program starts up.
struct Registrar {
  Registrar(const char *name, TestBody body) { registerTest(name, body); }
};

template <typename T> std::string describe(const T &value) {
  std::ostringstream text;
  text << value;
  return text.str();
}

std::string join(const std::vector<std::string> &items,
                 const char *separator);

} // namespace testing

#define TEST(testName)                                                         \
  static void testName();                                                      \
  static const ::testing::Registrar registrar_##testName(#testName, testName); \
  static void testName()

#define CHECK_TRUE(condition)                                                  \
  do {                                                                         \
    if (!(condition)) {                                                        \
      ::testing::fail(__FILE__, __LINE__, "expected " #condition " to hold");  \
    }                                                                          \
  } while (false)

#define CHECK_EQ(actual, expected)                                             \
  do {                                                                         \
    const auto actualValue = (actual);                                         \
    const auto expectedValue = (expected);                                     \
    if (!(actualValue == expectedValue)) {                                     \
      ::testing::fail(__FILE__, __LINE__,                                      \
                      #actual " is " + ::testing::describe(actualValue) +      \
                          ", expected " +                                      \
                          ::testing::describe(expectedValue));                 \
    }                                                                          \
  } while (false)

#define CHECK_TEXT_EQ(actual, expected)                                        \
  do {                                                                         \
    const std::string actualText((actual));                                    \
    const std::string expectedText((expected));                                \
    if (actualText != expectedText) {                                          \
      ::testing::fail(__FILE__, __LINE__,                                      \
                      std::string(#actual) + " is\n  \"" + actualText +        \
                          "\"\nexpected\n  \"" + expectedText + "\"");         \
    }                                                                          \
  } while (false)

#endif
