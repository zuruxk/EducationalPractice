#ifndef DOUBLEPOINTCROSSOVER_H
#define DOUBLEPOINTCROSSOVER_H

#include "ICrossover.hpp"

class DoublePointCrossover : public ICrossover {
public:
    std::pair<Chromosome, Chromosome> crossover (Chromosome& parent1, Chromosome& parent2) const noexcept;
    std::string getName () const noexcept;
};

#endif 