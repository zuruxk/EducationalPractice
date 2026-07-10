add_library(Mutation STATIC)

target_sources(Mutation
    PRIVATE
    "ConversionMutation/source/ConversionMutation.cpp"
    "ExchangeMutation/source/ExchangeMutation.cpp"
    "ShufflingMutation/source/ShufflingMutation.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "IMutation/include/IMutation.hpp"
    "ConversionMutation/include/ConversionMutation.hpp"
    "ExchangeMutation/include/ExchangeMutation.hpp"
    "ShufflingMutation/include/ShufflingMutation.hpp"
)

target_include_directories(Mutation PUBLIC
    "IMutation/include"
    "ConversionMutation/include"
    "ExchangeMutation/include"
    "ShufflingMutation/include"
)

target_link_libraries(Mutation PRIVATE
    Chromosome
)