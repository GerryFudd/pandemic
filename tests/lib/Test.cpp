#include <Test.hpp>
#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <execinfo.h>
#include <cxxabi.h>
#include <sstream>

namespace gerryfudd::test {
  const char *as_char_pointer(std::string str_text) {
    char *text = new char[str_text.length() + 1];
    text[str_text.length()] = '\0';
    for (int i = 0; i < str_text.length(); i++) {
      text[i] = str_text[i];
    }
    return text;
  }
  AssertionFailure::AssertionFailure(std::string str_message): AssertionFailure::AssertionFailure(as_char_pointer(str_message)) {}
  AssertionFailure::AssertionFailure(const char* message): message{message} {
    std::stringstream trace_captor;
    unw_cursor_t cursor;
    unw_context_t context;

    // Initialize cursor to current frame for local unwinding.
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    // Unwind frames one by one, going up the frame stack.
    while (unw_step(&cursor) > 0) {
      unw_word_t offset, pc;
      unw_get_reg(&cursor, UNW_REG_IP, &pc);
      if (pc == 0) {
        break;
      }
      trace_captor.str("");

      char sym[256];
      if (unw_get_proc_name(&cursor, sym, sizeof(sym), &offset) == 0) {
        char* print_me = sym;
        int status;
        char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
        if (status == 0) {
          print_me = demangled;
        }
        trace_captor << print_me;
      } else {
        trace_captor << "-- error: unable to obtain symbol name for this frame";
      }
      trace.push_back(trace_captor.str());
    }
  }
  const char* AssertionFailure::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW {
    return message.c_str();
  }

  std::ostream& operator<<(std::ostream& out, const AssertionFailure &e) {
    out << e.message << std::endl;
    for (std::vector<std::string>::const_iterator trace_line = e.trace.begin(); trace_line != e.trace.end(); trace_line++) {
      out << "    " << *trace_line << std::endl;
    }
    return out;
  }

  Test::Test(const char * filename, int line, const char * name, void (*exec)()): filename{filename}, line{line}, name{name}, exec{exec} {}
  void null_test() {}
  Test::Test(): Test("", -1, "", &null_test) {}

  bool Test::run(unsigned short ordinal,std::ostream& out,std::ostream& err) {
    out << ordinal << ". " << name;
    try {
      exec();
      out << " PASSED.";
      return false;
    } catch(AssertionFailure e) {
      err << e;
    }
    out << " (" << filename << ":" << line << ") FAILED.";
    return true;
  }

  std::string Test::get_filename() {
    return filename;
  }

  int Test::get_line() {
    return line;
  }
}