#include "value.h"
#include "bytecode.h"
#include "gc.h"
#include "util.h"

static void
default_dtor(object* ptr)
{
  UNUSED_PARAMETER(ptr);
}

static void
default_trace_pointers(object* ptr, void (*fn)(object**))
{
  UNUSED_PARAMETER(ptr);
  UNUSED_PARAMETER(fn);
}

// =============================================================================
// Module
// =============================================================================

object_module*
starlark_module_new(starlark_thread_t* thread)
{
  object_module* mod =
    (object_module*)starlark_gc_alloc_object(thread, &module_vtbl);
  return mod;
}

object_vtbl module_vtbl = {
  .size = sizeof(object_module),
  .finalizable = false,
  .contains_pointers = false,
  .dtor = default_dtor,
  .trace_pointers = default_trace_pointers,
};

// =============================================================================
// Function
// =============================================================================

static void
function_dtor(object_function* ptr)
{
  starlark_bytecode_free(&ptr->code);
}

static void
function_trace_pointers(object_function* ptr, void (*fn)(object**))
{
  UNUSED_PARAMETER(ptr);
  UNUSED_PARAMETER(fn);
}

object_function*
starlark_function_new(starlark_thread_t* thread, bytecode code)
{
  object_function* func =
    (object_function*)starlark_gc_alloc_object(thread, &function_vtbl);
  func->code = code;
  return func;
}

object_vtbl function_vtbl = {
  .size = sizeof(object_function),
  .finalizable = true,
  .dtor = (destructor)function_dtor,
  .contains_pointers = false,
  .trace_pointers = (tracefn)function_trace_pointers,
};

// =============================================================================
// String
// =============================================================================

static void
string_dtor(object_string* ptr)
{
  free(ptr->contents);
}

object_vtbl string_vtbl = {
  .size = sizeof(object_string),
  .finalizable = true,
  .dtor = (destructor)string_dtor,
  .contains_pointers = false,
  .trace_pointers = default_trace_pointers,
};

// =============================================================================
// Array
// =============================================================================

static void
array_trace_pointers(object_array* ptr, void (*fn)(object**))
{
  for (size_t i = 0; i < ptr->length; i++) {
    if (starlark_value_is_object(ptr->data[i])) {
      fn(AS_OBJ_REF(&ptr->data[i]));
    }
  }
}

object_vtbl array_vtbl = {
  .size = sizeof(object_array),
  .finalizable = false,
  .dtor = default_dtor,
  .contains_pointers = true,
  .trace_pointers = (tracefn)array_trace_pointers,
};

// =============================================================================
// List
// =============================================================================

static void
list_trace_pointers(object_list* ptr, void (*fn)(object**))
{
  fn(AS_OBJ_REF(&ptr->data));
}

object_vtbl list_vtbl = {
  .size = sizeof(object_list),
  .finalizable = false,
  .dtor = default_dtor,
  .contains_pointers = true,
  .trace_pointers = (tracefn)list_trace_pointers,
};

// =============================================================================
// Tuple
// =============================================================================

static void
tuple_trace_pointers(object_tuple* ptr, void (*fn)(object**))
{
  fn(AS_OBJ_REF(&ptr->data));
}

object_vtbl tuple_vtbl = {
  .size = sizeof(object_tuple),
  .finalizable = false,
  .dtor = default_dtor,
  .contains_pointers = true,
  .trace_pointers = (tracefn)tuple_trace_pointers,
};

// =============================================================================
// Dict
// =============================================================================

static void
dict_trace_pointers(object_dict* ptr, void (*fn)(object**))
{
  fn(AS_OBJ_REF(&ptr->data));
  if (ptr->most_recently_added != NULL) {
    fn(AS_OBJ_REF(&ptr->most_recently_added));
  }
}

object_vtbl dict_vtbl = {
  .size = sizeof(object_dict),
  .finalizable = false,
  .dtor = default_dtor,
  .contains_pointers = true,
  .trace_pointers = (tracefn)dict_trace_pointers,
};

/*
void
starlark_dict_insert(object_dict* dict, value key, value value)
{
  // Entries in the Starlark dictionary are 4-tuples:
  //   1) The key, in its unhashed form
  //   2) The value
  //   3) A linked list entry pointing to the next tuple in the insertion order
  //
  // TODO(swgillespie) just a sketch, this is not going to work
  uint64_t key_hash = starlark_value_hash(key);
  size_t bucket_id = key_hash % dict->data->length;
  object_tuple* entry_tuple = (object_tuple*)NULL; // TODO gc.h
}
*/