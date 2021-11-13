cmake_minimum_required(VERSION 3.17)
project(CrosswalkSim)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(CrosswalkSim
        Simulation.cpp RandomFunctions.cpp RandomFunctions.h Event.cpp Event.h Automobile.cpp Automobile.h Pedestrian.cpp Pedestrian.h)
