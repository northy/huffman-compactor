cmake_minimum_required(VERSION 3.0)
project(huffman CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set (CMAKE_CXX_STANDARD 20)

include_directories(libs/fire-hpp/include)

add_executable(Entropy app/entropy.cxx)

add_executable(Compact app/compact.cxx)

add_executable(Decompact app/decompact.cxx)
