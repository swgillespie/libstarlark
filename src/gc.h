#ifndef __STARLARK_GC_H__
#define __STARLARK_GC_H__

#include "value.h"

typedef struct thread_heap thread_heap;
typedef struct global_heap global_heap;

thread_heap*
starlark_gc_thread_heap_new(void);

void
starlark_gc_thread_heap_destroy(thread_heap* heap);

object_module*
starlark_gc_alloc_module(thread_heap* heap);

object_function*
starlark_gc_alloc_function(thread_heap* heap);

object*
starlark_gc_freeze(global_heap* heap, thread_heap* thread_heap, object* obj);

#endif // __STARLARK_GC_H__