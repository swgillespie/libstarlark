#include "starlark.h"
#include <stdio.h>

int
main()
{
  starlark_vm_t* vm = starlark_vm_new();
  starlark_vm_free(vm);
}