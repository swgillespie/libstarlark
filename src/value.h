#ifndef __STARLARK_VALUE_H__
#define __STARLARK_VALUE_H__

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#include "bytecode.h"
#include "util.h"
#include "vm.h"

typedef uint_fast64_t value;

static inline bool
starlark_value_is_int(value v)
{
  return (v & 7) == 1;
}

static inline bool
starlark_value_is_object(value v)
{
  return (v & 7) == 0;
}

static inline uint64_t
starlark_value_hash(value v)
{
  UNUSED_PARAMETER(v);
  return 0;
}

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

typedef struct object_function
{
  object root;
  bytecode code;
} object_function;

object_function*
starlark_function_new(starlark_thread_t* thread, bytecode code);

typedef struct object_string
{
  object root;
  size_t length;
  char* contents;
} object_string;

typedef struct object_array
{
  object root;
  size_t length;
  value data[];
} object_array;

typedef struct object_list
{
  object root;
  size_t count;
  object_array* data;
} object_list;

value
starlark_list_get_element(object_list* list, int index);

typedef struct object_tuple
{
  object root;
  size_t count;
  object_array* data;
} object_tuple;

typedef struct object_dict
{
  object root;
  size_t count;
  object_array* data;
  object_tuple* most_recently_added;
} object_dict;

void
starlark_dict_insert(object_dict* dict, value key, value value);

typedef struct object_module
{
  object root;
  object_string* export_name;
  value export_value;
} object_module;

object_module*
starlark_module_new(starlark_thread_t* thread);

object_module
starlark_module_add_export(object_string* name, value value);

extern object_vtbl module_vtbl;
extern object_vtbl function_vtbl;
extern object_vtbl string_vtbl;
extern object_vtbl array_vtbl;
extern object_vtbl list_vtbl;
extern object_vtbl tuple_vtbl;
extern object_vtbl dict_vtbl;

static inline size_t
starlark_object_size(object* ptr)
{
  INVARIANT(ptr != NULL);
  if (ptr->vtbl == &array_vtbl) {
    object_array* arr = (object_array*)ptr;
    return arr->length;
  }

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
#define AS_OBJ_REF(ptr) ((object**)(ptr))

#endif // __STARLARK_VALUE_H__