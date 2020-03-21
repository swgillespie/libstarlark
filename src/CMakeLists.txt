add_library(starlark SHARED
  starlark.h
  vm.h
  vm.c
)

set_property(TARGET starlark PROPERTY C_STANDARD_REQUIRED 11)
target_compile_options(starlark PRIVATE -Wextra -Wall)

add_executable(starlark-repl
  main.c
)

target_link_libraries(starlark-repl starlark)
set_property(TARGET starlark-repl PROPERTY OUTPUT_NAME starlark)