#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

noreturn void
starlark_invariant_violation(const char* file, int line, const char* expr)
{
  void* backtrace_buf[255];
  fprintf(stderr, "invariant violation (%s:%d): %s\n", file, line, expr);
  int num_symbols = backtrace(backtrace_buf, 255);
  backtrace_symbols_fd(backtrace_buf, num_symbols, 2);
  fflush(stderr);
  abort();
}
