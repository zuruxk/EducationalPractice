add_library(ShufflingMutation STATIC)

target_sources(ShufflingMutation
    PRIVATE
    "source/ShufflingMutation.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/ShufflingMutation.hpp"
)

target_include_directories(ShufflingMutation PUBLIC
    "include"
)

target_link_libraries(ShufflingMutation PRIVATE
    IMutation
)