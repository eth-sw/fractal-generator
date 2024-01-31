cmake_minimum_required(VERSION 3.26)
project(main_assignment C)

set(CMAKE_C_STANDARD 11)

add_executable(main_assignment main.c
        file_parser.c
        file_parser.h
        fractal_generator.c
        fractal_generator.h
        structures.h
)
