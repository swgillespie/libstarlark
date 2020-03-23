#ifndef __STARLARK_VALUE_H__
#define __STARLARK_VALUE_H__

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#include "bytecode.h"
#include "util.h"

typedef uint64_t value;

typedef struct object
{
  struct object_vtbl* vtbl;
  atomic_bool frozen;
} object;

typedef void (*destructor)(object*);

typedef struct object_vtbl
{
  size_t size;
  bool finalizable;
  destructor dtor;
} object_vtbl;

typedef struct object_module
{
  object root;
} object_module;

typedef struct object_function
{
  object root;
  bytecode* code;
} object_function;

extern object_vtbl module_vtbl;
extern object_vtbl function_vtbl;

static inline size_t
starlark_object_size(object* ptr)
{
  INVARIANT(ptr != NULL);
  return ptr->vtbl->size;
}

static inline bool
starlark_object_is_finalizable(object* ptr)
{
  INVARIANT(ptr != NULL);
  return ptr->vtbl->finalizable;
}

static inline void
starlark_object_finalize(object* ptr)
{
  INVARIANT(ptr != NULL);
  INVARIANT(starlark_object_is_finalizable(ptr));
  ptr->vtbl->dtor(ptr);
}

#endif // __STARLARK_VALUE_H__