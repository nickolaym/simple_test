#pragma once

// gtest or simple_test shall be included prior to this one.

#include <cassert>
#include <vector>

TEST(simple_test, lots_of_failed_expectations) {
  EXPECT_EQ(123, 456);
  EXPECT_TRUE(false);
  EXPECT_STREQ("hello", "world");
  EXPECT_STRNE("hello\0one", "hello\0two");
  std::cout << "some went wrong (or not...)" << std::endl;
}

TEST(simple_test, some_assertion_failed) {
  ASSERT_EQ(123, 456);
  assert(false);  // unreachable
}

TEST(simple_test, raised_exception) {
  throw std::runtime_error("ooo");
  assert(false);  // unreachable
}

TEST(simple_test, some_fault_1) {
  FAIL();  // comments not supported yet
  assert(false);  // unreachable
}

TEST(simple_test, compare_floats_implicitly_failed) {
  const auto pivot = 123.4;
  const auto epsilon = 0.1;
  const auto tiny = 0.001;

  EXPECT_NEAR(pivot, pivot - epsilon - tiny, epsilon);
  EXPECT_NEAR(pivot, pivot + epsilon + tiny, epsilon);
}

TEST(simple_test, compare_types_failed) {
  EXPECT_EQ(typeid(int), typeid(char));
}
