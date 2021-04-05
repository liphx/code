cmake_minimum_required(VERSION 3.19)

project(qt-demo VERSION 1.0
                DESCRIPTION "first qt project")

set(QT_INSTALL_DIR "/opt/Qt5.14.2/5.14.2/gcc_64/")

add_definitions(-DQT_NO_VERSION_TAGGING)
add_compile_options(-fPIC)

add_executable(qt-demo qt-demo.cpp)

target_include_directories(qt-demo PUBLIC
                            "${QT_INSTALL_DIR}/include"
                            "${QT_INSTALL_DIR}/include/QtWidgets"
                            )

target_link_libraries(qt-demo PUBLIC 
                        Qt5Widgets
                        Qt5Gui
                        Qt5Core)

target_link_directories(qt-demo PUBLIC
                        "${QT_INSTALL_DIR}/lib"
                        )
