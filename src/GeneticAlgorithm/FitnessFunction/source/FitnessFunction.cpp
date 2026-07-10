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
    for (int i = 1; i < N; i++) {
        indices.push_back(i);
    }
    for (int o: order) {
        int matrixNum = o + 1;
        auto it = std::find(indices.begin(), indices.end(), matrixNum);
        if (it == indices.end()) continue;
        int leftIdx = *it;
        it++;
        int rightIdx = *it;
        int a = dims[leftIdx - 1];
        int b = dims[rightIdx - 1];
        int c = dims[rightIdx];
        totalCost += operationCost(a, b, c);
        it--;
        indices.erase(it);
    }
    return totalCost;
}

long long FitnessFunction::operationCost (int a, int b, int c) const {
    return 1LL * a * b * c;
}