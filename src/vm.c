#include <stdbool.h>
#include <stdlib.h>

#include "bytecode.h"
#include "compiler.h"
#include "gc.h"
#include "starlark.h"
#include "util.h"
#include "value.h"
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
  ctx->heap = starlark_gc_thread_heap_new();
  return ctx;
}

void
starlark_thread_free(starlark_thread_t* thread)
{
  starlark_gc_thread_heap_free(thread->heap);
  free(thread);
}

starlark_result_t
starlark_thread_eval(starlark_thread_t* thread,
                     const char* module,
                     const char* source)
{
  UNUSED_PARAMETER(module);
  object_function* f = starlark_compile(thread, NULL, source);
  starlark_bytecode_disassemble(stderr, &f->code);
  return STARLARK_RESULT_SUCCESS;
}