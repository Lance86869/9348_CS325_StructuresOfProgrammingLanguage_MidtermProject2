cmake_minimum_required(VERSION 3.29)
project(StructuresMidtermProject2)

set(CMAKE_CXX_STANDARD 20)

add_executable(StructuresMidtermProject2 src/main.cpp
        src/game.cpp
        include/game.h
        include/question.h
        src/question.cpp
)
