#include "compiler.h"
#include "gc.h"
#include "vm.h"

struct compiler
{
  starlark_thread_t* thread;
  object_module* module;
  object_function* func;
  bytecode_builder builder;
  const char* source;
};

static object_function*
compile(compiler* compiler)
{
  starlark_bytecode_builder_new(&compiler->builder);
  starlark_bytecode_builder_append(&compiler->builder, OP_RETURN);
  compiler->func = starlark_gc_alloc_function(compiler->thread);
  starlark_bytecode_new(&compiler->builder, &compiler->func->code);
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