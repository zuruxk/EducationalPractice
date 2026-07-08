#ifndef MATRIXMULTPROBLEM_H
#define MATRIXMULTPROBLEM_H

#include <vector>
#include <string>
#include <stdexcept>
#include <sstream>

class MatrixMultProblem {
private:
    std::vector<int> dimensions;
    int dimensionsCount;

public:
    MatrixMultProblem() = default;
    explicit MatrixMultProblem (const std::vector<int>& dimensions);

    int getDimensionsCount () const noexcept;
    const std::vector<int>& getDimensions () const noexcept;
    int getDimensionAt (int index) const;

    bool isValid () const noexcept;
    std::string toString () const noexcept;
};

#endif