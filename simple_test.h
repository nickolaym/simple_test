#pragma once

#include <unistd.h>
#include <iostream>
#include <exception>

#define FORWARD(x) std::forward<decltype(x)>(x)

namespace simple_print {

static const char* bar = "-------------------";
static const char* barbar = "===================";
static const char* red = "\x1b[31m";
static const char* green = "\x1b[32m";
static const char* yellow = "\x1b[33m";
static const char* blue = "\x1b[34m";
static const char* normal = "\x1b[0m";

inline void print(const char* color, auto&& ... args) {
  bool tty = (isatty(fileno(stdout)));
  if (tty) std::cout << color;
  ((std::cout << FORWARD(args)) , ...);
  if (tty) std::cout << normal;
  std::cout << std::endl;
}

}  // namespace simple_print

namespace simple_test {

struct assertion_fault {};  // out of std::exception hierarchy

struct TestCase {
  static TestCase*& first() { static TestCase* t = nullptr; return t; }
  static TestCase*& last() { static TestCase* t = nullptr; return t; }

  static TestCase*& current() { static TestCase* t = nullptr; return t; }

  // chain
  TestCase* m_next = nullptr;
  const char* m_name;
  void (*m_func)();
  bool m_enabled;

  // result
  bool m_called = false;
  bool m_passed = false;

  TestCase(const char* name, void(*func)(), bool enabled = true)
    : m_name(name)
    , m_func(func)
    , m_enabled(enabled)
  {
    if (first()) {
      last() = last()->m_next = this;
    } else {
      last() = first() = this;
    }
  }

  static bool run_all() {
    int num_skipped = 0, num_passed = 0, num_failed = 0;
    for (TestCase* t = first(); t; t = t->m_next) {
      if (!t->m_enabled) {
        num_skipped++;
        continue;
      }

      current() = t;

      simple_print::print(simple_print::blue, t->m_name, " running...");
      simple_print::print(simple_print::blue, simple_print::bar);
      try {
        t->m_passed = true;  // could be reset in the func
        t->m_func();
      } catch (assertion_fault) {
        t->m_passed = false;
      } catch (const std::exception& e) {
        t->m_passed = false;
        simple_print::print(simple_print::red, t->m_name, " raised an exception ", e.what());
      } catch (...) {
        t->m_passed = false;
        simple_print::print(simple_print::red, t->m_name, " raised an exception");
      }

      if (t->m_passed) {
        num_passed++;
        simple_print::print(simple_print::green, simple_print::bar);
        simple_print::print(simple_print::green, t->m_name, " PASSED");
      } else {
        num_failed++;
        simple_print::print(simple_print::red, simple_print::bar);
        simple_print::print(simple_print::red, t->m_name, " FAILED");
      }

      current() = nullptr;
      simple_print::print(simple_print::normal, "");
    }

    simple_print::print(simple_print::normal, simple_print::barbar);
    if (num_passed)  simple_print::print(simple_print::green,  "passed:  ", num_passed);
    if (num_failed)  simple_print::print(simple_print::red,    "failed:  ", num_failed);
    if (num_skipped) simple_print::print(simple_print::blue,   "skipped: ", num_skipped);

    return !num_failed;
  }
};

inline void test_failed(bool assertion) {
  TestCase::current()->m_passed = false;
  if (assertion) throw assertion_fault{};
}

// testing functions
inline bool expect_comparison(
    const char* file, int line,
    const char* aexpr, const auto& a,
    const char* bexpr, const auto& b,
    bool assertion,
    auto opfunc, const char* opexpr) {
  bool passed = opfunc(a, b);
  auto color = passed ? simple_print::green : assertion ? simple_print::red : simple_print::yellow;
  const char* category = assertion ? "assertion" : "expectation";
  const char* verdict = passed ? "passed" : "failed";
  simple_print::print(color, file, ":", line);
  simple_print::print(color, "  ", category, " ", verdict, ": ", aexpr, " ", opexpr, " ", bexpr);
  simple_print::print(color, "    left : ", std::boolalpha, a);
  simple_print::print(color, "    right: ", std::boolalpha, b);
  simple_print::print(color);
  if (passed) return true;
  test_failed(assertion);
  return false;
}

inline bool examine_fault(const char* file, int line, auto&& ... comments) {
  simple_print::print(simple_print::red, file, ":", line);
  simple_print::print(simple_print::red, "  explicitly failed");
  if constexpr (sizeof...(comments) > 0) {
    simple_print::print(simple_print::red, "    ", FORWARD(comments)...);
  }
  test_failed(true);
  return false;
}

inline int testing_main(int argc, char** argv) {
  return !simple_test::TestCase::run_all();
}

}  // namespace simple_test

#define TEST(name, ...) \
    void func_##name(); \
    simple_test::TestCase test_##name(#name, func_##name ,##__VA_ARGS__); \
    void func_##name() /* test body goes here */

#define EXAMINE(a, b, assertion, ...) \
    simple_test::expect_comparison(__FILE__, __LINE__, #a, a, #b, b, assertion, ##__VA_ARGS__)

#define EXAMINE_CMP(a, op, b, assertion) \
    EXAMINE(a, b, assertion, \
        [](const auto& x, const auto& y) { return x op y; }, #op)
#define ASSERT_CMP(a, op, b) EXAMINE_CMP(a, op, b, true)
#define EXPECT_CMP(a, op, b) EXAMINE_CMP(a, op, b, false)

#define EXAMINE_STRCMP(a, op, b, assertion) \
    EXAMINE(a, b, assertion, \
        [](const auto& x, const auto& y) { return strcmp(x, y) op 0; }, "[strcmp]" #op)
#define ASSERT_STRCMP(a, op, b) EXAMINE_STRCMP(a, op, b, true)
#define EXPECT_STRCMP(a, op, b) EXAMINE_STRCMP(a, op, b, false)

#define EXAMINE_BOOL(a, b, assertion) \
    EXAMINE(a, b, assertion, \
        [](const auto& x, const auto& y) { return (bool)a == (bool)b; }, "is")
#define ASSERT_BOOL(a, b) EXAMINE_BOOL(a, b, true)
#define EXPECT_BOOL(a, b) EXAMINE_BOOL(a, b, false)

#define ASSERTION_FAULT(...) simple_test::examine_fault(__FILE__, __LINE__, ##__VA_ARGS__)

#define TESTING_MAIN() \
    int main(int argc, char** argv) { return simple_test::testing_main(argc, argv); }
