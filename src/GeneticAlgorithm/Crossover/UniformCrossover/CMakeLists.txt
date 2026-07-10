add_library(UniformCrossover STATIC)

target_sources(UniformCrossover
    PRIVATE
    "source/UniformCrossover.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/UniformCrossover.hpp"
)

target_include_directories(UniformCrossover PUBLIC
    "include"
)

target_link_libraries(UniformCrossover PRIVATE
    ICrossover
)