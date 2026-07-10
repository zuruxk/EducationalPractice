#ifndef SINGLEPOINTCROSSOVER_H
#define SINGLEPOINTCROSSOVER_H

#include "ICrossover.hpp"

class SinglePointCrossover : public ICrossover {
public:
    std::pair<Chromosome, Chromosome> crossover (Chromosome& parent1, Chromosome& parent2) const noexcept;
    std::string getName () const noexcept;
};

#endif 