add_library(ExchangeMutation STATIC)

target_sources(ExchangeMutation
    PRIVATE
    "source/ExchangeMutation.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/ExchangeMutation.hpp"
)

target_include_directories(ExchangeMutation PUBLIC
    "include"
)

target_link_libraries(ExchangeMutation PRIVATE
    IMutation
)