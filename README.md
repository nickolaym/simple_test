# simple_test
simple unittest

## Usage

- add `simple_test.h` to your project
- include
- write set of `TEST(suite, name) {body}` functions
- add `TESTING_MAIN()`
- voila

## Macros

### TEST
```
TEST(suite, name, [enabled]) {
  test body goes here;
}
```
- `suite` is valid C identifier (not decorated)
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

ASSERT_FLOATCMP(a, op, b, eps)
EXPECT_FLOATCMP(a, op, b, eps)

ASSERT_BOOL(a, b)
EXPECT_BOOL(a, b)
```
- `a`, `b` - arbitrary expressions
- `op` - arbitrary comparison operator (except `<=>`)

- `_CMP` compares a and b directly, `a op b`, so they should be comparable
- `_STRCMP` uses C string comparison `strcmp(a, b) op 0`
- `_FLOATCMP` uses inaccurate float comparison, `(a ± eps) op b`
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
- test throwing / nothrowing exceptions
- printing nonprintable values using some stubs
