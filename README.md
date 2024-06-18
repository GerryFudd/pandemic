# Pandemic in C++

This project will implement the boardgame Pandemic in C++. The proof of concept will use text prompts/input as its user interface. This will not ever be used for commercial purposes. It is simply a concrete exercise that I am using to learn the fundamentals of C/C++.

## The repository structure

The implementation of this game exists within the `./include/` and `./lib/` directories. The tests for this code are implemented in the `./tests/` directory. The `./run_tests.sh` script compiles and executes these tests.

### Setup

This project is not complete enough to run the game at this point. The test suite exercises the code that has been implemented so far. It uses the `c++20` standard and the `libunwind` libary. I run these tests on a machine running debian 12 with the `libunwind-dev` library installed via `apt`.

### How the tests are written

I have implemented a simple testing framework in the `./tests/include/` and `./tests/lib/` directories. The `./tests/main.cpp` file contains the `main` method for the tests. It calls the static method `Aggregator::run_all()`, which runs all of the tests and prints their results. Any failing tests also print details after the list of test results. Failures print the file name and line number of the failing test, the message from the assertion failure, and a stacktrace courtesy of `libunwind`.

This framework includes a `TEST(name)` macro that creates a new test and registers it with the `Aggregator` class. These tests are designed so that they pass if they complete without throwing any exception and they fail if they throw an `AssertionFailure` exception. The `./tests/include/Assertions.inl` file includes `assert_equal`, `assert_true`, and `assert_false` methods. These each check the relevant condition and throw an `AssertionFailure` with an appropriate message.

The `assert_equal` method takes two arguments. These may be any type, but must be the same type as one another. If the type does not implement `operator!=` or `operator<<`, then this method will not behave properly. If `actual != expected`, then it will stream the arguments along with some descriptive text to a `std::stringstream` and use the resulting string to populate the assertion failure message. For example, if the following test were added to the end of `./tests/gameTests.cpp` at the time of writing this documentation, 

```c++
TEST(example) {
  assert_equal(5, 3);
}
```

then it would generate this output.

```bash
.../pandemic$ ./run_tests.sh 

Test file: ./tests/types/cardTests.cpp

1. populate_deck PASSED.
...

Test file: ./tests/gameTests.cpp

22. get_reserve PASSED.
...
69. example (./tests/gameTests.cpp:1062) FAILED.
Test failures

69. example (./tests/gameTests.cpp:1062) FAILED.
"5" should equal "3"
    gerryfudd::test::AssertionFailure::AssertionFailure(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >)
    void gerryfudd::test::assert_equal<int>(int const&, int const&)
    example()
    gerryfudd::test::Test::run(unsigned short, std::ostream&, std::ostream&)
    gerryfudd::test::Aggregator::run_all()
    main
    __libc_init_first
    __libc_start_main
    _start
```
