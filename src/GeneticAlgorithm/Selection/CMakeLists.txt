add_library(Selection STATIC)

target_sources(Selection
    PRIVATE
    "TournamentSelection/source/TournamentSelection.cpp"
    PUBLIC
    FILE_SET HEADERS 
    BASE_DIRS "."
    FILES
    "ISelection/include/IMutation.hpp"
    "TournamentSelection/include/TournamentSelection.hpp"
)

target_include_directories(Selection PUBLIC
    "ISelection/include"
    "TournamentSelection/include"
)

target_link_libraries(Selection PRIVATE
    Population
)