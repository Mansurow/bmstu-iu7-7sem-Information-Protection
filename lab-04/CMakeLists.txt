cmake_minimum_required(VERSION 3.26)
project(lab_04)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab_04 main.cpp
    ap.hpp
    array.hpp
    asm.hpp
    asm.cpp
    common.cpp
    common.h
    config.hpp
    integer.hpp
    integer_api.hpp
    integer_api.cpp
    rsa.h
    rsa.cpp
    sha1.cpp
    sha1.h
        common.cpp)
