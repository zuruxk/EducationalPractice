#include "MatrixMultProblem.hpp"

MatrixMultProblem::MatrixMultProblem (const std::vector<int>& dimensions) : dimensions(dimensions) {
    if (dimensions.size() < 3) {
        throw std::invalid_argument("Dimensions number must be 3 or more");
    }
    dimensionsCount = static_cast<int>(dimensions.size());
}

int MatrixMultProblem::getDimensionsCount () const {
    return dimensionsCount;
}

const std::vector<int>& MatrixMultProblem::getDimensions () const noexcept {
    return dimensions;
}

int MatrixMultProblem::getDimensionAt (int index) const {
    if (index < 0 || index >= dimensionsCount) {
        throw std::out_of_range("Index out of range");
    }
    return dimensions[index];
}

bool MatrixMultProblem::isValid () const noexcept {
    if (dimensions.size() < 3) {
        return false;
    }
    for (size_t i = 0; i < dimensionsCount; i++) {
        if (dimensions[i] <= 0) {
            return false;
        }
    }
    return true;
}

std::string MatrixMultProblem::toString () const noexcept {
    std::ostringstream ss;
    ss << "Matrix multiplication problem\n";
    ss << "Dimensions:\n";
    for (size_t i = 0; i < dimensionsCount - 1; i++) {
        ss << "[" << dimensions[i] << "," << dimensions[i+1] << "]" << "\n"; 
    }
    return ss.str();
}