#ifndef __STARLARK_BYTECODE_H__
#define __STARLARK_BYTECODE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

typedef enum opcode
{
#define OPCODE(num, mnemonic, stack_behavior) OP_##mnemonic = num
#include "opcodes.h"
} opcode;

DECLARE_VECTOR(bytecode_builder, uint8_t)
DECLARE_FROZEN_VECTOR(bytecode, bytecode_builder, uint8_t)

void
starlark_bytecode_disassemble(FILE* stream, bytecode* code);

#endif // __STARLARK_BYTECODE_H__