cmake_minimum_required(VERSION 3.10)
project(ISelection)

add_library(ISelection INTERFACE)

target_include_directories(ISelection INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Population/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Chromosome/include
)