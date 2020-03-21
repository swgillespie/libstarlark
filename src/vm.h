#ifndef __STARLARK_VM_H__
#define __STARLARK_VM_H__

struct starlark_vm_t
{};

struct starlark_thread_t
{
  starlark_vm_t* vm;
};

#endif // __STARLARK_VM_H__