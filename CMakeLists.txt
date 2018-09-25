cmake_minimum_required(VERSION 3.7.2)
project(Knapsack C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "-fopenmp")

set(SOURCE_FILES Algorithm/algorithms.h
        Algorithm/heuristics.c
        Algorithm/simpleBranchAndBound.c
        DataStructure/DataStructure.h
        DataStructure/LinkedStack.c
        IO/IO.c
        IO/IO.h
        main.c
        utils.c
        utils.h)

add_executable(Knapsack ${SOURCE_FILES})


