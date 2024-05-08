# CMakeLists.txt

# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Engine VERSION 0.9)

set(SOURCES
    #    Debug.cpp
    Driver.cpp
    PST.cpp
    Search.cpp
    Board.cpp
    Bithack.cpp
    Moves.cpp
    Move.cpp
    Fen.cpp
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
# Add an executable target
add_executable(Engine ${SOURCES})


