#ifndef __STARLARK_UTIL_H__
#define __STARLARK_UTIL_H__

#include <stdnoreturn.h>

#define UNUSED_PARAMETER(p) ((void)(p))

#define UNLIKELY(expr) __builtin_expect(!!(expr), 0)

#define INVARIANT(expr)                                                        \
  do {                                                                         \
    if (UNLIKELY(!(expr))) {                                                   \
      starlark_invariant_violation(__FILE__, __LINE__, #expr);                 \
      __builtin_unreachable();                                                 \
    }                                                                          \
  } while (0)

noreturn void
starlark_invariant_violation(const char* file, int line, const char* expr);

#endif // __STARLARK_UTIL_H__