# rm -rf build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=~/Qt/6.6.2/macos/lib/cmake && cmake --build build

# cmake -S . -B build -DCMAKE_PREFIX_PATH=C:\\Qt\\6.6.0\\mingw_64\\lib\\cmake -G "MinGW Makefiles" && cmake --build build
# /c/Qt/6.6.0/mingw_64/bin/windeployqt build/main.exe

cmake_minimum_required(VERSION 3.27)

project(example VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wfatal-errors")

# find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(main
    main.cc
    mainwindow.h
    mainwindow.cc
    2048.h
    sudoku.h
    clickable_label.h
)

# target_link_libraries(main PRIVATE Qt6::Core)
target_link_libraries(main PRIVATE Qt6::Widgets)

set_target_properties(main PROPERTIES
    MACOSX_BUNDLE ON
)
