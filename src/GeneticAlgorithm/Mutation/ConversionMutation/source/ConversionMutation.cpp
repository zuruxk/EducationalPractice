#include "ConversionMutation.hpp"

void ConversionMutation::mutate (Chromosome& chrom, double probability) noexcept {
    if (chrom.getLength() < 2) {
        return;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(0.0, 1.0);
    if (distr(gen) >= probability) {
        return;
    }
    std::uniform_int_distribution<> sdistr(0, chrom.getLength() - 1);
    int start = sdistr(gen);
    std::uniform_int_distribution<> ldistr(1, chrom.getLength() - start);
    int length = ldistr(gen);
    int end = start + length - 1;
    auto genes = chrom.getGenes();
    while (start < end) {
        std::swap(genes[start], genes[end]);
        start++;
        end--;
    }
    chrom.setGenes(genes);
    chrom.resetFitness();
}

std::string ConversionMutation::getName () const noexcept {
    return "Conversion Mutation\n";
}