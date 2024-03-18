cmake_minimum_required(VERSION 3.27)

project(example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wfatal-errors")

find_package(Qt6 REQUIRED COMPONENTS Core)
qt_standard_project_setup()

add_executable(main
    main.cc
    a.h
)

target_link_libraries(main PRIVATE Qt6::Core)
