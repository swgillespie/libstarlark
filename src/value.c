#include "value.h"
#include "bytecode.h"
#include "util.h"

static void
default_dtor(object* ptr)
{
  UNUSED_PARAMETER(ptr);
}

object_vtbl module_vtbl = { .size = sizeof(object_module),
                            .finalizable = false,
                            .dtor = default_dtor };

static void
function_dtor(object_function* ptr)
{
  starlark_bytecode_free(ptr->code);
}

object_vtbl function_vtbl = { .size = sizeof(object_function),
                              .finalizable = true,
                              .dtor = (destructor)function_dtor };