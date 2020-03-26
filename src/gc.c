#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "gc.h"
#include "util.h"

#define MAX_PROTECTED_POINTERS (1 << 8)
#define DEFAULT_SEGMENT_SIZE (1 << 16)

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
  size_t protected_pointers_count;
  object** protected_pointers[MAX_PROTECTED_POINTERS];
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

object*
starlark_gc_alloc_object(starlark_thread_t* thread, object_vtbl* vtbl)
{
  INVARIANT(thread != NULL);
  thread_heap* heap = thread->heap;

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

object_array*
starlark_gc_alloc_array(starlark_thread_t* thread, size_t num_elements)
{
  INVARIANT(false && "NYI");
  UNUSED_PARAMETER(thread);
  UNUSED_PARAMETER(num_elements);
  return NULL;
}

void
starlark_gc_protect_push(starlark_thread_t* thread, object** ptr)
{
  INVARIANT(thread->heap->protected_pointers_count < MAX_PROTECTED_POINTERS);
  thread->heap->protected_pointers[thread->heap->protected_pointers_count++] =
    ptr;
}

void
starlark_gc_protect_pop(starlark_thread_t* thread)
{
  INVARIANT(thread->heap->protected_pointers_count > 0);
  // Not strictly necessary, useful for debugging.
  thread->heap->protected_pointers[thread->heap->protected_pointers_count] =
    NULL;
  thread->heap->protected_pointers_count--;
}

thread_heap*
starlark_gc_thread_heap_new(void)
{
  thread_heap_segment* default_segment = malloc(DEFAULT_SEGMENT_SIZE);
  default_segment->size =
    DEFAULT_SEGMENT_SIZE - sizeof(thread_heap_segment) - sizeof(ptrdiff_t);
  memset(default_segment->data, 0, default_segment->size);
  thread_heap* heap = malloc(sizeof(thread_heap));
  heap->fromspace = default_segment;
  heap->free = default_segment->data;
  heap->top = default_segment->data + default_segment->size;
  heap->protected_pointers_count = 0;
  memset(
    heap->protected_pointers, 0, MAX_PROTECTED_POINTERS * sizeof(object**));
  return heap;
}

void
starlark_gc_thread_heap_free(thread_heap* heap)
{
  free(heap->fromspace);
  // TODO(swgillespie) need to be consistent about pointer ownership in _free
  // methods
  free(heap);
}