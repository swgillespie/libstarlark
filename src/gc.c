#include <stddef.h>
#include <stdint.h>

#include "gc.h"

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
