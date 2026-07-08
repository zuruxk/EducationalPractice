#include "Chromosome.hpp"

Chromosome::Chromosome (const std::vector<int>& genes) : genes(genes), fitness(-1) {
    if (genes.empty()) {
        throw std::invalid_argument("Genes must not be empty");
    }
    for (size_t i = 0; i < genes.size(); i++) {
        if (genes[i] <= 0) {
            throw std::invalid_argument("Genes must be positive");
        }
    }
}

int Chromosome::getGeneAt (int index) const {
    if (index < 0 || index >= static_cast<int>(genes.size())) {
        throw std::out_of_range("Index out of range");
    }
    return genes[index];
}

int Chromosome::getLength () const noexcept {
    return static_cast<int>(genes.size());
}

const std::vector<int>& Chromosome::getGenes () const noexcept {
    return genes;
}

void Chromosome::setGene (int index, int gene) {
    if (index < 0 || index >= static_cast<int>(genes.size())) {
        throw std::out_of_range("Index out of range");
    }
    if (gene <= 0) {
        throw std::invalid_argument("Gene must be positive");
    }
    genes[index] = gene;
}

int Chromosome::getFitness () const noexcept {
    return fitness;
}

void Chromosome::setGenes (const std::vector<int>& genes) {
    if (genes.empty()) {
        throw std::invalid_argument("Genes must not be empty");
    }
    for (size_t i = 0; i < genes.size(); i++) {
        if (genes[i] <= 0) {
            throw std::invalid_argument("Genes must be positive");
        }
    }
    this->genes = genes;
}

void Chromosome::setFitness (int value) noexcept {
    fitness = value;
}

bool Chromosome::isFitnessCalculated () const {
    return fitness == -1;
}

const std::vector<int>& Chromosome::decodeToOrder () const noexcept {
    std::vector<std::pair<int, int>> priorityPosition;
    for (size_t i = 0; i < genes.size(); i++) {
        priorityPosition.emplace_back(genes[i], static_cast<int>(i));
    }
    std::sort(priorityPosition.begin(), priorityPosition.end());
    std::vector<int> order;
    for (auto const& p: priorityPosition) {
        order.push_back(p.second);
    }
    return order;
}

std::string Chromosome::toOrderString () const noexcept {
    std::vector<int> order = decodeToOrder();
    std::ostringstream ss;
    ss << "Decoded order:\n";
    for (auto const& o: order) {
        ss << o << " ";
    }
    return ss.str();
}

std::string Chromosome::toString () const noexcept {
    std::ostringstream ss;
    ss << "Chromosome:\n";
    for (auto const& gene: genes) {
        ss << gene << " ";
    }
    return ss.str();
}

