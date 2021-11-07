cmake_minimum_required(VERSION 3.20)
project(rubiks_cube)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include)
include_directories(./include/core)
include_directories(./include/utility)
include_directories(./include/blindsolving)

add_executable(rubiks_cube
        src/core/Colour.cpp
        src/core/Face.cpp
        src/core/Slice.cpp
        src/core/RotationAxis.cpp
        src/core/RotationAmount.cpp
        src/core/EdgePiece.cpp
        src/core/CornerPiece.cpp
        src/core/EdgeLocation.cpp
        src/core/CornerLocation.cpp
        src/core/Turn.cpp
        src/core/CubeRotation.cpp
        src/core/Move.cpp
        src/core/CubeOrientation.cpp
        src/core/Algorithm.cpp
        src/core/Cube.cpp
        src/blindsolving/SolveData.cpp
        src/blindsolving/Reconstruction.cpp
        src/blindsolving/Blindsolving.cpp
        main.cpp)
