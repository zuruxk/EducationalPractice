#include "Population.hpp"

Population::Population (const std::vector<Chromosome>& chroms) noexcept : individuals(chroms) {
    size = static_cast<int>(chroms.size());
}

void Population::addIndividual (const Chromosome& chrom) noexcept {
    individuals.push_back(chrom);
}

void Population::addIndividuals (const std::vector<Chromosome>& chroms) noexcept {
    individuals.insert(individuals.end(), chroms.begin(), chroms.end());
}

void Population::removeIndividual (int index) {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    individuals.erase(individuals.begin() + index);
}
    
const Chromosome& Population::getIndividual (int index) const {
    if (index < 0 || index >= size) {
        throw std::out_of_range("Index out of range");
    }
    return individuals[index];
}

const std::vector<Chromosome>& Population::getIndividuals () const noexcept {
    return individuals;
}

int Population::getSize () const noexcept {
    return size;
}

bool Population::isEmpty () const noexcept {
    return individuals.empty();
}

std::string Population::toString () const noexcept {
    std::ostringstream ss;
    ss << "Population:\n";
    for (auto const& individual: individuals) {
        ss << individual.toString() << "\n";
    }
}