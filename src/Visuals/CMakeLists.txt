cmake_minimum_required(VERSION 3.16)
project(Visuals)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(SOURCES
    source/MainWindow.cpp
)

set(HEADERS
    include/MainWindow.hpp
)

add_executable(MatrixChainGUI
    ../main.cpp
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(MatrixChainGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/GeneticAlgorithm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/MatrixMultProblem/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Chromosome/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Population/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/FitnessFunction/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Mutation/IMutation/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Selection/ISelection/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Crossover/ICrossover/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Mutation/ConversionMutation/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Mutation/ExchangeMutation/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Mutation/ShufflingMutation/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Selection/TournamentSelection/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Crossover/SinglePointCrossover/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Crossover/DoublePointCrossover/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../GeneticAlgorithm/Crossover/UniformCrossover/include
)

target_link_libraries(MatrixChainGUI
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    GeneticAlgorithm
    MatrixMultProblem
    Chromosome
    Population
    FitnessFunction
    IMutation
    ISelection
    ICrossover
    ConversionMutation
    ExchangeMutation
    ShufflingMutation
    TournamentSelection
    SinglePointCrossover
    DoublePointCrossover
    UniformCrossover
)