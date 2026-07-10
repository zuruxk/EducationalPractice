#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include <memory>
#include <chrono>
#include <functional>

#include "Chromosome.hpp"
#include "Population.hpp"
#include "FitnessFunction.hpp"
#include "MatrixMultProblem.hpp"
#include "IMutation.hpp"
#include "ISelection.hpp"
#include "ICrossover.hpp"

struct AlgorithmParams {
    int populationSize = 100;
    int maxGenerations = 1000;
    double crossoverProbability = 0.85;
    double mutationProbability = 0.1;
    int elitismSize = 2;
};

class GeneticAlgorithm {
private:
    MatrixMultProblem problem;
    FitnessFunction fitnessFunc;
    std::unique_ptr<Population> currentPopulation;
    int genNumber = 0;
    bool finished = false;
    AlgorithmParams params;

    std::unique_ptr<ISelection> selectionOperator;
    std::unique_ptr<ICrossover> crossoverOperator;
    std::unique_ptr<IMutation> mutationOperator;

    std::vector<int> bestFitnessHistory;

    void createInitialPopulation ();
    void evaluatePopulation ();
    void updateHistory ();

public:
    explicit GeneticAlgorithm (const FitnessFunction& fitnessFunction) : problem(fitnessFunction.getProblem()), fitnessFunc(fitnessFunction) {};
    ~GeneticAlgorithm () = default;

    void setSelectionOperator (std::unique_ptr<ISelection> op) noexcept;
    void setCrossoverOperator (std::unique_ptr<ICrossover> op) noexcept;
    void setMutationOperator (std::unique_ptr<IMutation> op) noexcept;
    void setParams (const AlgorithmParams& params) noexcept;

    void initializePopulation () noexcept;
    void evolveGeneration ();
    void runFullOpt () noexcept;

    int getGenNumber () const noexcept;
    bool isFinished () const noexcept;
    Population& getCurrentPopulation () noexcept;
    Chromosome getBestSolution () const;

    const std::vector<int>& getBestFitnessHistory () const noexcept;
};


#endif