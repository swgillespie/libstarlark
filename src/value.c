#include "value.h"
#include "bytecode.h"
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

object_vtbl module_vtbl = {
  .size = sizeof(object_module),
  .finalizable = false,
  .contains_pointers = false,
  .dtor = default_dtor,
  .trace_pointers = default_trace_pointers,
};

static void
function_dtor(object_function* ptr)
{
  starlark_bytecode_free(ptr->code);
}

static void
function_trace_pointers(object_function* ptr, void (*fn)(object**))
{
  UNUSED_PARAMETER(ptr);
  UNUSED_PARAMETER(fn);
}

object_vtbl function_vtbl = {
  .size = sizeof(object_function),
  .finalizable = true,
  .dtor = (destructor)function_dtor,
  .contains_pointers = false,
  .trace_pointers = (tracefn)function_trace_pointers,
};

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