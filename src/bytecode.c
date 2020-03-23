#include <assert.h>

#include "bytecode.h"

static const char* mnemonics[] = {
#define OPCODE(num, mnemonic, stack) "OP_" #mnemonic,
#include "opcodes.h"
};

DEFINE_VECTOR(bytecode_builder, uint8_t)
DEFINE_FROZEN_VECTOR(bytecode, bytecode_builder, uint8_t)

void
starlark_bytecode_disassemble(FILE* stream, bytecode* code)
{
  for (int i = 0; i < code->count; i++) {
    fprintf(stream, "%d: %s\n", i, mnemonics[code->data[code->count]]);
  }
}