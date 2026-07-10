add_library(SinglePointCrossover STATIC)

target_sources(SinglePointCrossover
    PRIVATE
    "source/SinglePointCrossover.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/SinglePointCrossover.hpp"
)

target_include_directories(SinglePointCrossover PUBLIC
    "include"
)

target_link_libraries(SinglePointCrossover PRIVATE
    ICrossover
)