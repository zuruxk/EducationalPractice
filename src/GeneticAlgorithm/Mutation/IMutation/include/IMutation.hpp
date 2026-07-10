#ifndef IMUTATION_H
#define IMUTATION_H

#include <random>
#include "Chromosome.hpp"

class IMutation {
public:
    virtual ~IMutation () = default;

    virtual void mutate (Chromosome& chrom, double probability) noexcept = 0;
    virtual std::string getName () const noexcept = 0;
};

#endif