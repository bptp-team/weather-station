#include "TestFramework.h"

#include <cstdio>
#include <stdexcept>

namespace testing {
namespace {

struct RegisteredTest {
  std::string name;
  TestBody body;
};

// Built on first use so registration never depends on initialization order.
std::vector<RegisteredTest> &registeredTests() {
  static std::vector<RegisteredTest> tests;
  return tests;
}

// Thrown by fail() and caught by the runner, so one broken check stops its own
// test without taking the rest of the suite down.
struct TestFailure : std::runtime_error {
  explicit TestFailure(const std::string &message)
      : std::runtime_error(message) {}
};

} // namespace

void registerTest(const char *name, TestBody body) {
  registeredTests().push_back({name, body});
}

void fail(const char *file, int line, const std::string &message) {
  throw TestFailure(std::string(file) + ":" + std::to_string(line) + ": " +
                    message);
}

std::string join(const std::vector<std::string> &items,
                 const char *separator) {
  std::string joined;

  for (const std::string &item : items) {
    if (!joined.empty()) {
      joined += separator;
    }
    joined += item;
  }

  return joined;
}

int runAllTests() {
  int failed = 0;

  for (const RegisteredTest &test : registeredTests()) {
    printf("RUN  %s\n", test.name.c_str());

    try {
      test.body();
      printf("PASS %s\n", test.name.c_str());
    } catch (const TestFailure &failure) {
      failed++;
      printf("FAIL %s\n     %s\n", test.name.c_str(), failure.what());
    } catch (const std::exception &error) {
      failed++;
      printf("FAIL %s\n     unexpected exception: %s\n", test.name.c_str(),
             error.what());
    }
  }

  printf("\n%zu test(s), %d failure(s)\n", registeredTests().size(), failed);

  return failed == 0 ? 0 : 1;
}

} // namespace testing
