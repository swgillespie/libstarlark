#ifndef __STARLARK_COMPILER_H__
#define __STARLARK_COMPILER_H__

#include <stdint.h>
#include <stdlib.h>

typedef struct starlark_bytecode_t
{
  uint16_t count;
  uint16_t capacity;
  uint8_t* code;
} starlark_bytecode_t;

void
starlark_bytecode_disassemble(starlark_bytecode_t* bytecode);

#endif // __STARLARK_COMPILER_H__