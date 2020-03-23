#ifndef __STARLARK_VM_H__
#define __STARLARK_VM_H__

#include "compiler.h"

struct starlark_vm_t
{};

struct starlark_thread_t
{
  starlark_vm_t* vm;
  compiler* compiler;
  struct thread_heap* heap;
};

#endif // __STARLARK_VM_H__