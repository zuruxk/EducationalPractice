add_library(MatrixMultProblem STATIC)

target_sources(MatrixMultProblem
    PRIVATE
    "source/MatrixMultProblem.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/MatrixMultProblem.hpp"
)

target_include_directories(MatrixMultProblem PUBLIC
    "include"
)