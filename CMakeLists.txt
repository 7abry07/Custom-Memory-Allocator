cmake_minimum_required(VERSION 3.10)

project(main)
set(CMAKE_BUILD_TYPE Debug)
add_executable(main src/main.c src/mem.c src/mem_internals.c)
