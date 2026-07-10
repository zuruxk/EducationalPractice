#include "SinglePointCrossover.hpp"

std::pair<Chromosome, Chromosome> SinglePointCrossover::crossover (Chromosome& parent1, Chromosome& parent2) const noexcept {
    if (parent1.getLength() < 2) {
        return {parent1, parent2};
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> idistr(0, parent1.getLength() - 1);
    int index = idistr(gen);
    auto genes1 = parent1.getGenes();
    auto genes2 = parent2.getGenes();
    for (int i = 0; i <= index; i++) {
        std::swap(genes1[i], genes2[i]);
    }
    Chromosome child1(genes1);
    Chromosome child2(genes2);
    return {child1, child2};
}

std::string SinglePointCrossover::getName () const noexcept {
    return "SinglePoint Crossover\n";
}