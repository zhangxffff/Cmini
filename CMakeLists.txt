cmake_minimum_required(VERSION 2.8.4)
project(Complier)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
AUX_SOURCE_DIRECTORY(./ SOURCE_FILES)
#set(SOURCE_FILES main.cpp lexer.cpp emitter.cpp error.cpp init.cpp parser.cpp symbol.cpp global.h token.h lexer.h symbol.h parser.h)
add_executable(Complier ${SOURCE_FILES})
