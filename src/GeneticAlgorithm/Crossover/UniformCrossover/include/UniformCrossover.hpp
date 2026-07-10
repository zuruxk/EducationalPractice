#ifndef UNIFORMCROSSOVER_H
#define UNIFORMCROSSOVER_H

#include "ICrossover.hpp"

class UniformCrossover : public ICrossover {
public:
    std::pair<Chromosome, Chromosome> crossover (Chromosome& parent1, Chromosome& parent2) const noexcept;
    std::string getName () const noexcept;
};

#endif 