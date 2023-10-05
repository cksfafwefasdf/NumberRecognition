cmake_minimum_required(VERSION 3.26)
project(number)

set(CMAKE_CXX_STANDARD 11)

add_executable(
        main.cpp
        matrix_func/matrix.h
        matrix_func/matrix.cpp
        main.cpp
        train/train.h
        train/readfile.cpp)
