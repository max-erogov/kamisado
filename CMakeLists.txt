cmake_minimum_required(VERSION 3.26)
project(kamisado)

set(CMAKE_CXX_STANDARD 20)

add_executable(kamisado main.cpp)

target_link_libraries(kamisado sfml-system sfml-window sfml-graphics)

