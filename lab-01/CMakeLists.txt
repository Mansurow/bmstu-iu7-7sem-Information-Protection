cmake_minimum_required(VERSION 3.26)
project(lab_01)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab_01 main.cpp
        src/enigma.cpp
        src/enigma.h
        src/Rotor.cpp
        src/Rotor.h
        src/Reflector.cpp
        src/Reflector.h
        src/encoder.cpp
        src/encoder.h
)
