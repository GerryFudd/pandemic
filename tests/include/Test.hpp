#ifndef TEST_TYPE
#define TEST_TYPE
#define STACK_TRACE_CAPACITY 20
#include <iostream>
#include <exception>

namespace gerryfudd {
    namespace test {
        class AssertionFailure: public std::exception {
                const char* message;
                char* trace;
            public:
                AssertionFailure(std::string);
                AssertionFailure(const char*);
                const char* what() const  _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;
                friend std::ostream& operator<<(std::ostream&, const AssertionFailure&);
        };

        class Test {
            const char * filename;
            int line;
            const char * name;
            void (*exec)();
        public:
            Test();
            Test(const char*, int, const char *, void (*exec)());
            bool run(unsigned short,std::ostream&,std::ostream&);
            const char * get_filename(void);
            int get_line(void);
        };

        void print_stack_trace(std::ostream&);
    }
}
#endif
