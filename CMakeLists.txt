cmake_minimum_required(VERSION 3.20)
project(rubiks_cube)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include)

add_executable(rubiks_cube
        src/Colour.cpp
        src/Face.cpp
        src/EdgePiece.cpp
        src/CornerPiece.cpp
        src/EdgeLocation.cpp
        src/CornerLocation.cpp
        src/RotationAmount.cpp
        src/Turn.cpp
        src/CubeOrientation.cpp
        src/CubeRotation.cpp
        src/Algorithm.cpp
        src/Cube.cpp
        main.cpp)
