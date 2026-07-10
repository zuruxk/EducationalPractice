add_library(Crossover STATIC)

target_sources(Crossover
    PRIVATE
    "SinglePointCrossover/source/SinglePointCrossover.cpp"
    "DoublePointCrossover/source/DoublePointCrossover.cpp"
    "UniformCrossover/source/UniformCrossover.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "ICrossover/include/ICrossover.hpp"
    "SinglePointCrossover/include/SinglePointCrossover.hpp"
    "DoublePointCrossover/include/DoublePointCrossover.hpp"
    "UniformCrossover/include/UniformCrossover.hpp"
)

target_include_directories(Сrossover PUBLIC
    "ICrossover/include"
    "SinglePointCrossover/include"
    "DoublePointCrossover/include"
    "UniformCrossover/include"
)

target_link_libraries(Crossover PRIVATE
    Chromosome
)