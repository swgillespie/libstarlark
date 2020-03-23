#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "gc.h"
#include "util.h"

const size_t default_segment_size = 1 << 16; // 64k

typedef struct thread_heap_segment
{
  struct thread_heap_segment* next;
  ptrdiff_t size;
  uint8_t data[];
} thread_heap_segment;

struct thread_heap
{
  thread_heap_segment* fromspace;
  uint8_t* free;
  uint8_t* top;
};

static uint8_t*
align_up(uint8_t* ptr)
{
  return (uint8_t*)(((size_t)ptr + 7) & ~7);
}

static bool
aligned(uint8_t* ptr)
{
  return align_up(ptr) == ptr;
}

static object*
alloc_object(thread_heap* heap, object_vtbl* vtbl)
{
  INVARIANT(heap != NULL);
  INVARIANT(vtbl != NULL);
  INVARIANT(aligned(heap->free));
  uint8_t* result_ptr = heap->free;
  uint8_t* bump = align_up(result_ptr + vtbl->size);
  if (bump > heap->top) {
    INVARIANT(false && "NYI: collect garbage or expand heap");
  }

  heap->free = bump;
  memset(result_ptr, 0, vtbl->size);
  object* obj = (object*)result_ptr;
  obj->vtbl = vtbl;
  obj->frozen = false;
  return obj;
}

object_module*
starlark_gc_alloc_module(starlark_thread_t* thread)
{
  return (object_module*)alloc_object(thread->heap, &module_vtbl);
}

object_function*
starlark_gc_alloc_function(starlark_thread_t* thread)
{
  return (object_function*)alloc_object(thread->heap, &function_vtbl);
}

thread_heap*
starlark_gc_thread_heap_new(void)
{
  thread_heap_segment* default_segment = malloc(default_segment_size);
  default_segment->size =
    default_segment_size - sizeof(thread_heap_segment) - sizeof(ptrdiff_t);
  memset(default_segment->data, 0, default_segment->size);
  thread_heap* heap = malloc(sizeof(thread_heap));
  heap->fromspace = default_segment;
  heap->free = default_segment->data;
  heap->top = default_segment->data + default_segment->size;
  return heap;
}