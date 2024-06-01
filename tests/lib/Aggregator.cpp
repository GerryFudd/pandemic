#include <Aggregator.hpp>
#include <exception>
#include <iostream>
#include <sstream>

namespace gerryfudd::test {
  AggregationException::AggregationException(const char* message): message{message} {}
  const char* AggregationException::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
  }

  std::vector<Test> Aggregator::tests;
  void Aggregator::add(Test t) {
    Aggregator::tests.push_back(t);
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
    std::string current_file;
    int ordinal = 1;

    for (std::vector<Test>::iterator current_test = tests.begin(); current_test != tests.end(); current_test++) {
      info_buff.str(std::string());
      failure_buff.str(std::string());
      if (current_file != current_test->get_filename()) {
        current_file = current_test->get_filename();
        std::cout << std::endl << "Test file: " << current_file << std::endl << std::endl;
      }
      if (current_test->run(ordinal++, info_buff, failure_buff)) {
        failureCount++;
        failure << std::endl << info_buff.str() << std::endl << failure_buff.str();
      }
      std::cout << info_buff.str() << std::endl;
    }

    if (failureCount > 0) {
      std::cerr << "Test failures" << std::endl;
      std::cerr << failure.str() << std::endl;
    } else {
      std::cout << std::endl << std::endl << "ALL TESTS PASSED" << std::endl << std::endl;
    }
    return failureCount;
  }
}