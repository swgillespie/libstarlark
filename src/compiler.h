#ifndef __STARLARK_COMPILER_H__
#define __STARLARK_COMPILER_H__

#include "starlark.h"
#include "value.h"

object_function*
starlark_compile(starlark_thread_t* thread,
                 object_module* module,
                 const char* source);

#endif // __STARLARK_COMPILER_H__