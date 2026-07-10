#ifndef SHUFFLINGMUTATION_H
#define SHUFFLINGMUTATION_H

#include "IMutation.hpp"

class ShufflingMutation : public IMutation {
public:
    void mutate (Chromosome& chrom, double probability) noexcept;
    std::string getName () const noexcept;
};

#endif