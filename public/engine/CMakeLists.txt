# CMakeLists.txt

# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Engine VERSION 0.9)

set(SOURCES
    Driver.cpp
    Engine.cpp
    Board.cpp
    Bithack.cpp
    Moves.cpp
    Move.cpp
    Fen.cpp
    )
# Add an executable target
add_executable(Engine ${SOURCES})


