#ifndef __STARLARK_H__
#define __STARLARK_H__

typedef struct starlark_vm_t starlark_vm_t;

typedef struct starlark_context_t starlark_context_t;

starlark_vm_t*
starlark_vm_new(void);

void
starlark_vm_free(starlark_vm_t* vm);

#endif // __STARLARK_H__