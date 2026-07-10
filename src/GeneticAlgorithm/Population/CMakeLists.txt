add_library(Population STATIC)

target_sources(Population
    PRIVATE
    "source/Population.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/Population.hpp"
)

target_include_directories(Population PUBLIC
    "include"
)

target_link_libraries(Population PRIVATE
    Chromosome
)