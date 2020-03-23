#ifndef __STARLARK_COMPILER_H__
#define __STARLARK_COMPILER_H__

#include "starlark.h"
#include "value.h"

typedef struct compiler compiler;

object_function*
starlark_compile(starlark_thread_t* thread,
                 object_module* module,
                 const char* source);

void
starlark_compiler_trace(compiler* compiler, void (*fn)(object**));

#endif // __STARLARK_COMPILER_H__