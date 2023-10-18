cmake_minimum_required(VERSION 3.22)

project(main VERSION 1.0 DESCRIPTION "main")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(liph)

add_executable(main examples/main.cpp)

target_link_libraries(main PUBLIC liph)

target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}")

add_compile_definitions(NOSQLITE)
