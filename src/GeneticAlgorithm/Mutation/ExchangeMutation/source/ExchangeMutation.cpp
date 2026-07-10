#include "ExchangeMutation.hpp"

void ExchangeMutation::mutate (Chromosome& chrom, double probability) noexcept {
    if (chrom.getLength() < 2) {
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0.0, 1.0);
    if (distr(gen) >= probability) {
        return;
    }
    std::uniform_int_distribution<> idistr(0, chrom.getLength() - 1);
    int index1 = idistr(gen);
    int index2 = idistr(gen);
    int gene = chrom.getGeneAt(index1);
    chrom.setGene(index1, chrom.getGeneAt(index2));
    chrom.setGene(index2, gene);
    chrom.resetFitness();
}

std::string ExchangeMutation::getName () const noexcept {
    return "Exchange Mutation\n";
}