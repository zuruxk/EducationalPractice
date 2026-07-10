cmake_minimum_required(VERSION 3.16)
project(MatrixChainOptimization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
option(ENABLE_ASAN "Enable AddressSanitizer" ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)

# ==========================================
# Базовые библиотеки
# ==========================================
add_subdirectory(GeneticAlgorithm/MatrixMultProblem)
add_subdirectory(GeneticAlgorithm/Chromosome)
add_subdirectory(GeneticAlgorithm/Population)
add_subdirectory(GeneticAlgorithm/FitnessFunction)

# ==========================================
# И
# ==========================================
add_subdirectory(GeneticAlgorithm/Mutation/IMutation)
add_subdirectory(GeneticAlgorithm/Selection/ISelection)
add_subdirectory(GeneticAlgorithm/Crossover/ICrossover)

# ==========================================
# Реализации мутаций (все есть!)
# ==========================================
add_subdirectory(GeneticAlgorithm/Mutation/ConversionMutation)
add_subdirectory(GeneticAlgorithm/Mutation/ExchangeMutation)
add_subdirectory(GeneticAlgorithm/Mutation/ShufflingMutation)

# ==========================================
# Реализации отбора
# ==========================================
add_subdirectory(GeneticAlgorithm/Selection/TournamentSelection)

# ==========================================
# Реализации скрещивания
# ==========================================
add_subdirectory(GeneticAlgorithm/Crossover/SinglePointCrossover)
add_subdirectory(GeneticAlgorithm/Crossover/DoublePointCrossover)
add_subdirectory(GeneticAlgorithm/Crossover/UniformCrossover)

# ==========================================
# Сам GeneticAlgorithm (главный класс)
# ==========================================
add_subdirectory(GeneticAlgorithm/GeneticAlgorithm)

# ==========================================
# GUI (Visuals)
# ==========================================
add_subdirectory(Visuals)