add_library(ConversionMutation STATIC)

target_sources(ConversionMutation
    PRIVATE
    "source/ConversionMutation.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/ConversionMutation.hpp"
)

target_include_directories(ConversionMutation PUBLIC
    "include"
)

target_link_libraries(ConversionMutation PRIVATE
    IMutation
)