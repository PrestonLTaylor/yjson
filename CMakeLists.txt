cmake_minimum_required(VERSION 3.24)
project(YJSON)

set(CMAKE_CXX_STANDARD 20)

add_executable(YJSON json_array.cpp json_boolean.cpp json_lexer.cpp json_number.cpp json_object.cpp json_parser.cpp main.cpp)
