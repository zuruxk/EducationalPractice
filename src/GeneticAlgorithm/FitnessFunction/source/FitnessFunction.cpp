#include "../include/FitnessFunction.hpp"

FitnessFunction::FitnessFunction (const MatrixMultProblem& problem) noexcept : problem(problem) {
    if (!problem.isValid()) {
        throw std::invalid_argument("Invalid problem");
    }
}

const MatrixMultProblem& FitnessFunction::getProblem () const noexcept {
    return problem;
}

int FitnessFunction::calculate (const Chromosome& chrom) const noexcept {
    auto order = chrom.decodeToOrder();
    auto dims = problem.getDimensions();
    int N = problem.getDimensionsCount();
    long long totalCost = 0;
    std::list<int> indices;
    for (int i = 0; i <= N; i++) {
        indices.push_back(i);
    }
    for (int o: order) {
        auto it = indices.begin();
        std::advance(it, o);
        int pos = *it;
        int a = dims[pos-1];
        int b = dims[pos];
        int c = dims[pos+1];
        totalCost += operationCost(a, b, c);
        indices.erase(it);
    }
    return totalCost;
}

long long FitnessFunction::operationCost (int a, int b, int c) const {
    return 1LL * a * b * c;
}