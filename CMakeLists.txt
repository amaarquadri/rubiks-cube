cmake_minimum_required(VERSION 3.20)
project(rubiks_cube)

set(CMAKE_CXX_STANDARD 20)

# increase allowed amount of work in compile time constexpr evaluations
set(CMAKE_CXX_FLAGS "-fconstexpr-ops-limit=42000000 ${CMAKE_CXX_FLAGS}")

include_directories(./include)
include_directories(./include/core)
include_directories(./include/utility)
include_directories(./include/blindsolving)
include_directories(./include/solvers)

include_directories(./include/tests)
include_directories(./include/tests/core)
include_directories(./include/tests/blindsolving)
include_directories(./include/tests/solvers)
include_directories(./include/tests/utility)

add_executable(rubiks_cube
        src/utility/SolveAttemptParsingUtils.cpp
        src/core/Algorithm.cpp
        src/core/Colour.cpp
        src/core/CornerLocation.cpp
        src/core/Cube.cpp
        src/core/CubeOrientation.cpp
        src/core/CubeRotation.cpp
        src/core/EdgeLocation.cpp
        src/core/Face.cpp
        src/core/Move.cpp
        src/core/RotationAmount.cpp
        src/core/RotationAxis.cpp
        src/core/Slice.cpp
        src/core/SliceTurn.cpp
        src/core/Turn.cpp
        src/core/WideTurn.cpp
        src/blindsolving/Blindsolving.cpp
        src/blindsolving/BlindsolvingMove.cpp
        src/blindsolving/BlindsolvingReconstruction.cpp
        src/blindsolving/CornerCycleSequenceIterator.cpp
        src/blindsolving/EdgeCycleSequenceIterator.cpp
        src/blindsolving/Lettering.cpp
        src/blindsolving/Reconstruction.cpp
        src/blindsolving/ReconstructionIterator.cpp
        src/blindsolving/SolveData.cpp
        src/solvers/EdgeOrientationSolver.cpp
        src/tests/RunTests.cpp
        src/tests/core/TestAlgorithm.cpp
        src/tests/core/TestCubeOrientation.cpp
        src/tests/core/TestPLLs.cpp
        src/tests/blindsolving/TestBlindsolvingMoveHash.cpp
        src/tests/blindsolving/TestCornerCycleSequenceIterator.cpp
        src/tests/blindsolving/TestEdgeCycleSequenceIterator.cpp
        src/tests/blindsolving/TestReconstructionIterator.cpp
        src/tests/solvers/TestEdgeOrientationSolver.cpp
        src/tests/utility/TestCombination.cpp
        src/tests/utility/TestPackedBitsArray.cpp
        src/tests/utility/TestPermutation.cpp
        main.cpp)
