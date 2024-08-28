cmake_minimum_required(VERSION 3.5)
project(nlib)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)

add_subdirectory(set)

add_dependencies(nset_test nset)
