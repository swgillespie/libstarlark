#include "compiler.h"
#include "value.h"
#include "vm.h"

// https://github.com/bazelbuild/starlark/blob/master/spec.md#lexical-elements
// https://docs.python.org/3/reference/lexical_analysis.html
typedef enum token_type
{
  TOKEN_IDENTIFIER,
  TOKEN_NUMBER,
  TOKEN_EQUALS,
  TOKEN_EOF
} token_type;

typedef struct token
{
  token_type type;
  int length;
  int line;
} token;

typedef struct parser
{
  const char* source;
} parser;

struct compiler
{
  starlark_thread_t* thread;
  object_module* module;
  object_function* func;
  bytecode_builder builder;
};

static object_function*
compile(compiler* compiler)
{
  starlark_bytecode_builder_new(&compiler->builder);
  starlark_bytecode_builder_append(&compiler->builder, OP_RETURN);
  bytecode code;
  starlark_bytecode_new(&compiler->builder, &code);
  compiler->func = starlark_function_new(compiler->thread, code);
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
  UNUSED_PARAMETER(source);
  thread->compiler = malloc(sizeof(compiler));
  INVARIANT(thread->compiler != NULL);
  thread->compiler->thread = thread;
  thread->compiler->module = module;
  object_function* fn = compile(thread->compiler);
  free(thread->compiler);
  thread->compiler = NULL;
  return fn;
}