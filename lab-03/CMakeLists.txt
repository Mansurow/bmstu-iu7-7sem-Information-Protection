cmake_minimum_required(VERSION 3.26)
project(lab_03)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_03 main.cpp
        aes.cpp
        aes.h
        cfb.cpp
        cfb.h)
