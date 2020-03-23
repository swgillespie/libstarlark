#ifndef __STARLARK_BYTECODE_H__
#define __STARLARK_BYTECODE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum opcode
{
#define OPCODE(num, mnemonic, stack_behavior) OP_##mnemonic = num
#include "opcodes.h"
} opcode;

typedef struct bytecode
{
  uint16_t count;
  uint16_t capacity;
  uint8_t* code;
} bytecode;

bytecode*
starlark_bytecode_new(void);

void
starlark_bytecode_free(bytecode* code);

void
starlark_bytecode_append(bytecode* code, uint8_t data);

void
starlark_bytecode_disassemble(FILE* stream, bytecode* code);

#endif // __STARLARK_BYTECODE_H__