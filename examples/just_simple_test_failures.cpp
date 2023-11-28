#include "../simple_test.h"
#include <cassert>

#include <vector>

TEST(should_fail, vector_capacity) {
  std::vector<int> xs;
  xs.reserve(100500);
  xs.clear();
  EXPECT_CMP(xs.capacity(), ==, 0U) << " ahaha ? ahaha!";
  ASSERT_CMP(xs.size(), ==, 1U) << " ahaha ? ahaha!";
}

TEST(should_fail, lots_of_failed_expectations) {
  EXPECT_CMP(123, ==, 456);
  EXPECT_BOOL(true, false);
  EXPECT_STRCMP("hello", >, "world");
  EXPECT_STRCMP("hello\0one", !=, "hello\0two");
  std::cout << "some went wrong (or not...)" << std::endl;
}

TEST(should_fail, some_assertion_failed) {
  ASSERT_CMP(123, ==, 456);
  assert(false);  // unreachable
}

TEST(should_fail, raised_exception) {
  throw std::runtime_error("ooo");
  assert(false);  // unreachable
}

TEST(should_fail, some_fault_1) {
  ASSERTION_FAULT() << "comment " << 123 << " goes here";
  assert(false);  // unreachable
}

TEST(should_fail, some_fault_2) {
  ASSERTION_FAULT();
  assert(false);  // unreachable
}

TEST(should_fail, some_fault_3) {
  EXPECTATION_FAULT();
  EXPECTATION_FAULT() << "hello";
  std::cout << "here we are!" << std::endl;
}

TEST(should_fail, strings) {
  EXPECT_EQ("aaa\x11", "aaa\x12") << simple_print::verbose("bbb\x13");
}

TESTING_MAIN()
