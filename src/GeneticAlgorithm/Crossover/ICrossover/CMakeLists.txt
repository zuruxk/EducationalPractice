cmake_minimum_required(VERSION 3.10)
project(ICrossover)

add_library(ICrossover INTERFACE)

target_include_directories(ICrossover INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Chromosome/include
)