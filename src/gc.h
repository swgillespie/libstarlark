#ifndef __STARLARK_GC_H__
#define __STARLARK_GC_H__

#include "value.h"
#include "vm.h"

typedef struct thread_heap thread_heap;
typedef struct global_heap global_heap;

thread_heap*
starlark_gc_thread_heap_new(void);

void
starlark_gc_thread_heap_free(thread_heap* heap);

object_array*
starlark_gc_alloc_array(starlark_thread_t* thread, size_t num_elements);

object*
starlark_gc_alloc_object(starlark_thread_t* thread, object_vtbl* vtbl);

// very low level - should these be gc.c internal?

void
starlark_gc_protect_push(starlark_thread_t* thread, object** ptr_ref);

void
starlark_gc_protect_pop(starlark_thread_t* thread);

#endif // __STARLARK_GC_H__