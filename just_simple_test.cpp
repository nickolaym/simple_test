#include "simple_test.h"
#include <cassert>

TEST(lots_of_failed_expectations) {
  EXPECT_CMP(123, ==, 456);
  EXPECT_BOOL(true, false);
  EXPECT_STRCMP("hello", >, "world");
  EXPECT_STRCMP("hello\0one", !=, "hello\0two");
  std::cout << "some went wrong (or not...)" << std::endl;
}

TEST(some_assertion_failed) {
  ASSERT_CMP(123, ==, 456);
  assert(false);  // unreachable
}

TEST(raised_exception) {
  throw std::runtime_error("ooo");
  assert(false);  // unreachable
}

TEST(some_disabled, false) {
  assert(false);  // unreachable
}

TEST(some_fault_1) {
  ASSERTION_FAULT();
  assert(false);  // unreachable
}

TEST(some_fault_2) {
  ASSERTION_FAULT("comment ", 123, " goes here");
  assert(false);  // unreachable
}

TEST(some_correct) {
  const char h1[] = "hello";
  const char h2[] = "hello";
  EXPECT_CMP(h1, !=, h2);  // different addresses
  EXPECT_STRCMP(h1, ==, h2);  // equal content

  EXPECT_STRCMP("hello", <, "world");

  EXPECT_STRCMP("hello\0one", ==, "hello\0two");  // difference beyond trailing zero

  std::cout << "all right!" << std::endl;
}

TEST(do_not_eval_twice) {
  int counter = 0;
  ASSERT_CMP(counter++, ==, 0);
  ASSERT_CMP(++counter, ==, 2);
  ASSERT_CMP(2, ==, counter++);
  ASSERT_CMP(4, ==, ++counter);
}

struct D {
  static const int TRASH = 123456789;
  static int g_x;
  explicit D(int x) { g_x = x; }
  D(const D&) = delete;
  void operator=(const D&) = delete;
  ~D() { g_x = TRASH; }
  const int& cref() const { return g_x; }
};
int D::g_x = D::TRASH;

TEST(do_not_make_dangling_references) {
  EXPECT_CMP(D(123).cref(), ==, 123);

  const int& d = D(456).cref();
  EXPECT_CMP(d, !=, 456);
}

TEST(compare_floats) {
  EXPECT_CMP(123.456, ==, simple_test::nearly_abs(123.4, 0.1));
  EXPECT_CMP(123.456, ==, simple_test::nearly_abs(123.5, 0.1));
  EXPECT_CMP(123.456, <, simple_test::nearly_abs(123.6, 0.1));
  EXPECT_CMP(123.456, >, simple_test::nearly_abs(123.3, 0.1));
  EXPECT_CMP(123.456, <=, simple_test::nearly_abs(123.4, 0.1));
  EXPECT_CMP(123.456, >=, simple_test::nearly_abs(123.5, 0.1));
}

TESTING_MAIN()
