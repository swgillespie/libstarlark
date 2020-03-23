add_library(starlark SHARED
  compiler.c
  gc.c
  vm.c
  bytecode.c
  util.c
  value.c
)

set_target_properties(starlark PROPERTIES C_STANDARD_REQUIRED 11)
target_compile_options(starlark PRIVATE -Wextra -Wall -g -Og)

add_executable(starlark-repl
  main.c
)

target_link_libraries(starlark-repl starlark)
set_target_properties(starlark-repl PROPERTIES OUTPUT_NAME starlark)
target_compile_options(starlark-repl PRIVATE -Wextra -Wall -g -Og)
