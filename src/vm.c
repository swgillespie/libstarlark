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
  ctx->slots = NULL;
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

int
starlark_thread_get_slot_count(starlark_thread_t* thread)
{
  if (thread->slots == NULL) {
    return 0;
  }

  return thread->slots->length;
}

static void
validate_api_slots(starlark_thread_t* thread, starlark_slot_t slot)
{
  INVARIANT(slot >= 0);
  INVARIANT(slot < starlark_thread_get_slot_count(thread));
  INVARIANT(thread->slots != NULL);
}

void
starlark_thread_ensure_slots(starlark_thread_t* thread, int desired_slots)
{
  INVARIANT(desired_slots > 0);
  if (thread->slots == NULL || thread->slots->length < (size_t)desired_slots) {
    thread->slots = starlark_gc_alloc_array(thread, desired_slots);
  }
}

starlark_type_t
starlark_thread_get_slot_type(starlark_thread_t* thread, starlark_slot_t slot)
{
  validate_api_slots(thread, slot);
  value v = thread->slots->data[slot];
  if (starlark_value_is_int(v)) {
    return STARLARK_TYPE_INT;
  }

  // TODO(swgillespie) other types
  return STARLARK_TYPE_INT;
}

void
starlark_thread_get_variable(starlark_thread_t* thread,
                             const char* module,
                             const char* name,
                             starlark_slot_t slot)
{
  UNUSED_PARAMETER(thread);
  UNUSED_PARAMETER(module);
  UNUSED_PARAMETER(name);
  UNUSED_PARAMETER(slot);
}