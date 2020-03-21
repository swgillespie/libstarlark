#ifndef __STARLARK_H__
#define __STARLARK_H__

typedef struct starlark_vm_t starlark_vm_t;

typedef struct starlark_thread_t starlark_thread_t;

typedef enum starlark_result_t
{
  STARLARK_RESULT_SUCCESS,
  STARLARK_RESULT_COMPILE_ERROR,
  STARLARK_RESULT_RUNTIME_ERROR
} starlark_result_t;

starlark_vm_t*
starlark_vm_new(void);

void
starlark_vm_free(starlark_vm_t* vm);

starlark_thread_t*
starlark_thread_new(starlark_vm_t* vm);

void
starlark_thread_free(starlark_thread_t* ctx);

starlark_result_t
starlark_thread_eval(starlark_thread_t* ctx,
                     const char* module,
                     const char* source);

#endif // __STARLARK_H__