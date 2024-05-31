#ifndef FRAMEWORK_DEFS
#define FRAMEWORK_DEFS
#include <Aggregator.hpp>

namespace gerryfudd {
    namespace test {
        struct cheater_registrar {
            cheater_registrar(Test);
        };
    }
}

#define TEST(name) \
void name(); \
Test name ## _test(__FILE__, __LINE__, #name, &name); \
cheater_registrar name ## _registered (name ## _test); \
void name()

#endif