add_library(starlark-py SHARED python.c)

target_link_libraries(starlark-py starlark)
target_include_directories(starlark-py SYSTEM PUBLIC ${PYTHON_INCLUDE_DIRS})
target_include_directories(starlark-py PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_compile_options(starlark-py PRIVATE -Wextra -Wall -Wno-missing-field-initializers -g -Og)
set_target_properties(starlark-py PROPERTIES
  PREFIX ""
  OUTPUT_NAME "starlark"
  LINKER_LANGUAGE C
  C_STANDARD 11
)
