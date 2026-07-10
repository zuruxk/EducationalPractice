cmake_minimum_required(VERSION 3.10)
project(IMutation)

add_library(IMutation INTERFACE)

target_include_directories(IMutation INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Chromosome/include
)