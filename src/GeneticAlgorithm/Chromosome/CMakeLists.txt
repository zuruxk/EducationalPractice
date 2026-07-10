add_library(Chromosome STATIC)

target_sources(Chromosome
    PRIVATE
    "source/Chromosome.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/Chromosome.hpp"
)

target_include_directories(Chromosome PUBLIC
    "include"
)