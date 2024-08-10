cmake_minimum_required(VERSION 3.26)
project(lab_02)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_02 main.cpp
        DES.cpp
        DES.h
        ECB.cpp
        ECB.h)
