cmake_minimum_required(VERSION 2.8.4)
project(Complier)

SET(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror")
set(SOURCE_FILES main.cpp lexer.cpp symboltable.cpp global.h token.h lexer.h symboltable.h parser.h parser.cpp grammar.h test.cpp)
add_executable(Complier ${SOURCE_FILES})
