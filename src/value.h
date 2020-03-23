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
typedef void (*tracefn)(object*, void (*)(object**));

typedef struct object_vtbl
{
  size_t size;
  bool finalizable;
  bool contains_pointers;
  destructor dtor;
  tracefn trace_pointers;
} object_vtbl;

typedef struct object_module
{
  object root;
} object_module;

typedef struct object_function
{
  object root;
  bytecode code;
} object_function;

typedef struct object_string
{
  object root;
  size_t length;
  char* contents;
} object_string;

extern object_vtbl module_vtbl;
extern object_vtbl function_vtbl;
extern object_vtbl string_vtbl;

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

#define AS_OBJ(ptr) ((object*)(ptr))
#define AS_OBJ_REF(ptr) ((object**)(&ptr))

#endif // __STARLARK_VALUE_H__