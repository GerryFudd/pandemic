#include <Framework.hpp>

namespace gerryfudd::test {
  cheater_registrar::cheater_registrar(Test t) {
    Aggregator::add(t);
  };
}
