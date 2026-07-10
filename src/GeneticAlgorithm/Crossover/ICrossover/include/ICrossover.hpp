#ifndef ICROSSOVER_H
#define ICROSSOVER_H

#include <random>
#include "Chromosome.hpp"

class ICrossover {
public:
    virtual ~ICrossover () = default;

    virtual std::pair<Chromosome, Chromosome> crossover (Chromosome& parent1, Chromosome& parent2) const noexcept = 0;
    virtual std::string getName () const noexcept = 0;
};

#endif