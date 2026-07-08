#ifndef FITNESSFUNCTION_H
#define FITNESSFUNCTION_H

#include <list>

#include "MatrixMultProblem.hpp"
#include "Chromosome.hpp"

class FitnessFunction {
private:
    MatrixMultProblem problem;

    long long operationCost (int a, int b, int c) const;

public:
    FitnessFunction (const MatrixMultProblem& problem) noexcept;

    const MatrixMultProblem& getProblem () const noexcept;
    int calculate (const Chromosome& chrom) const noexcept;
};

#endif