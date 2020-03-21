cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(starlark VERSION 0.1
                 DESCRIPTION "An efficient implementation of Starlark"
                 LANGUAGES C)

find_package(PythonInterp 3.6)

# This goes after, since it uses PythonInterp as hint
find_package(PythonLibs 3.6)

add_subdirectory(src)
if (PythonLibs_FOUND)
    message(STATUS "Compiling Python bindings: ${PYTHON_INCLUDE_DIRS}")
    add_subdirectory(src/py)
endif (PythonLibs_FOUND)