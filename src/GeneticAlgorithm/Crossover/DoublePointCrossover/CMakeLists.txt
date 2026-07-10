add_library(DoublePointCrossover STATIC)

target_sources(DoublePointCrossover
    PRIVATE
    "source/DoublePointCrossover.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/DoublePointCrossover.hpp"
)

target_include_directories(DoublePointCrossover PUBLIC
    "include"
)

target_link_libraries(DoublePointCrossover PRIVATE
    ICrossover
)