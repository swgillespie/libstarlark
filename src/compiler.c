#include "compiler.h"
#include "gc.h"
#include "vm.h"

struct compiler
{
  starlark_thread_t* thread;
  object_module* module;
  object_function* func;
  const char* source;
};

static object_function*
compile(compiler* compiler)
{
  bytecode* code = starlark_bytecode_new();
  starlark_bytecode_append(code, OP_RETURN);
  compiler->func = starlark_gc_alloc_function(compiler->thread);
  compiler->func->code = code;
  return compiler->func;
}

void
starlark_compiler_trace(compiler* compiler, void (*fn)(object**))
{
  fn(AS_OBJ_REF(compiler->module));
  if (compiler->func != NULL) {
    fn(AS_OBJ_REF(compiler->func));
  }
}

object_function*
starlark_compile(starlark_thread_t* thread,
                 object_module* module,
                 const char* source)
{
  thread->compiler = malloc(sizeof(compiler));
  INVARIANT(thread->compiler != NULL);
  thread->compiler->thread = thread;
  thread->compiler->module = module;
  thread->compiler->source = source;
  object_function* fn = compile(thread->compiler);
  free(thread->compiler);
  thread->compiler = NULL;
  return fn;
}