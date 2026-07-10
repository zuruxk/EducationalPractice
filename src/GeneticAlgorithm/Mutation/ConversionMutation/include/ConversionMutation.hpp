#ifndef CONVERSIONMUTATION_H
#define CONVERSIONMUTATION_H

#include <algorithm>
#include "IMutation.hpp"

class ConversionMutation : public IMutation {
public:
    void mutate (Chromosome& chrom, double probability) noexcept;
    std::string getName () const noexcept;
};

#endif