# simple_test
simple unittest

## Usage

- add `simple_test.h` to your project
- include
- write set of `TEST(name) {body}` functions
- add `TESTING_MAIN()`
- voila

## Macros

### TEST
```
TEST(name, [enabled]) {
  test body goes here;
}
```
- `name` is valid C identifier (not decorated)
- `enabled` is optional bool expression (runtime constant, evaluated before main())

introduces auxillary object test_name and function
```
void func_name() {
  test body goes here;
}
```
which shall throw an exception if some test assertion fails.

Please don't catch(...) inside it without special need.

### ASSERT_..., EXPECT_...
```
ASSERT_CMP(a, op, b)
EXPECT_CMP(a, op, b)

ASSERT_STRCMP(a, op, b)
EXPECT_STRCMP(a, op, b)

ASSERT_BOOL(a, b)
EXPECT_BOOL(a, b)
```
- `a`, `b` - arbitrary expressions
- `op` - arbitrary comparison operator (except `<=>`)

- `_CMP` compares a and b directly, `a op b`, so they should be comparable
- `_STRCMP` uses C string comparison `strcmp(a, b) op 0`
- `_BOOL` matches boolean `(bool)a == (bool)b`

So, GTest's `ASSERT_TRUE(a)` is our `ASSERT_BOOL(a, true)` or `ASSERT_BOOL(true, a)`

If a comparison failed, compared values are printed `std::cout << a`.
So, they should be printable.

### TESTING_MAIN
Just implementation of `int main()`

Feel free to call a subsequent function from your `main()` with your fixtures:
```
int main(int argc, char** argv) {
  ...
  int result = simple_test::testing_main(argc, argv);
  ...
  return result;
}
```

### TODO:
- inaccurate comparison of float point
- test throwing / nothrowing exceptions
- printing nonprintable values using some stubs
- speed up compiling, less lambdas.
