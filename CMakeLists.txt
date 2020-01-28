cmake_minimum_required(VERSION 3.14)
project(Tiny_Basic)

set(CMAKE_CXX_STANDARD 14)

add_executable(Tiny_Basic main.cpp Expression.cpp Expression.h helpers.cpp helpers.h)