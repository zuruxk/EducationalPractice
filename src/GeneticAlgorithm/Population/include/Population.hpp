#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <string>

#include "Chromosome.hpp"

class Population {
private:
    std::vector<Chromosome> individuals;
    int size;

public:
    Population () = default;
    explicit Population (const std::vector<Chromosome>& chroms) noexcept;
    
    void addIndividual (const Chromosome& chrom) noexcept;
    void addIndividuals (const std::vector<Chromosome>& chroms) noexcept;
    void removeIndividual (int index);
    
    const Chromosome& getIndividual (int index) const;
    const std::vector<Chromosome>& getIndividuals () const noexcept;
    int getSize () const noexcept;

    bool isEmpty () const noexcept;
    std::string toString () const noexcept;
};

#endif