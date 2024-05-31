#include <Aggregator.hpp>
#include <exception>
#include <iostream>
#include <sstream>

namespace gerryfudd::test {
  AggregationException::AggregationException(const char* message): message{message} {}
  const char* AggregationException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message;
  }

  Aggregator::Aggregator(): testCount{0} {
    tests = new Test[MAX_TEST_COUNT];
  }

  Aggregator * Aggregator::singleton = new Aggregator();

  void Aggregator::add(Test t) {
    if ((*Aggregator::singleton).testCount >= MAX_TEST_COUNT) {
      throw AggregationException("The test count overflowed the aggregator's capacity.");
    }
    (*Aggregator::singleton).tests[(*Aggregator::singleton).testCount++] = t;
  }

  bool update_name(char * name, const char * source) {
    bool changed = false;
    int i = 0;
    while (*(source + i) != '\0') {
      changed = changed || *(name + i) != *(source + i);
      *(name + i) = *(source + i);
      i++;
    }
    changed = changed || *(name + i) != '\0';
    *(name + i) = '\0';
    return changed;
  }

  int Aggregator::run_all() {
    int failureCount = 0;
    std::stringstream failure, info_buff, failure_buff;
    char * current_file = new char[NAME_BUFFFER];
    current_file[0] = '\0';
    Test current_test;

    for (int ordinal = 0; ordinal < (*Aggregator::singleton).testCount; ordinal++) {
      info_buff.str(std::string());
      failure_buff.str(std::string());
      current_test = (*Aggregator::singleton).tests[ordinal];
      if (update_name(current_file, current_test.get_filename())) {
        std::cout << std::endl << "Test file: " << current_file << std::endl << std::endl;
      }

      if (current_test.run(ordinal + 1, info_buff, failure_buff)) {
        failureCount++;
        failure << std::endl << info_buff.str() << std::endl << failure_buff.str();
      }
      std::cout << info_buff.str() << std::endl;
    }
    delete[] current_file;

    if (failureCount > 0) {
      std::cerr << "Test failures" << std::endl;
      std::cerr << failure.str() << std::endl;
    }
    return failureCount;
  }
}