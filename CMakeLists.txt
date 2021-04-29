cmake_minimum_required(VERSION 3.0)
project(huffman CXX)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set (CMAKE_CXX_STANDARD 20)

find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

include_directories(libs/fire-hpp/include REQUIRED)

add_executable(Entropy app/entropy.cxx)

target_link_libraries(Entropy
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

add_executable(Compact app/compact.cxx)

add_executable(Decompact app/decompact.cxx)
