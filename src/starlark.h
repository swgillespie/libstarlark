#ifndef __STARLARK_H__
#define __STARLARK_H__

typedef struct starlark_vm_t starlark_vm_t;

typedef struct starlark_thread_t starlark_thread_t;

typedef int starlark_slot_t;

typedef enum starlark_result_t
{
  STARLARK_RESULT_SUCCESS,
  STARLARK_RESULT_COMPILE_ERROR,
  STARLARK_RESULT_RUNTIME_ERROR
} starlark_result_t;

typedef enum starlark_type_t
{
  STARLARK_TYPE_BOOL,
  STARLARK_TYPE_INT,
} starlark_type_t;

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

int
starlark_thread_get_slot_count(starlark_thread_t* thread);

void
starlark_thread_ensure_slots(starlark_thread_t* thread, int desired_slots);

starlark_type_t
starlark_thread_get_slot_type(starlark_thread_t* thread, starlark_slot_t slot);

int
starlark_thread_get_slot_int(starlark_thread_t* thread, starlark_slot_t slot);

void
starlark_thread_get_variable(starlark_thread_t* thread,
                             const char* module,
                             const char* name,
                             starlark_slot_t slot);

#endif // __STARLARK_H__