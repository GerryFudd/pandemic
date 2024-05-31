#ifndef AGGREGATOR_TYPE
#define AGGREGATOR_TYPE
#define MAX_TEST_COUNT 256
#define NAME_BUFFFER 256

#include <Test.hpp>
#include <exception>

namespace gerryfudd::test {
  class AggregationException: public std::exception {
    const char* message;
  public:
    AggregationException(const char *);
    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
  };

  class Aggregator {
    static Aggregator* singleton;
    Test* tests;
    int testCount;
  public:
    Aggregator();
    static void add(Test);
    static int run_all();
  };
}
#endif
