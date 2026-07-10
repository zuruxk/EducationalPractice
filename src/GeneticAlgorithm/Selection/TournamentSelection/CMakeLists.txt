add_library(TournamentSelection STATIC)

target_sources(TournamentSelection
    PRIVATE
    "source/TournamentSelection.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "include/TournamentSelection.hpp"
)

target_include_directories(TournamentSelection PUBLIC
    "include"
)

target_link_libraries(TournamentSelection PRIVATE
    ISelection
)