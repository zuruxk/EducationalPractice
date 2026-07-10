#include "ShufflingMutation.hpp"

void ShufflingMutation::mutate (Chromosome& chrom, double probability) noexcept {
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
    int start = idistr(gen);
    std::uniform_int_distribution<> ldistr(1, chrom.getLength() - start);
    int length = ldistr(gen);
    int end = start + length - 1;
    auto genes = chrom.getGenes();
    std::mt19937 g(rd());
    std::shuffle(genes.begin() + start, genes.begin() + end, g);
    chrom.setGenes(genes);
    chrom.resetFitness();
}

std::string ShufflingMutation::getName () const noexcept {
    return "Shuffling Mutation\n";
}