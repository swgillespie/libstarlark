#include <assert.h>

#include "bytecode.h"

static const size_t initial_size = 10;

static const char* mnemonics[] = {
#define OPCODE(num, mnemonic, stack) #mnemonic,
#include "opcodes.h"
};

bytecode*
starlark_bytecode_new(void)
{
  bytecode* code = malloc(sizeof(bytecode));
  if (code == NULL) {
    return NULL;
  }

  code->count = 0;
  code->capacity = 0;
  code->code = NULL;
  return code;
}

void
starlark_bytecode_free(bytecode* code)
{
  free(code->code);
  free(code);
}

void
starlark_bytecode_append(bytecode* code, uint8_t data)
{
  if (code->code == NULL) {
    code->capacity = initial_size;
    code->code = calloc(initial_size, sizeof(uint8_t));
    assert(code->code != NULL);
  }

  if (code->count >= code->capacity) {
    code->capacity = code->capacity * 2;
    code->code = realloc(code->code, code->capacity);
    assert(code->code != NULL);
  }

  assert(code->count < code->capacity);
  code->code[code->count++] = data;
}

void
starlark_bytecode_disassemble(FILE* stream, bytecode* code)
{
  for (int i = 0; i < code->count; i++) {
    fprintf(stream, "%d: %s\n", i, mnemonics[code->code[code->count]]);
  }
}