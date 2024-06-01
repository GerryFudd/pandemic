#ifndef AGGREGATOR_TYPE
#define AGGREGATOR_TYPE

#include <Test.hpp>
#include <exception>
#include <vector>

namespace gerryfudd::test {
  class AggregationException: public std::exception {
    std::string message;
  public:
    AggregationException(const char *);
    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
  };

  class Aggregator {
    static std::vector<Test> tests;
  public:
    static void add(Test);
    static int run_all();
  };
}
#endif
