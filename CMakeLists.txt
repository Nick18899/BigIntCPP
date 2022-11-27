cmake_minimum_required(VERSION 3.24)
project(BigIntCPP)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(BigIntCPP
        BigInteger.cpp
        BigInteger.hpp
        main.cpp)
