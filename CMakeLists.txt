cmake_minimum_required(VERSION 3.17)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 14)

find_package(SFML COMPONENTS graphics REQUIRED)

add_executable(GameOfLife main.cpp GameOfLife.cpp GameOfLife.h)

target_link_libraries(GameOfLife sfml-graphics)