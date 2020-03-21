#include <stdlib.h>

#include "starlark.h"
#include "util.h"
#include "vm.h"

starlark_vm_t*
starlark_vm_new()
{
  return malloc(sizeof(starlark_vm_t));
}

void
starlark_vm_free(starlark_vm_t* vm)
{
  free(vm);
}

starlark_thread_t*
starlark_thread_new(starlark_vm_t* vm)
{
  starlark_thread_t* ctx = malloc(sizeof(starlark_thread_t));
  if (ctx == NULL) {
    return NULL;
  }

  ctx->vm = vm;
  return ctx;
}

void
starlark_thread_free(starlark_thread_t* thread)
{
  free(thread);
}

starlark_result_t
starlark_thread_eval(starlark_thread_t* thread,
                     const char* module,
                     const char* source)
{
  UNUSED_PARAMETER(thread);
  UNUSED_PARAMETER(module);
  UNUSED_PARAMETER(source);
  return STARLARK_RESULT_SUCCESS;
}