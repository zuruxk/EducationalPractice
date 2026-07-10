#include "../include/DoublePointCrossover.hpp"

std::pair<Chromosome, Chromosome> DoublePointCrossover::crossover (Chromosome& parent1, Chromosome& parent2) const noexcept {
    if (parent1.getLength() < 2) {
        return {parent1, parent2};
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> sdistr(0, parent1.getLength() - 1);
    int start = sdistr(gen);
    std::uniform_int_distribution<> ldistr(0, parent1.getLength() - start);
    int length = ldistr(gen);
    int end = start + length - 1;
    auto genes1 = parent1.getGenes();
    auto genes2 = parent2.getGenes();
    while (start < end) {
        std::swap(genes1[start], genes2[start]);
        start++;
    }
    Chromosome child1(genes1);
    Chromosome child2(genes2);
    return {child1, child2};
}

std::string DoublePointCrossover::getName () const noexcept {
    return "Double Point Crossover\n";
}