#ifndef __STARLARK_VECTOR_H__
#define __STARLARK_VECTOR_H__

#include "util.h"

#define VECTOR_INITIAL_SIZE 10

#define DECLARE_VECTOR(name, T)                                                \
  typedef struct name                                                          \
  {                                                                            \
    uint32_t count;                                                            \
    uint32_t capacity;                                                         \
    T* data;                                                                   \
  } name;                                                                      \
                                                                               \
  void starlark_##name##_new(name* vector);                                    \
  void starlark_##name##_free(name* vector);                                   \
  void starlark_##name##_append(name* vector, T data);

#define DEFINE_VECTOR(name, T)                                                 \
  void starlark_##name##_new(name* vector)                                     \
  {                                                                            \
    INVARIANT(vector != NULL);                                                 \
    vector->count = 0;                                                         \
    vector->capacity = 0;                                                      \
    vector->data = NULL;                                                       \
  }                                                                            \
                                                                               \
  void starlark_##name##_free(name* vector)                                    \
  {                                                                            \
    INVARIANT(vector != NULL);                                                 \
    free(vector->data);                                                        \
    vector->count = 0;                                                         \
    vector->capacity = 0;                                                      \
    vector->data = NULL;                                                       \
  }                                                                            \
                                                                               \
  void starlark_##name##_append(name* vector, T data)                          \
  {                                                                            \
    if (vector->data == NULL) {                                                \
      vector->capacity = VECTOR_INITIAL_SIZE;                                  \
      vector->data = calloc(VECTOR_INITIAL_SIZE, sizeof(uint8_t));             \
      INVARIANT(vector->data != NULL);                                         \
    }                                                                          \
                                                                               \
    if (vector->count >= vector->capacity) {                                   \
      vector->capacity = vector->capacity * 2;                                 \
      vector->data = realloc(vector->data, vector->capacity);                  \
      INVARIANT(vector->data != NULL);                                         \
    }                                                                          \
                                                                               \
    INVARIANT(vector->count < vector->capacity);                               \
    vector->data[vector->count++] = data;                                      \
  }

#define DECLARE_FROZEN_VECTOR(name, from, T)                                   \
  typedef struct name                                                          \
  {                                                                            \
    uint32_t count;                                                            \
    T* data;                                                                   \
  } name;                                                                      \
                                                                               \
  void starlark_##name##_new(from* vector, name* out);                         \
  void starlark_##name##_free(name* vector);

#define DEFINE_FROZEN_VECTOR(name, from, T)                                    \
  void starlark_##name##_new(from* vector, name* out)                          \
  {                                                                            \
    INVARIANT(vector != NULL);                                                 \
    INVARIANT(out != NULL);                                                    \
    out->count = vector->count;                                                \
    out->data = vector->data;                                                  \
    vector->count = 0;                                                         \
    vector->capacity = 0;                                                      \
    vector->data = NULL;                                                       \
  }                                                                            \
                                                                               \
  void starlark_##name##_free(name* vector)                                    \
  {                                                                            \
    INVARIANT(vector != NULL);                                                 \
    free(vector->data);                                                        \
    vector->count = 0;                                                         \
  }

#endif // __STARLARK_VECTOR_H__