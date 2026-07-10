#include "../include/GeneticAlgorithm.hpp"
#include <iostream>

void GeneticAlgorithm::setSelectionOperator (std::unique_ptr<ISelection> op) noexcept {
    selectionOperator = std::move(op); 
}

void GeneticAlgorithm::setCrossoverOperator (std::unique_ptr<ICrossover> op) noexcept {
    crossoverOperator = std::move(op);
}

void GeneticAlgorithm::setMutationOperator (std::unique_ptr<IMutation> op) noexcept {
    mutationOperator = std::move(op);
}

void GeneticAlgorithm::setParams (const AlgorithmParams& params) noexcept {
    this->params = params;
}

void GeneticAlgorithm::initializePopulation () noexcept {
    genNumber = 0;
    finished = false;
    bestFitnessHistory.clear();
    createInitialPopulation();
}

void GeneticAlgorithm::evolveGeneration () {
    if (finished) {
        return;
    }
    if (currentPopulation->isEmpty()) {
        initializePopulation();
        return;
    }
    const auto& currentInds = currentPopulation->getIndividuals();
    int popSize = currentPopulation->getSize();
    int chromLength = currentInds[0].getLength();
    auto sorted = currentInds;
    std::sort(sorted.begin(), sorted.end(), [](const Chromosome& a, const Chromosome& b) {
        return a.getFitness() < b.getFitness();
    });
    
    std::vector<Chromosome> nextGeneration;
    nextGeneration.reserve(popSize);
    int elitismCount = std::min(params.elitismSize, popSize);
    for (int i = 0; i < elitismCount; ++i) {
        nextGeneration.push_back(sorted[i]);
    }
    int parentsNeeded = (popSize - elitismCount) * 2;
    int selectionCount = std::min(parentsNeeded, popSize);
    auto parents = selectionOperator->select(*currentPopulation, selectionCount);
    
    for (size_t i = 0; i + 1 < parents.size(); i += 2) {
        if ((int)nextGeneration.size() >= popSize) break;

        auto children = crossoverOperator->crossover(parents[i], parents[i+1]);
        
        mutationOperator->mutate(children.first, params.mutationProbability);
        mutationOperator->mutate(children.second, params.mutationProbability);

        children.first.setFitness(fitnessFunc.calculate(children.first));
        children.second.setFitness(fitnessFunc.calculate(children.second));

        nextGeneration.push_back(children.first);
        if ((int)nextGeneration.size() < popSize) {
            nextGeneration.push_back(children.second);
        }
    }

    currentPopulation = std::make_unique<Population>(nextGeneration);
    
    genNumber++;
    updateHistory();

    if (genNumber >= params.maxGenerations) {
        finished = true;
    }
}

void GeneticAlgorithm::runFullOpt () noexcept {
    if (currentPopulation->isEmpty()) {
        initializePopulation();
    }
    while (!finished) {
        evolveGeneration();
    }
}

int GeneticAlgorithm::getGenNumber () const noexcept {
    return genNumber;
}

bool GeneticAlgorithm::isFinished () const noexcept {
    return finished;
}

Population& GeneticAlgorithm::getCurrentPopulation () noexcept {
    if (!currentPopulation) {
        throw std::runtime_error("Population is not initialized");
    }
    return *currentPopulation;
}

Chromosome GeneticAlgorithm::getBestSolution () const {
    if (currentPopulation->isEmpty()) {
        throw std::runtime_error("Population is empty");
    }
    return currentPopulation->getBestIndividual();
}

const std::vector<int>& GeneticAlgorithm::getBestFitnessHistory () const noexcept {
    return bestFitnessHistory;
}

void GeneticAlgorithm::createInitialPopulation () {
    int N = params.populationSize;
    int chromLength = fitnessFunc.getProblem().getDimensionsCount() - 1;
    if (chromLength <= 0) {
        throw std::runtime_error("Invalid chromosome length");
    }
    std::vector<Chromosome> individuals;
    individuals.reserve(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> geneDist(1, chromLength);
    for (int i = 0; i < N; i++) {
        std::vector<int> genes;
        genes.reserve(chromLength);
        for (int j = 0; j < chromLength; ++j) {
            genes.push_back(geneDist(gen));
        }  
        Chromosome chrom(genes);
        individuals.push_back(chrom);
    }
    currentPopulation = std::make_unique<Population>(individuals);
    evaluatePopulation();
    updateHistory();
}

void GeneticAlgorithm::evaluatePopulation () {
    for (auto& chrom: currentPopulation->getIndividuals()) {
        if (!chrom.isFitnessCalculated()) {
            chrom.setFitness(fitnessFunc.calculate(chrom));
        }
    }
}

void GeneticAlgorithm::updateHistory () {
    if (currentPopulation->isEmpty()) {
        return;
    }
    bestFitnessHistory.push_back(currentPopulation->getBestIndividual().getFitness());
}