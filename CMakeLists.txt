cmake_minimum_required(VERSION 3.20)
project(rubiks_cube)

set(CMAKE_CXX_STANDARD 20)

include_directories(./include)
include_directories(./include/core)
include_directories(./include/utility)
include_directories(./include/blindsolving)

add_executable(rubiks_cube
        src/core/Algorithm.cpp
        src/core/Colour.cpp
        src/core/CornerLocation.cpp
        src/core/CornerPiece.cpp
        src/core/CornerRotationAmount.cpp
        src/core/Cube.cpp
        src/core/CubeOrientation.cpp
        src/core/CubeRotation.cpp
        src/core/EdgeLocation.cpp
        src/core/EdgePiece.cpp
        src/core/Face.cpp
        src/core/Move.cpp
        src/core/RotationAmount.cpp
        src/core/RotationAxis.cpp
        src/core/Slice.cpp
        src/core/Turn.cpp
        src/blindsolving/Blindsolving.cpp
        src/blindsolving/BlindsolvingMove.cpp
        src/blindsolving/BlindsolvingReconstruction.cpp
        src/blindsolving/CornerCycleSequenceIterator.cpp
        src/blindsolving/EdgeCycleSequenceIterator.cpp
        src/blindsolving/Lettering.cpp
        src/blindsolving/Reconstruction.cpp
        src/blindsolving/ReconstructionIterator.cpp
        src/blindsolving/SolveData.cpp
        main.cpp)
