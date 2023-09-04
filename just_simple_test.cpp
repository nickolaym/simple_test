#include "simple_test.h"
#include <cassert>

#include <vector>

TEST(simple_test, vector_capacity) {
  std::vector<int> xs;
  xs.reserve(100500);
  xs.clear();
  EXPECT_CMP(xs.capacity(), ==, 0) << " ahaha ? ahaha!";
  ASSERT_CMP(xs.size(), ==, 1) << " ahaha ? ahaha!";
}

TEST(simple_test, lots_of_failed_expectations) {
  EXPECT_CMP(123, ==, 456);
  EXPECT_BOOL(true, false);
  EXPECT_STRCMP("hello", >, "world");
  EXPECT_STRCMP("hello\0one", !=, "hello\0two");
  std::cout << "some went wrong (or not...)" << std::endl;
}

TEST(simple_test, some_assertion_failed) {
  ASSERT_CMP(123, ==, 456);
  assert(false);  // unreachable
}

TEST(simple_test, raised_exception) {
  throw std::runtime_error("ooo");
  assert(false);  // unreachable
}

TEST(simple_test, some_disabled, false) {
  assert(false);  // unreachable
}

TEST(simple_test, some_fault_1) {
  ASSERTION_FAULT() << "comment " << 123 << " goes here";
  assert(false);  // unreachable
}

TEST(simple_test, some_fault_2) {
  ASSERTION_FAULT("comment ", 123, " goes here");
  assert(false);  // unreachable
}

TEST(simple_test, some_correct) {
  const char h1[] = "hello";
  const char h2[] = "hello";
  EXPECT_CMP(h1, !=, h2);  // different addresses
  EXPECT_STRCMP(h1, ==, h2);  // equal content

  EXPECT_STRCMP("hello", <, "world");

  EXPECT_STRCMP("hello\0one", ==, "hello\0two");  // difference beyond trailing zero

  std::cout << "all right!" << std::endl;
}

TEST(simple_test, do_not_eval_twice) {
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

TEST(simple_test, do_not_make_dangling_references) {
  EXPECT_CMP(D(123).cref(), ==, 123);

  const int& d = D(456).cref();
  EXPECT_CMP(d, !=, 456);
}

TEST(simple_test, compare_floats_explicitly) {
  EXPECT_CMP(123.456, ==, simple_test::nearly_abs(123.4, 0.1));
  EXPECT_CMP(123.456, ==, simple_test::nearly_abs(123.5, 0.1));

  EXPECT_CMP(123.456, <, simple_test::nearly_abs(123.6, 0.1));
  EXPECT_CMP(123.456, >, simple_test::nearly_abs(123.3, 0.1));

  EXPECT_CMP(123.456, <=, simple_test::nearly_abs(123.4, 0.1));
  EXPECT_CMP(123.456, >=, simple_test::nearly_abs(123.5, 0.1));
}

TEST(simple_test, compare_floats_implicitly) {
  const auto pivot = 123.4;
  const auto epsilon = 0.1;
  const auto tiny = 0.001;

  EXPECT_FLOATCMP(pivot, ==, pivot - epsilon, epsilon);
  EXPECT_FLOATCMP(pivot, ==, pivot + epsilon, epsilon);

  EXPECT_FLOATCMP(pivot, !=, pivot - epsilon - tiny, epsilon);
  EXPECT_FLOATCMP(pivot, !=, pivot + epsilon + tiny, epsilon);

  EXPECT_FLOATCMP(pivot, >, pivot - epsilon - tiny, epsilon);
  EXPECT_FLOATCMP(pivot, <, pivot + epsilon + tiny, epsilon);

  EXPECT_FLOATCMP(pivot, <=, pivot - epsilon, epsilon);
  EXPECT_FLOATCMP(pivot, >=, pivot + epsilon, epsilon);
}

TEST(gtest_like, variety) {
  EXPECT_EQ(123, 123);
  EXPECT_STREQ("aaa\0bbb", "aaa\0ccc");
  EXPECT_TRUE(1 == 1);
  EXPECT_FALSE(1 == 2);
  EXPECT_NEAR(123.4, 123.5, 0.1);
}

TESTING_MAIN()
