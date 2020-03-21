#include <stdlib.h>

#include "starlark.h"
#include "util.h"
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