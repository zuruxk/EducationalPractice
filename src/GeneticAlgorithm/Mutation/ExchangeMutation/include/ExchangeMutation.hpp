#ifndef EXCHANGEMUTATION_H
#define EXCHANGEMUTATION_H

#include "IMutation.hpp"

class ExchangeMutation : public IMutation {
public:
    void mutate (Chromosome& chrom, double probability) noexcept;
    std::string getName () const noexcept;
};

#endif