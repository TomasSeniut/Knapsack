cmake_minimum_required(VERSION 3.7.2)
project(Knapsack C)

find_package(MPI REQUIRED)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-fopenmp")

include_directories(${MPI_INCLUDE_PATH})

SET(CMAKE_C_COMPILER mpicc)
SET(CMAKE_CXX_COMPILER mpicxx)

set(SOURCE_FILES Algorithm/algorithms.h
        Algorithm/heuristics.c
        Algorithm/simpleBranchAndBound.c
        Algorithm/parallelBranchAndBound.c
        DataStructure/DataStructure.h
        DataStructure/LinkedStack.c
        IO/IO.c
        IO/IO.h
        main.c
        utils.c
        utils.h
        DataStructure/ParallelStack.c
        DataStructure/ParallelStack.h
        DataStructure/LinkedStack.h
        MPI/mpiWrapper.c
        MPI/mpiWrapper.h Algorithm/hybridBranchAndBound.c DataStructure/LinkedQueue.c DataStructure/LinkedQueue.h)

add_executable(Knapsack ${SOURCE_FILES})


