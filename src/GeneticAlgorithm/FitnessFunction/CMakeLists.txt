add_library(FitnessFunction STATIC)

target_sources(FitnessFunction
    PRIVATE
    "source/FitnessFunction.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/FitnessFunction.hpp"
)

target_include_directories(FitnessFunction PUBLIC
    "include"
)

target_link_libraries(FitnessFunction PRIVATE
    MatrixMultProblem
    Chromosome
)