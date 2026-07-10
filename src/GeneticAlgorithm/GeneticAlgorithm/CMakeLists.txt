add_library(GeneticAlgorithm STATIC)

target_sources(GeneticAlgorithm
    PRIVATE
    "source/GeneticAlgorithm.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/GeneticAlgorithm.hpp"
)

target_include_directories(GeneticAlgorithm PUBLIC
    "include"
)

target_link_libraries(GeneticAlgorithm PRIVATE
    Chromosome
    Population
    FitnessFunction
    MatrixMultProblem
    IMutation
    ISelection
    ICrossover
)