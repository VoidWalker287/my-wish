cmake_minimum_required(VERSION 3.30)
project(wish C)

set(CMAKE_C_STANDARD 99)

add_executable(wish main.c commands.c main_utils.c)
