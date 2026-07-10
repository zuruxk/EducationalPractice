#include "UniformCrossover.hpp"

std::pair<Chromosome, Chromosome> UniformCrossover::crossover (Chromosome& parent1, Chromosome& parent2) const noexcept {
    if (parent1.getLength() < 2) {
        return {parent1, parent2};
    }
    double prob = 0.5;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> distr(0.0, 1.0);
    Chromosome child1(parent1.getGenes());
    Chromosome child2(parent2.getGenes());
    for (int i = 0; i < parent1.getLength(); i++) {
        if (distr(gen) < prob) {
            child1.setGene(i, parent1.getGeneAt(i));
            child2.setGene(i, parent2.getGeneAt(i));
        }
        else {
            child1.setGene(i, parent2.getGeneAt(i));
            child2.setGene(i, parent1.getGeneAt(i));
        }
    }
    return {child1, child2};
}

std::string UniformCrossover::getName () const noexcept {
    return "Uniform crossover\n";
}