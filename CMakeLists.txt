cmake_minimum_required(VERSION 3.20)
project(rubiks_cube)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include)

add_executable(rubiks_cube
        src/Colour.cpp
        src/Face.cpp
        src/Slice.cpp
        src/RotationAxis.cpp
        src/RotationAmount.cpp
        src/EdgePiece.cpp
        src/CornerPiece.cpp
        src/EdgeLocation.cpp
        src/CornerLocation.cpp
        src/Turn.cpp
        src/CubeRotation.cpp
        src/Move.cpp
        src/CubeOrientation.cpp
        src/Algorithm.cpp
        src/Cube.cpp
        src/Blindsolving.cpp
        main.cpp)
