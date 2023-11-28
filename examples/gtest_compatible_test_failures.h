#pragma once

// gtest or simple_test shall be included prior to this one.

#include <cassert>
#include <vector>

#define UNREACHABLE_CODE() assert(!"unreachable code")  // will crash

struct Sentry {
  void (*f)() = nullptr;
  void checkpoint() { f = nullptr; }
  ~Sentry() { if (f) f(); }
};

#define BEGIN_REACHABLE_CODE() Sentry sentry{[](){assert(!"checkpoint not reached");}}
#define END_REACHABLE_CODE() sentry.checkpoint()

TEST(simple_test, lots_of_failed_expectations) {
  BEGIN_REACHABLE_CODE();

  EXPECT_EQ(123, 456);
  EXPECT_TRUE(false);
  EXPECT_STREQ("hello", "world");
  EXPECT_STRNE("hello\0one", "hello\0two");
  std::cout << "some went wrong (or not...)" << std::endl;

  END_REACHABLE_CODE();
}

TEST(simple_test, some_assertion_failed) {
  ASSERT_EQ(123, 456);
  UNREACHABLE_CODE();
}

TEST(simple_test, raised_exception) {
  throw std::runtime_error("ooo");
  UNREACHABLE_CODE();
}

TEST(simple_test, some_fault_1) {
  FAIL() << "comment " << 123 << " goes here";
  UNREACHABLE_CODE();
}

TEST(simple_test, some_fault_2) {
  FAIL();
  UNREACHABLE_CODE();
}

TEST(simple_test, some_fault_3) {
  BEGIN_REACHABLE_CODE();

  ADD_FAILURE();
  ADD_FAILURE() << "hello";
  std::cout << "here we are!" << std::endl;

  END_REACHABLE_CODE();
}

TEST(simple_test, compare_floats_implicitly_failed) {
  const auto pivot = 123.4;
  const auto epsilon = 0.1;
  const auto tiny = 0.001;

  BEGIN_REACHABLE_CODE();

  EXPECT_NEAR(pivot, pivot - epsilon - tiny, epsilon);
  EXPECT_NEAR(pivot, pivot + epsilon + tiny, epsilon);

  END_REACHABLE_CODE();
}

TEST(simple_test, compare_types_failed) {
  BEGIN_REACHABLE_CODE();

  EXPECT_EQ(typeid(int), typeid(char));

  END_REACHABLE_CODE();
}
