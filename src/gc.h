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

object_module*
starlark_gc_alloc_module(starlark_thread_t* thread);

object_function*
starlark_gc_alloc_function(starlark_thread_t* thread);

#endif // __STARLARK_GC_H__