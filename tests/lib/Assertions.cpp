#include <Assertions.inl>

namespace gerryfudd::test {
  void assert_equal_strings(const char *actual, const char *expected) {
    assert_equal(std::string(actual), std::string(expected));
  }
  void assert_false(bool value, const char* failure_message) {
    if (value) {
      throw AssertionFailure(failure_message);
    }
  }
  void assert_false(bool value) {
    assert_false(value, "Value was expected to be false.");
  }
  void assert_true(bool value, const char * failure_message) {
    if (value != true) {
      throw AssertionFailure(failure_message);
    }
  }
  void assert_true(bool value) {
    assert_true(value, "Value was expected to be true.");
  }
}
