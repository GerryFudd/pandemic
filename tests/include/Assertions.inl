#ifndef ASSERTIONS_LIB
#define ASSERTIONS_LIB
#include <Test.hpp>
#include <sstream>

namespace gerryfudd {
  namespace test {
    template <class commonType>
    void assert_equal(const commonType& actual, const commonType& expected)  {
      if (actual != expected) {
        std::stringstream capture_message;
        try {
          capture_message << "\"" << actual << "\" should equal \"" << expected << "\"";
        } catch(...) {
          capture_message << "Values are not equal. Exception encountered while streaming values to this error message. Consider implementing friend ostream& operator<<(ostream&,const T&)";
        }

        throw AssertionFailure(capture_message.str());
      }
    }
    void assert_equal_strings(const char *, const char *);
    void assert_false(bool, const char*);
    void assert_false(bool);
    void assert_true(bool, const char*);
    void assert_true(bool);
  }
}
#endif
